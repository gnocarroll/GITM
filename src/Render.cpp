#include "src/Render.hpp"

#include "glad/glad.h"
#include <SDL2/SDL.h>

#include "src/Shader.hpp"
#include "src/Texture.hpp"
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

void renderScene() {
	Shader shader(
		{ "shaders/vertex.glsl", "shaders/frag.glsl" },
		{ GL_VERTEX_SHADER, GL_FRAGMENT_SHADER }
	);
	shader.Use();

	unsigned int VAO, VBO, EBO;

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
	};

	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	Texture tex("assets/container.jpg");

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER,
		sizeof(vertices),
		vertices,
		GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*) (3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
		(void*) (6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);



	glBindTexture(GL_TEXTURE_2D, tex.GetName());
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}