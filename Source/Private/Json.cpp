#include "Json.hpp"
#include <fstream>
#include "Format.hpp"

namespace oeng
{
	OEAPI Json ReadFileAsJson(Path file)
	{
		std::ifstream is{file};
		if (!is.is_open()) throw std::runtime_error{format("Can't open file '{}'", file->string())};
		
		Json json;
		try { is >> json; }
		catch (const std::exception& e)
		{
			throw std::runtime_error{format("Failed to parse '{}': {}", file->string(), e.what())};
		}
		
		return json;
	}
}
