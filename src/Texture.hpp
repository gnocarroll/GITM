#pragma once

#include <string_view>

class Texture {
private:
	int width;
	int height;
	int channels;

	unsigned name;

public:
	Texture(std::string_view filename);
	Texture(const Texture& other) = delete;
	Texture(Texture&& other) = default;

	~Texture();

	bool IsValid() const {
		return name;
	}

	auto GetWidth() const {
		return width;
	}

	auto GetHeight() const {
		return height;
	}

	auto GetChannels() const {
		return channels;
	}

	auto GetName() {
		return name;
	}
};