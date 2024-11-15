#include "src/Render.hpp"

#include "glad/glad.h"
#include <SDL2/SDL.h>

#include "src/Shader.hpp"
#include "src/Window.hpp"

bool setViewportWithWindow(const Window& window) {
	int w = -1;
	int h = -1;

	SDL_GL_GetDrawableSize(static_cast<SDL_Window*>(window.GetResource()),
		&w, &h);

	if ((w == -1) || (h == -1))
		return false;

	glViewport(0, 0, w, h);

	return true;
}

void swapWindow(const Window& window) {
	if (window.IsValid())
		SDL_GL_SwapWindow(static_cast<SDL_Window*>(window.GetResource()));
}

void renderScene() {
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
		(void*)0);
	glEnableVertexAttribArray(0);

	Shader shader(
		{ "shaders/vertex.glsl", "shaders/frag.glsl" },
		{ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }
	);
	shader.Use();

	glDrawArrays(GL_TRIANGLES, 0, 3);
}