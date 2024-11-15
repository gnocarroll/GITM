#pragma once

#include <initializer_list>
#include <string_view>

class Shader {
private:
	unsigned int progID;

public:
	// boolean true => shaders are already c strings, do not need to do file IO
	Shader(std::initializer_list<std::string_view> shaders,
		std::initializer_list<int> types, bool areStrings = false);
	Shader(const Shader& other) = delete;
	Shader(Shader&& other) = default;

	~Shader();

	unsigned int GetID() const {
		return progID;
	}

	bool IsValid() const {
		return (progID != 0);
	}

	void Use() const;
	int GetUniformLocation(std::string_view name) const;
};