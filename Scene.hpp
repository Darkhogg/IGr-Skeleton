#ifndef __SCENE__H__
#define __SCENE__H__

#include <SDL.h>
#include <vector>

typedef struct {
    Uint32 time;
    Uint32 code;
} KeyEvent;

class Scene {
    private:
        bool stopped;
        int exitCode;

        std::vector<bool> keysPressed;

        void initialize ();
        void update (float delta);
        void draw ();
        void processEvents ();

    protected:
        virtual void onInitialize () {}
        virtual void onUpdate (float delta) {}
        virtual void onDraw () {}
        virtual void onStop (int exitCode) {}
        virtual bool onQuit () {return true;}
        virtual void onKeyDown (const KeyEvent &key) {}
        virtual void onKeyUp (const KeyEvent &key) {}

        void quit ();
        void stop (int code);
        bool isKeyPressed (Uint32 code);

    public:
        Scene () : stopped(false), exitCode(0) {}
        virtual ~Scene () {}

        int run ();
};

#endif