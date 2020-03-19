#pragma once
#include <string>
#include <unordered_set>

namespace Game
{
	struct FName
	{
		FName();
		FName(const std::string& s);
		FName(std::string&& s);

		[[nodiscard]] auto& Str() const noexcept
		{
			return *s_;
		}

		operator const std::string&() const noexcept
		{
			return *s_;
		}

		bool operator==(const FName&) const noexcept = default;
		bool operator!=(const FName&) const noexcept = default;

	private:
		friend std::hash<FName>;
		static std::unordered_set<std::string> set_;
		const std::string* s_;
	};
}

template <>
struct std::hash<Game::FName>
{
	size_t operator()(const Game::FName& key) const noexcept
	{
		return size_t(key.s_);
	}
};
