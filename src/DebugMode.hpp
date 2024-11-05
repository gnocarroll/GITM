#pragma once

// NDEBUG - CMake
// _DEBUG - MSVC

#if defined(NDEBUG) || defined(_DEBUG)
	#define DEBUG_MODE (1)
#endif

#define MY_INFO_LOG_LEN (512)