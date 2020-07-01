#include "Json.hpp"
#include <fstream>
#include "Format.hpp"

namespace oeng
{
	OEAPI Json ReadFileAsJson(const char* file)
	{
		std::ifstream is{file};
		if (!is.is_open()) throw std::runtime_error{format("Can't open file '{}'", file)};
		
		Json json;
		try { is >> json; }
		catch (const std::exception& e)
		{
			throw std::runtime_error{format("Failed to parse '{}': {}", file, e.what())};
		}
		
		return json;
	}
}
