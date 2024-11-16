#include "src/GLUtil.hpp"

#include <glad/glad.h>

static int targetToBinding(int target) {
	switch (target) {
	case GL_TEXTURE_1D:
		return GL_TEXTURE_BINDING_1D;
	case GL_TEXTURE_2D:
		return GL_TEXTURE_BINDING_2D;
	case GL_TEXTURE_3D:
		return GL_TEXTURE_BINDING_3D;
	default:
		break;
	}

	return -1;
}

static int bind(int target, unsigned name) {
	switch (target) {
	case GL_TEXTURE_1D:
	case GL_TEXTURE_2D:
	case GL_TEXTURE_3D:
		glBindTexture(target, name);
		break;
	default:
		return -1;
	}

	return 0;
}

TempBind::TempBind(int _target, unsigned _name) :
	target(_target), name(_name), oldName(0) {

	glGetIntegerv(targetToBinding(target),
		reinterpret_cast<int*>(&oldName));

	if (bind(target, name) == 0)
		return;

	target = 0;
	name = 0;
	oldName = 0;
}

TempBind::~TempBind() {
	if (target) {
		bind(target, oldName);
	}
}