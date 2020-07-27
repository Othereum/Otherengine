#include "Json.hpp"
#include <fstream>
#include "Format.hpp"

namespace oeng::core
{
	CORE_API Json ReadFileAsJson(Path file)
	{
		std::ifstream is{*file};
		if (!is.is_open()) Throw(u8"Can't open file '{}'"sv, file.Str());
		
		try
		{
			return Json::parse(is, nullptr, true, true);
		}
		catch (const std::exception& e)
		{
			Throw(u8"Failed to parse '{}': {}"sv, file.Str(), What(e));
		}
	}
}
