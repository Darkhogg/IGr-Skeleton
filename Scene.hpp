#ifndef __SCENE__H__
#define __SCENE__H__

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

        void abort (std::string message);
        void initialize ();
        void update (float delta);
        void draw ();
        void processEvents ();
        void handleResize (int width, int height);
        void resetOrtho ();

    protected:
        virtual void onInitialize () {}
        virtual void onUpdate (float delta) {}
        virtual void onDraw () {}
        virtual void onStop (int exitCode) {}
        virtual bool onQuit () {return true;}
        virtual void onResize (int width, int height) {}
        virtual void onKeyDown (int code) {}
        virtual void onKeyUp (int code) {}

        void quit ();
        void stop (int code);
        bool isKeyPressed (int code);

        void resize (int width, int height);

        void zoomTo (double zoom);
        void zoom (double zoom);

        void moveTo (double x, double y);
        void move (double x, double y);

        void zoom () { return _zoom; }
        void x () { return _x; }
        void y () { return _y; }


    public:
        Scene () : _stopped(false), _exitCode(0),_width(640), _height(480), _zoom(0.0) {}
        virtual ~Scene () {}

        int run ();
};

#endif