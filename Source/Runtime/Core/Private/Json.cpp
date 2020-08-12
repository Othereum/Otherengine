#include "Json.hpp"
#include <fstream>

namespace oeng::core
{
	CORE_API Json ReadFileAsJson(const fs::path& file)
	{
		std::ifstream is{file};
		if (!is.is_open()) throw std::runtime_error{fmt::format("Can't open file '{}'"sv, file.string())};
		
		try
		{
			return Json::parse(is, nullptr, true, true);
		}
		catch (const std::exception& e)
		{
			throw std::runtime_error{fmt::format("Failed to parse '{}': {}", file.string(), e.what())};
		}
	}
}
