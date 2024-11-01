#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "src/Setup.hpp"
#include "src/Window.hpp"

int main() {
	if (!initialSetup())
		std::exit(1);

	Window window("Test", 800, 600);

	if (!window.IsValid())
		std::exit(1);

	WrapGLContext ctx = window.GetContext();

	if (!ctx.IsValid())
		std::exit(1);

	if (!loadGL())
		std::exit(1);

	return 0;
}