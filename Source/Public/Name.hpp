#pragma once
#include <string>
#include "JsonFwd.hpp"

namespace oeng
{
	struct Name
	{
		Name() noexcept;
		Name(const char* s);
		Name(const std::string& s);
		Name(std::string&& s);

		[[nodiscard]] const std::string& Str() const noexcept { return *sp; }
		[[nodiscard]] const char* CStr() const noexcept { return sp->c_str(); }

		bool operator==(const Name&) const noexcept = default;

	private:
		friend std::hash<Name>;
		const std::string* sp;
	};
	
	void to_json(Json& json, const Name& name);
	void from_json(const Json& json, Name& name);
}

template <>
struct std::hash<oeng::Name>
{
	size_t operator()(const oeng::Name& key) const noexcept
	{
		return size_t(key.sp);
	}
};
