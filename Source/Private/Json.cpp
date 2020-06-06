#include "Json.hpp"
#include <fstream>

namespace oeng
{
	OEAPI Json ReadFileAsJson(Path file)
	{
		Json json;
		std::ifstream{file} >> json;
		return json;
	}
}
