#pragma once
#include <filesystem>
#include "Name.hpp"

namespace oeng
{
	/**
	 * \brief Lightweight representation of filepath.
	 * Very fast O(1) copy and comparison.
	 * No heap allocation on copy.
	 * Good to use as key for Map/Set.
	 * \note Case-insensitive on any platform
	 * \note Comparisons are not lexical.
	 */
	struct OEAPI Path
	{
		using Pair = std::pair<const Name, std::filesystem::path>;
		
		Path() noexcept;
		Path(const std::filesystem::path& path);
		Path(const char* s) :Path{std::filesystem::path{s}} {}

		bool operator==(const Path& r) const noexcept { return p == r.p; }
		bool operator!=(const Path& r) const noexcept { return p != r.p; }
		bool operator<(const Path& r) const noexcept { return p < r.p; }
		bool operator>(const Path& r) const noexcept { return p > r.p; }
		bool operator<=(const Path& r) const noexcept { return p <= r.p; }
		bool operator>=(const Path& r) const noexcept { return p >= r.p; }

		[[nodiscard]] Name AsName() const noexcept { return p->first; }
		[[nodiscard]] const char* CStr() const noexcept { return p->first->c_str(); }
		[[nodiscard]] const std::string& Str() const noexcept { return p->first; }

		operator Name() const noexcept { return AsName(); }
		operator const char*() const noexcept { return CStr(); }
		operator const std::string&() const noexcept { return Str(); }
		operator const std::filesystem::path&() const noexcept { return p->second; }
		const std::filesystem::path& operator*() const noexcept { return p->second; }
		const std::filesystem::path* operator->() const noexcept { return &p->second; }

	private:
		const Pair* p;
	};
	
	OEAPI void to_json(Json& json, const Path& path);
	OEAPI void from_json(const Json& json, Path& path);
}

template <>
struct std::hash<oeng::Path>
{
	size_t operator()(const oeng::Path& key) const noexcept
	{
		return size_t(&*key);
	}
};
