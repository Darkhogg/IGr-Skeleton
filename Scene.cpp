#include "Scene.hpp"

/* SDL and OpenGL */
#include <GL/glu.h>
#include <GL/glx.h>

#include <cmath>
#include <ctime>

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
    configScreen(1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    onInitialize();
}

int Scene::run () {
    initialize();

    Uint32 oldt = SDL_GetTicks();
    int lasted = 1;

    while (!_stopped) {
        update(lasted / 1000.0f);
        draw();

        Uint32 newt = SDL_GetTicks();
        lasted = newt-oldt;
        oldt = newt;
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
}

void Scene::draw () {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (std::vector<Camera2D>::iterator cam = _cameras.begin(); cam != _cameras.end(); ++cam) {
        // Viewport
        Quad vp = cam->vp();
        glViewport(
            _width*vp.left,
            _height - (_height*vp.top),
            _width*(vp.right-vp.left), 
            _height*(vp.top-vp.bottom)
        );

        // Projection
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        Quad view = cam->view(_width/_camCols, _height/_camRows);
        gluOrtho2D(view.left, view.right, view.bottom, view.top);

        onDraw();
    }
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

            case SDL_MOUSEMOTION: {
                _mouseX = event.motion.x;
                _mouseY = event.motion.y;

                findCurrentCamera();

                onMouseMove(_mouseX, _mouseY);

                break;
            }

            /* Mouse Event */
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                int btn = event.button.button;

                // Resize the keys array
                if (_mousePressed.size() < btn+1) {
                    _mousePressed.resize(btn+1);
                }

                // Call the event method
                if (event.type == SDL_MOUSEBUTTONDOWN) {
                    _mousePressed[btn] = true;
                    onMouseDown(btn);
                } else if (event.type == SDL_MOUSEBUTTONUP) {
                    _mousePressed[btn] = false;
                    onMouseUp(btn);
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

    // Update the local fields
    _width = width;
    _height = height;

    // Event
    onResize(width, height);
}

void Scene::zoomTo (double zoom) {
    _cameras[_cam].zoom(zoom);
}

void Scene::zoom (double zoom) {
    _cameras[_cam].zoom(_cameras[_cam].zoom() + zoom);
}

void Scene::moveTo (double x, double y) {
    _cameras[_cam].pos(Point2D(x, y));
}

void Scene::move (double x, double y) {
    _cameras[_cam].pos(Point2D(
        _cameras[_cam].pos().x + x / exp(_cameras[_cam].zoom()),
        _cameras[_cam].pos().y + y / exp(_cameras[_cam].zoom())
    ));
}

void Scene::configScreen (int rows, int cols) {
    _camRows = rows;
    _camCols = cols;

    _cameras.clear();

    GLdouble w = 1.0 / cols;
    GLdouble h = 1.0 / rows;

    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++) {
            Quad vp;
            vp.left = c*w;
            vp.right = (c+1)*w;
            vp.bottom = r*h;
            vp.top = (r+1)*h;

            Camera2D cam;
            cam.vp(vp);
            cam.zoom(log(1.0/std::max(rows, cols)));

            _cameras.push_back(cam);
        }
    }

    findCurrentCamera();
}

void Scene::findCurrentCamera () {
    double xr = (double) _mouseX / _width;
    double yr = (double) _mouseY / _height;

    for (int i = 0; i < _cameras.size(); i++) {
        Quad vp = _cameras[i].vp();
        if (xr >= vp.left && xr < vp.right && yr >= vp.bottom && yr < vp.top) {
            _cam = i;
        }
    }
}

Point2D Scene::getMouseWorldPosition () {
    Quad vp = cam().vp();
    Quad view = cam().view(_width/_camCols, _height/_camRows);

    /* Normalized mouse position */
    Point2D mr (
        (double) _mouseX / _width,
        (double) _mouseY / _height);

    /* Within-camera mouse position */
    Point2D cmp (
        (mr.x - vp.left) / (vp.right - vp.left),
        (mr.y - vp.top) / (vp.bottom - vp.top));

    Point2D mwp (
        view.left + cmp.x*(view.right-view.left),
        view.bottom + cmp.y*(view.top-view.bottom));

    return mwp;
}