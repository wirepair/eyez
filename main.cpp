#include <iostream>
#include <memory>
#include <algorithm>
#include "EyeWindow.h"
#include "Eye.h"
#include "time.h"

void runGLLoop(std::unique_ptr<EyeWindow>& window, std::unique_ptr<Eye>& eye);

int main(int argc, char* args[])
{
	std::string vertexPath = "res/vertex.glsl";
	std::string fragmentPath = "res/fragment.glsl";

	std::unique_ptr<EyeWindow> eyeWindow(new EyeWindow(SCREEN_WIDTH, SCREEN_HEIGHT));
	if (!eyeWindow->Init())
	{
		return -1;
	}
	
	std::unique_ptr<Eye> eye(new Eye(vertexPath, fragmentPath));
	if (!eye->Init())
	{
		std::cout << "Failed to load our eye!" << std::endl;
		return -1;
	}
	
	// Debugging, disable for better perf
	if (!eye->SetMonitorFragmentFile())
	{
		return -1;
	}
	runGLLoop(eyeWindow, eye);

}

void runGLLoop(std::unique_ptr<EyeWindow>& window, std::unique_ptr<Eye>& eye)
{
	bool running = true;

	//Event handler
	SDL_Event e;
	float frameCount = 0.0f;
	float startTime = 0.0f;
	while(running)
	{
		//Handle events on queue
		while(SDL_PollEvent( &e ) != 0)
		{
			//User requests quit
			running = !(e.type == SDL_QUIT);
			// Handle keys
			if (e.type == SDL_KEYDOWN)
			{
				switch(e.key.keysym.sym)
				{
					case SDLK_r:
						eye->Reload();
						std::cout << "Reloading shaders." << std::endl;
						break;
					case SDLK_f:
						eye->SetFocus();
						break;
					case SDLK_ESCAPE:
						running = false;
						break;
				}
			}
		}

		// clear screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// calculate delta
		float deltaTime = (SDL_GetTicks() - startTime) / 1000.f;
		eye->Update(deltaTime);

		// ensure FRAMES_PER_SECOND		
		startTime = SDL_GetTicks();
		SDL_Delay((1000 / FRAMES_PER_SECOND));

		SDL_GL_SwapWindow(window->GetWindow());
	}
	
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}