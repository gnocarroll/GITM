#include "src/Util.hpp"

#include <fstream>

std::vector<char> fileToBuf(std::string_view filename) {
	static constexpr size_t initialBufSize = 1000;

	char cFilename[128];
	const char* filenamePtr;

	auto alloc = copyOrAlloc(cFilename, filename);

	filenamePtr = (alloc ? alloc.get() : cFilename);

	std::ifstream stream(filenamePtr, std::ios::binary);

	std::vector<char> ret(initialBufSize);
	size_t offset = 0;
	size_t retSize = initialBufSize;

	while (stream.read(ret.data() + offset, retSize - offset)) {
		offset += stream.gcount();
		
		if (offset < retSize)
			break;

		retSize *= 2;
		ret.resize(retSize);
	}

	ret.resize(offset);

	return ret;
}