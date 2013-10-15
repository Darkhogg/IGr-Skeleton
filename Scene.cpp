#include "Scene.hpp"

/* Standard C */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <iostream>

/* SDL and OpenGL */
#include <GL/glu.h>
#include <GL/glext.h>
#include <GL/glx.h>
#include <GL/glxext.h>

/* Unix */
//#include <math.h>
#include <time.h>
#include <unistd.h>

void Scene::initialize () {
    putenv((char*) "SDL_VIDEO_CENTERED=1");

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "Failed to initialize SDL Video!\n");
        exit(1);
    }

    // tell system which funciton to process when exit() call is made
    atexit(SDL_Quit);

    // get optimal video settings
    const SDL_VideoInfo* vidinfo = SDL_GetVideoInfo();
    if (!vidinfo) {
        fprintf(stderr, "Coudn't get video information! [%s]\n", SDL_GetError());
        exit(1);
    }

    // set opengl attributes
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    // get a framebuffer
    SDL_Surface* gDrawSurface = SDL_SetVideoMode(
        100, 100, vidinfo->vfmt->BitsPerPixel, SDL_OPENGL);

    if (!gDrawSurface) {
        fprintf(stderr, "Couldn't set video mode! [%s]\n", SDL_GetError());
        exit(1);
    }

    onInitialize();
}

int Scene::run () {
    initialize();

    while (!stopped) {
        update(1.0f / 60.0f);
        draw();

        usleep(1000000 / 60);
    }

    return exitCode;
}

void Scene::quit () {
    if (onQuit()) {
        stop(0);
    }
}

void Scene::stop (int code) {
    if (!stopped) {
        stopped = true;
        exitCode = code;
        onStop(code);
    }
}

void Scene::update (float delta) {
    processEvents();
    onUpdate(delta);
}

void Scene::draw () {
    onDraw();
    SDL_GL_SwapBuffers();
}

void Scene::processEvents () {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            /* Quit the scene */
            case SDL_QUIT: {
                quit();
                break;
            }

            /* Keyboard Event */
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                SDL_KeyboardEvent keyEvent = event.key;

                KeyEvent key;
                key.code = keyEvent.keysym.sym;

                // Resize the keys array
                if (keysPressed.size() < key.code+1) {
                    keysPressed.resize(key.code+1);
                }

                // Call the event method
                if (event.type == SDL_KEYDOWN) {
                    keysPressed[key.code] = true;
                    onKeyDown(key);
                } else if (event.type == SDL_KEYUP) {
                    keysPressed[key.code] = false;
                    onKeyUp(key);
                }

                break;
            }
        }
    }
}

bool Scene::isKeyPressed (Uint32 code) {
    if (keysPressed.size() < code+1) {
        return false;
    }

    return keysPressed[code];
}