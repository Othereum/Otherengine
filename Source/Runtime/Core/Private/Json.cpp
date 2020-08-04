#include "Json.hpp"
#include <fstream>
#include "Format.hpp"

CORE_BEGIN

CORE_API Json ReadFileAsJson(const fs::path& file)
{
	std::ifstream is{file};
	if (!is.is_open()) Throw(u8"Can't open file '{}'"sv, file.string<char8_t>(PoolAllocator<char8_t>{}));
	
	try
	{
		return Json::parse(is, nullptr, true, true);
	}
	catch (const std::exception& e)
	{
		Throw(u8"Failed to parse '{}': {}"sv, file.string<char8_t>(PoolAllocator<char8_t>{}), What(e));
	}
}

CORE_END
