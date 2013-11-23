#ifndef __SCENE__H__
#define __SCENE__H__

#include "Camera2D.hpp"
#include "Vect.hpp"
#include "types.h"

#include <SDL.h>
#include <vector>
#include <string>

typedef struct {
    Uint32 time;
    Uint32 code;
} KeyEvent;

class Scene {
    private:
        bool _stopped, _paused;
        int _exitCode;
        float _nextTick = 0;

        int _width, _height;
        int _mouseX, _mouseY;

        std::vector<bool> _keysPressed;
        std::vector<bool> _mousePressed;

        std::vector<Camera2D> _cameras;
        int _cam, _camCols, _camRows;

        void abort (std::string message);
        void initialize ();
        void update (float delta);
        void draw ();

        void processEvents ();
        void handleResize (int width, int height);
        void findCurrentCamera ();

    protected:
        virtual void onInitialize () {}
        virtual void onUpdate (float delta) {}
        virtual void onDraw () {}
        virtual void onStop (int exitCode) {}
        virtual bool onQuit () {return true;}
        virtual void onResize (int width, int height) {}
        virtual void onKeyDown (int code) {}
        virtual void onKeyUp (int code) {}
        virtual void onMouseMove (int x, int y) {}
        virtual void onMouseDown (int button) {}
        virtual void onMouseUp (int button) {}

        void quit ();
        void stop (int code);

        void pause () { _paused = true; }
        void resume () { _paused = false; }

        void tick (float delta) { _nextTick = delta; }

        bool isPaused () { return _paused; }

        bool isKeyPressed (int code);
        bool isMousePressed (int button);

        Vect getMouseScreenPosition () {return Vect(_mouseX, _mouseY);}
        Vect getMouseWorldPosition ();

        void resize (int width, int height);

        int width () { return _width; }
        int height () { return _height; }

        void zoomTo (double zoom);
        void zoom (double zoom);

        void moveTo (double x, double y);
        void move (double x, double y);

        void setTitle (std::string title) {
            SDL_WM_SetCaption(title.c_str(), 0); 
        }

        Camera2D cam () { return _cameras[_cam]; }

        void configScreen (int rows, int cols);

    public:
        Scene () : _stopped(false), _exitCode(0),_width(640), _height(480) {}
        virtual ~Scene () {}

        int run ();
};

#endif