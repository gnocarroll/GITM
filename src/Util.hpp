#pragma once

#include <cstring>
#include <iostream>
#include <memory>
#include <string_view>
#include <vector>

#include <SDL2/SDL_error.h>

inline void perrorCustom(const std::string& s, const std::string& msg) {
	std::cerr << s << ": " << msg << '\n';
}

inline void perrorSDL(const std::string& s) {
	std::cerr << s << ": " << SDL_GetError() << '\n';
}

inline bool ltZero(int arg) {
	return arg < 0;
}

inline bool isNull(void* ptr) {
	return (ptr == nullptr);
}

template <auto func, auto checkErr, typename... Args>
inline auto WrapSDLFunc(Args... args) {
	auto ret = func(args...);

	if (checkErr(ret)) {
		perrorSDL("WrapSDLFunc");
	}

	return ret;
}

/*
 * If you need to create a null-terminated string from a string_view but
 * would prefer to use a stack-allocated buffer, this function will
 * - copy to the buffer and null-terminate if possible
 * - otherwise dynamically allocate buffer
 * 
 * RETURN: nullptr if no allocation, otherwise pointer to allocation
 */
template <size_t bufSize>
std::unique_ptr<char> copyOrAlloc(char (&buf)[bufSize], std::string_view src) {
	size_t srcLen = src.size();
	
	if (srcLen < bufSize) {
		std::memcpy(buf, src.data(), srcLen);
		buf[srcLen] = '\0';

		return nullptr;
	}

	auto ret = std::unique_ptr<char>(new char[srcLen + 1]);
	std::memcpy(ret.get(), src.data(), srcLen);
	ret.get()[srcLen] = '\0';

	return ret;
}

// read file into buffer
std::vector<char> fileToBuf(std::string_view filename);