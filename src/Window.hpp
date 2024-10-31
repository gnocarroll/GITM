#pragma once

#include <string>

class Window {
private:
	std::string title;
	void* resource;
	int w;
	int h;

public:
	Window(const std::string& _title);
	Window(const std::string& _title, int _w, int _h);

	Window(const Window& other) = delete;
	Window(Window&& other) = default;

	~Window();

	bool IsValid() {
		return (resource != nullptr);
	}
};