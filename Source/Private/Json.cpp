#include "Json.hpp"
#include <fstream>
#include "Format.hpp"

namespace oeng
{
	OEAPI Json ReadFileAsJson(Path file)
	{
		Json json;
		std::ifstream is{file};
		if (!is.is_open()) throw std::runtime_error{format("Can't open file '{}'", file->string())};
		try { is >> json; }
		catch (const std::exception& e)
		{
			throw std::runtime_error{format("Exception thrown while parsing '{}': {}", file->string(), e.what())};
		}
		return json;
	}
}
