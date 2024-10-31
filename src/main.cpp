#include <cstdlib>
#include <iostream>

#include "src/Window.hpp"

int main() {
	Window window("Test", 800, 600);

	if (!window.IsValid())
		std::exit(1);

	return 0;
}