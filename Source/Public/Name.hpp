#pragma once
#include "JsonFwd.hpp"
#include "API.hpp"
#include "Templates/String.hpp"

namespace oeng
{
	using NameString = BasicString<Char, std::char_traits<Char>, std::allocator<Char>>;
	
	/**
	 * \brief Lightweight string
	 * \brief Very fast O(1) copy and comparison
	 * \brief No heap allocation on copy
	 * \brief Good to use as key for HashMap
	 * \note Case-insensitive
	 */
	struct OEAPI Name
	{
		Name() noexcept;
		Name(const char* s);
		Name(NameString s);

		[[nodiscard]] const NameString& Str() const noexcept { return *sp; }
		[[nodiscard]] const char* CStr() const noexcept { return sp->c_str(); }

		bool operator==(const Name&) const noexcept = default;

	private:
		friend std::hash<Name>;
		const NameString* sp;
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
