#include "Scene.hpp"

/* SDL and OpenGL */
#include <GL/glu.h>
#include <GL/glx.h>

#include <math.h>

#include <iostream>

void Scene::abort (std::string message) {
    fprintf(stderr, "%s [%s]\n", message.c_str(), SDL_GetError());
    exit(1);
}

void Scene::initialize () {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        abort("Failed to initialize SDL Video!\n");
    }

    // tell system which funciton to process when exit() call is made
    atexit(SDL_Quit);

    // set opengl attributes
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    resize(_width, _height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    onInitialize();
}

int Scene::run () {
    initialize();

    while (!_stopped) {
        update(1.0f / 60.0f);
        draw();

        //usleep(1000000 / 60);
    }

    return _exitCode;
}

void Scene::quit () {
    if (onQuit()) {
        stop(0);
    }
}

void Scene::stop (int code) {
    if (!_stopped) {
        _stopped = true;
        _exitCode = code;
        onStop(code);
    }
}

void Scene::update (float delta) {
    processEvents();
    onUpdate(delta);

    resetOrtho();
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

            case SDL_VIDEORESIZE: {
                std::cout << event.resize.w << ", " << event.resize.h << std::endl;
                handleResize(event.resize.w, event.resize.h);
                break;
            }

            /* Keyboard Event */
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                int code = event.key.keysym.sym;

                // Resize the keys array
                if (_keysPressed.size() < code+1) {
                    _keysPressed.resize(code+1);
                }

                // Call the event method
                if (event.type == SDL_KEYDOWN) {
                    _keysPressed[code] = true;
                    onKeyDown(code);
                } else if (event.type == SDL_KEYUP) {
                    _keysPressed[code] = false;
                    onKeyUp(code);
                }

                break;
            }
        }
    }
}

bool Scene::isKeyPressed (int code) {
    if (_keysPressed.size() < code+1) {
        return false;
    }

    return _keysPressed[code];
}

void Scene::resize (int width, int height) {
    handleResize(width, height);
}

void Scene::handleResize (int width, int height) {
    // Resize the OpenGL Viewport
    SDL_SetVideoMode(width, height, 32, SDL_OPENGL|SDL_RESIZABLE);
    glViewport(0.0f, 0.0f, width, height);

    // Keep the zoom, change the aspect
    resetOrtho();

    // Update the local fields
    _width = width;
    _height = height;

    // Event
    onResize(width, height);
}

void Scene::resetOrtho () {
    double zmult = 0.5 / exp(_zoom);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(
        - _width * zmult + _x, _width * zmult + _x,
        - _height * zmult + _y, _height * zmult + _y
    );
}

void Scene::zoomTo (double zoom) {
    _zoom = zoom;
    resetOrtho();
}

void Scene::zoom (double zoom) {
    _zoom += zoom;
    resetOrtho();
}

void Scene::moveTo (double x, double y) {
    _x = x;
    _y = y;
    resetOrtho();
}

void Scene::move (double x, double y) {
    _x += x / exp(_zoom);
    _y += y / exp(_zoom);
    resetOrtho();
}