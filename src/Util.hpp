#pragma once

#include <iostream>

#include <SDL2/SDL_error.h>

inline void perrorSDL(const std::string& s) {
	std::cerr << s << ": " << SDL_GetError() << '\n';
}

template <auto func, auto errRet, typename... Args>
inline auto WrapSDLFunc(Args... args) {
	auto ret = func(args...);

	if (ret == errRet) {
		perrorSDL("WrapSDLFunc");
	}

	return ret;
}