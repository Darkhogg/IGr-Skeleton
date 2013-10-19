#ifndef __SCENE__H__
#define __SCENE__H__

#include "Camera2D.hpp"

#include <SDL.h>
#include <vector>
#include <string>

typedef struct {
    Uint32 time;
    Uint32 code;
} KeyEvent;

class Scene {
    private:
        bool _stopped;
        int _exitCode;

        int _width, _height;
        double _x, _y;
        double _zoom;

        std::vector<bool> _keysPressed;
        std::vector<bool> _mousePressed;
        std::vector<Camera2D> _cameras;
        int _cam;

        void abort (std::string message);
        void initialize ();
        void update (float delta);
        void draw ();
        void processEvents ();
        void handleResize (int width, int height);

    protected:
        virtual void onInitialize () {}
        virtual void onUpdate (float delta) {}
        virtual void onDraw () {}
        virtual void onStop (int exitCode) {}
        virtual bool onQuit () {return true;}
        virtual void onResize (int width, int height) {}
        virtual void onKeyDown (int code) {}
        virtual void onKeyUp (int code) {}
        virtual void onMouseDown (int button) {}
        virtual void onMouseUp (int button) {}

        void quit ();
        void stop (int code);
        bool isKeyPressed (int code);
        bool isMousePressed (int button);

        void resize (int width, int height);

        void zoomTo (double zoom);
        void zoom (double zoom);

        void moveTo (double x, double y);
        void move (double x, double y);

        double zoom () { return _zoom; }
        double x () { return _x; }
        double y () { return _y; }

        void configScreen (int rows, int cols);

    public:
        Scene () : _stopped(false), _exitCode(0),_width(640), _height(480), _zoom(0.0) {}
        virtual ~Scene () {}

        int run ();
};

#endif