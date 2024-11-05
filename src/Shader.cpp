#include "src/Shader.hpp"

#include <array>
#include <initializer_list>
#include <ranges>

#include <glad/glad.h>

#include "src/DebugMode.hpp"
#include "src/Util.hpp"

#define MAX_SHADERS (10)

struct RAII_CreateShader {
	int ref;

	RAII_CreateShader() : ref(0) {}
	RAII_CreateShader(int shaderType) {
		ref = glCreateShader(shaderType);
	}
	RAII_CreateShader(RAII_CreateShader&& other) = default;

	~RAII_CreateShader() {
		if (ref)
			glDeleteShader(ref);
	}

	void Init(int shaderType) {
		if (ref)
			glDeleteShader(ref);

		ref = glCreateShader(shaderType);
	}

	bool IsValid() {
		return (ref != 0);
	}
};

Shader::Shader(std::initializer_list<const char*> shaders,
	std::initializer_list<int> types, bool areStrings) : progID(0) {

	// The error-checking code that collects information to be put in an
	// infoLog is based heavily on code from the Shaders lesson on
	// learnopengl.com

	if (shaders.size() != types.size()) {
		perrorCustom("Shader", "number of shaders should equal number of types");
		return;
	}

	size_t nShaders = shaders.size();

	if ((nShaders == 0) || (nShaders > MAX_SHADERS)) {
		perrorCustom("Shader", "must have between 1 and MAX_SHADERS shaders");
		return;
	}

	std::array<RAII_CreateShader, MAX_SHADERS> createShaders;

	if (!areStrings) {
		std::cerr << "must provide c strings, file IO not implemented yet\n";
		return;
	}

	for (const auto& [createShader, shader, type] : std::views::zip(createShaders, shaders, types)) {
		createShader.Init(type);
		
		if (!createShader.IsValid()) {
			perrorCustom("Shader", "shader creation failed");
			return;
		}

		glShaderSource(createShader.ref, 1, &shader, NULL);
		glCompileShader(createShader.ref);

#ifdef DEBUG_MODE
		int success = 0;
		char infoLog[MY_INFO_LOG_LEN];

		glGetShaderiv(createShader.ref, GL_COMPILE_STATUS, &success);
		
		if (!success) {
			glGetShaderInfoLog(createShader.ref, MY_INFO_LOG_LEN, NULL, infoLog);
			perrorCustom("Shader", infoLog);
		}
#endif
	}

	progID = glCreateProgram();

	for (size_t i = 0; i < nShaders; i++) {
		glAttachShader(progID, createShaders[i].ref);
	}

	glLinkProgram(progID);

#ifdef DEBUG_MODE
	int success = 0;
	char infoLog[MY_INFO_LOG_LEN];

	glGetProgramiv(progID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(progID, 512, NULL, infoLog);
		perrorCustom("Shader", "program linking failed");
		progID = 0;
	}
#endif
}

Shader::~Shader() {
	if (progID)
		glDeleteProgram(progID);
}

void Shader::Use() const {
	glUseProgram(progID);
}

int Shader::GetUniformLocation(const std::string& name) const {
	return glGetUniformLocation(progID, name.c_str());
}