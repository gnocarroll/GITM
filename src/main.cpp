#include <SDL2/SDL_keycode.h>

#include "src/Events.hpp"
#include "src/Render.hpp"
#include "src/Setup.hpp"
#include "src/Window.hpp"

#include "src/Mat.hpp"

int main() {
	M44 rot = rotation<4>(90.0f, V3(0.0f, 1.0f, 0.0f));

	std::cout << (rot * V4(1.0f, 0.0f, 0.0f, 1.0f)) << '\n';

	return 0;

	if (!initialSetup())
		return 1;

	Window window("Test", 800, 600);

	if (!window.IsValid())
		return 1;

	WrapGLContext ctx = window.GetContext();

	if (!ctx.IsValid())
		return 1;

	if (!loadGL())
		return 1;

	// setViewportWithWindow calls glViewport internally

	if (!setViewportWithWindow(window))
		return 1;

	while (!shouldQuit()) {
		pollEvents();

		renderScene();

		window.SwapBuffer();

		if (keyThisFrame(SDLK_ESCAPE))
			setShouldQuit(true);
	}

	return 0;
}