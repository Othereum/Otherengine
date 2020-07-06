#include "Json.hpp"
#include <fstream>
#include "Format.hpp"

namespace oeng
{
	OEAPI Json ReadFileAsJson(Path file)
	{
		std::ifstream is{*file};
		if (!is.is_open()) Throw(u8"Can't open file '{}'", file);
		
		Json json;
		try { is >> json; }
		catch (const std::exception& e)
		{
			Throw(u8"Failed to parse '{}': {}", file, What(e));
		}
		
		return json;
	}
}
