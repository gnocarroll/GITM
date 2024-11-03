#include "src/Render.hpp"

#include "glad/glad.h"
#include <SDL2/SDL.h>

#include "src/Window.hpp"

bool setViewportWithWindow(const Window& window) {
	int w = -1;
	int h = -1;

	SDL_GL_GetDrawableSize(static_cast<SDL_Window*>(window.GetResource()),
		&w, &h);

	if ((w == -1) || (h == -1))
		return false;

	glViewport(0, 0, w, h);

	return true;
}