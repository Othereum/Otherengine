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
		Path(const char* s) :Path{Name{s}} {}
		Path(Name path);
		Path(std::filesystem::path&& path);
		Path(const std::filesystem::path& path);

		std::strong_ordering operator<=>(const Path&) const noexcept = default;

		[[nodiscard]] Name AsName() const noexcept { return p->first; }
		[[nodiscard]] const Name::Str& Str() const noexcept { return p->first; }
		operator Name() const noexcept { return p->first; }
		operator const Name::Str&() const noexcept { return p->first; }
		operator const std::filesystem::path&() const noexcept { return p->second; }
		const std::filesystem::path& operator*() const noexcept { return p->second; }
		const std::filesystem::path* operator->() const noexcept { return &p->second; }

	private:
		friend std::hash<Path>;
		explicit Path(const Pair* p) noexcept :p{p} {}
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
		return reinterpret_cast<size_t>(key.p);
	}
};

#define PATH(s) []{static ::oeng::Path n{::oeng::Name{s}};return n;}()
