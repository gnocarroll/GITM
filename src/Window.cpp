#include "src/Window.hpp"

#include <string>

#include <SDL2/SDL_video.h>

#include "src/Util.hpp"

Window::Window(const std::string& _title) : title(_title), w(0), h(0), resource(0) {
	resource = WrapSDLFunc<SDL_CreateWindow, isNull>(title.c_str(), 0, 0, w, h,
		SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL);
}
Window::Window(const std::string& _title, int _w, int _h) :
	title(_title), w(_w), h(_h), resource(0) {
	resource = WrapSDLFunc<SDL_CreateWindow, isNull>(title.c_str(),
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		w, h,
		SDL_WINDOW_OPENGL);
}

Window::~Window() {
	if (resource)
		SDL_DestroyWindow(static_cast<SDL_Window*>(resource));
}

void Window::SwapBuffer() const {
	if (resource)
		SDL_GL_SwapWindow(static_cast<SDL_Window*>(resource));
}