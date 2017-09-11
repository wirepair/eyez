#include "EyeWindow.h"

const char* title = "Eyez";

bool EyeWindow::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
		return false;
	}

	if (!initWindow() || !initContext())
	{
		return false;
	}

	return true;
}

bool EyeWindow::initWindow()
{
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);

	window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
							  width, height, SDL_WINDOW_OPENGL);
	if (window == nullptr)	
	{
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

bool EyeWindow::initContext()
{
	gl_context = SDL_GL_CreateContext(window);
	if (gl_context == nullptr)
	{
		std::cout << "SDL_GL_CreateContext Error: " << SDL_GetError() << std::endl;
		return false;
	}
	return true;
}

EyeWindow::~EyeWindow()
{
	if (gl_context != nullptr)
	{
		SDL_GL_DeleteContext(gl_context);
	}
	
	if (window != nullptr)
	{
		SDL_DestroyWindow(window);
	}
	SDL_Quit();
}