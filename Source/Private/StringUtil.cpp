#include "StringUtil.hpp"
#include <fstream>

namespace oeng::str
{
	std::string ReadFile(std::string_view filename)
	{
		std::ifstream file{filename.data(), std::ios_base::in | std::ios_base::ate};
		if (!file.is_open()) throw std::ios_base::failure{Concat("Cannot read file. File not found: ", filename)};

		std::string code(file.tellg(), 0);
		file.seekg(0);
		file.read(code.data(), code.size());
		return code;
	}
}
