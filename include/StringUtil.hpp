#pragma once
#include <string>
#include <string_view>

namespace game::str
{
	std::string ReadFile(std::string_view filename);

	template <class... Args>
	std::string Concat(std::string s, Args&&... args)
	{
		return (s.append(args), ...);
	}
}
