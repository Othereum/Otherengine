#pragma once
#include <string>

namespace oeng
{
	struct Name
	{
		Name();
		Name(const char* s);
		Name(const std::string& s);
		Name(std::string&& s);

		[[nodiscard]] const std::string& Str() const noexcept { return *sp; }
		[[nodiscard]] const char* CStr() const noexcept { return sp->c_str(); }

		bool operator==(const Name&) const noexcept = default;
		bool operator!=(const Name&) const noexcept = default;

	private:
		friend std::hash<Name>;
		const std::string* sp;
	};
}

template <>
struct std::hash<oeng::Name>
{
	size_t operator()(const oeng::Name& key) const noexcept
	{
		return size_t(key.sp);
	}
};
