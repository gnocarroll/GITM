#include "src/Setup.hpp"

#include <glad/glad.h>
#include <SDL2/SDL.h>

#include "src/Util.hpp"

#define MY_GL_PROFILE       (SDL_GL_CONTEXT_PROFILE_CORE)
#define MY_GL_MAJOR_VERSION (3)
#define MY_GL_MINOR_VERSION (3)

#define DEPTH_BUF_BITS      (24)


bool initialSetup() {
	// Init all SDL subsystems (e.g. video, audio)

	if (WrapSDLFunc<SDL_InitSubSystem, ltZero>(SDL_INIT_EVERYTHING))
		return false;

	// SDL_Quit handles exit of all subsystems in one call

	if (std::atexit(SDL_Quit)) {
		std::perror("atexit");
		return false;
	}

	// Set OpenGL attributes as desired

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, MY_GL_PROFILE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, MY_GL_MAJOR_VERSION);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, MY_GL_MINOR_VERSION);

	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, DEPTH_BUF_BITS);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	// Load OpenGL using SDL2

	if (WrapSDLFunc<SDL_GL_LoadLibrary, ltZero>(nullptr))
		return false;

	return true;
}

bool loadGL() {
	// GLAD can get us the function pointers

	if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
		return false;

	return true;
}