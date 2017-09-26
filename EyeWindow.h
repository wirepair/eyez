#ifndef EYEWINDOW_H
#define EYEWINDOW_H

#include <iostream>
#include "sdl_headers.h"

class EyeWindow 
{
public:
    /** Constructor taking window width & height. **/
    EyeWindow(const int width, const int height) : width(width), height(height) {}

    /** Initializes the window, renderer, and anything else we need. **/
    bool Init();

    inline SDL_Window* GetWindow() { return window; };

    inline SDL_GLContext GetContext() { return gl_context; }

    /** Destroys the resources for this window **/
    ~EyeWindow();

private:
    /** Initializes the window **/
    bool initWindow();

    /** Initializes the gl context **/
    bool initContext();

private:
    SDL_Window* window;
    SDL_GLContext gl_context;
    
    int width;
    int height;
};

#endif // EYEWINDOW_H