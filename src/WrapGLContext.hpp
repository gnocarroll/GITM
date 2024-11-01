#pragma once

class WrapGLContext {
private:
	void* resource;

public:
	WrapGLContext(void* window);

	WrapGLContext(const WrapGLContext& other) = delete;
	WrapGLContext(WrapGLContext&& other) = default;

	~WrapGLContext();

	bool IsValid() const {
		return (resource != nullptr);
	}
};