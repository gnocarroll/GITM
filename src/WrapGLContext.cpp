#include "src/WrapGLContext.hpp"

#include <SDL2/SDL_video.h>

#include "src/Util.hpp"

WrapGLContext::WrapGLContext(void* window) : resource(0) {
	resource = WrapSDLFunc<SDL_GL_CreateContext, isNull>(static_cast<SDL_Window*>(window));
}

WrapGLContext::~WrapGLContext() {
	if (resource)
		SDL_GL_DeleteContext(static_cast<SDL_GLContext>(resource));
}