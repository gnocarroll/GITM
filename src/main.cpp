#include "src/Events.hpp"
#include "src/Setup.hpp"
#include "src/Window.hpp"

int main() {
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

	while (!shouldQuit()) {
		pollEvents();
	}

	return 0;
}