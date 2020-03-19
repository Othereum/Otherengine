#pragma once
#include <string>
#include <unordered_map>

namespace Game
{
	struct FName
	{
		FName() noexcept = default;
		
		template <class T>
		FName(std::basic_string_view<T> s)
		{
			static std::unordered_map<std::string, unsigned> map;
			if (auto found = map.find(s); found != map.end())
			{
				v_ = found->second;
			}
			else
			{
				static unsigned i = 0;
				map.emplace(s, ++i);
				v_ = i;
			}
		}

		bool operator==(const FName&) const noexcept = default;
		bool operator!=(const FName&) const noexcept = default;

	private:
		friend std::hash<FName>;
		unsigned v_ = 0;
	};
}

template <>
struct std::hash<Game::FName>
{
	size_t operator()(const Game::FName& key) const noexcept
	{
		return key.v_;
	}
};
