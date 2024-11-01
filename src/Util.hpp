#pragma once

#include <iostream>

#include <SDL2/SDL_error.h>

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