#include "src/Texture.hpp"

#include <glad/glad.h>

#include "src/GLUtil.hpp"
#include "src/stb_image.h"
#include "src/Util.hpp"

class STBImage {
private:
	unsigned char* data;

public:
	template <typename... Args>
	STBImage(Args... args) {
		data = stbi_load(args...);
	}
	STBImage(const STBImage& other) = delete;
	STBImage(STBImage&& other) = default;

	~STBImage() {
		if (data)
			stbi_image_free(data);
	}

	bool IsValid() {
		return data;
	}

	unsigned char* GetData() const {
		return data;
	}
};

Texture::Texture(std::string_view filename) : width(0),
	height(0), channels(0), name(0) {
	char filenameBuf[128];

	auto alloc = copyOrAlloc(filenameBuf, filename);

	STBImage img(
		(alloc ? alloc.get() : filenameBuf),
		&width, &height, &channels, 0
	);

	if (!img.IsValid()) {
		perrorCustom("Texture", "image loading failed");
		return;
	}
	else if ((channels != 3) && (channels != 4)) {
		perrorCustom("Texture", "unsupported number of channels");
		return;
	}

	glGenTextures(1, &name);
	TempBind tmp(GL_TEXTURE_2D, name);

	// Set texture preferences
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int format = (channels == 3 ? GL_RGB : GL_RGBA);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		format,
		width,
		height,
		0,
		format,
		GL_UNSIGNED_BYTE,
		img.GetData());
	glGenerateMipmap(GL_TEXTURE_2D);
}

Texture::~Texture() {
	if (name)
		glDeleteTextures(1, &name);
}