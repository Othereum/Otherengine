#pragma once
#include <string>
#include <unordered_set>

namespace oeng
{
	struct FName
	{
		FName();
		FName(const char* s);
		FName(const std::string& s);
		FName(std::string&& s);

		[[nodiscard]] const std::string& Str() const noexcept
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
		const std::string* s_;
	};
}

template <>
struct std::hash<oeng::FName>
{
	size_t operator()(const oeng::FName& key) const noexcept
	{
		return size_t(key.s_);
	}
};
