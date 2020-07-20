#pragma once
#include <filesystem>
#include "Name.hpp"

namespace oeng::core
{
	namespace fs = std::filesystem;
	
#ifdef OE_PATH_THREADSAFE
	constexpr auto kPathThreadSafe = true;
#else
	constexpr auto kPathThreadSafe = false;
#endif
	
	struct PathHasher
	{
		[[nodiscard]] size_t operator()(const fs::path& p) const noexcept
		{
			auto& s = p.native();
			return HashRange(s.begin(), s.end(), tolower);
		}
	};

	struct PathEqual
	{
		[[nodiscard]] bool operator()(const fs::path& a, const fs::path& b) const noexcept
		{
			return StrEqCi(a.native(), b.native());
		}
	};
	
	using PathSet = CondMonitor<HashSet<fs::path, PathHasher, PathEqual>, kPathThreadSafe>;
	
	/**
	 * Lightweight representation of path.
	 * Very fast O(1) copy and comparison.
	 * No heap allocation on copy.
	 * Good to use as key for Map/Set.
	 * @note Case-insensitive on any platform
	 * @note Comparisons are not lexical.
	 */
	struct CORE_API Path
	{
		Path() noexcept :p{&*Set()->find({})} {}
		Path(std::u8string_view s) :Path{fs::path{s}} {}
		Path(fs::path::string_type&& s) :Path{fs::path{std::move(s)}} {}
		Path(const fs::path& path) :p{&*Set()->insert(proximate(path)).first} {}

		template <class Al>
		Path(const std::basic_string<char8_t, std::char_traits<char8_t>, Al>& s)
			:Path{fs::path{s}}
		{
		}

		bool operator==(const Path& r) const noexcept { return p == r.p; }
		bool operator!=(const Path& r) const noexcept { return p != r.p; }
		bool operator<(const Path& r) const noexcept { return p < r.p; }
		bool operator>(const Path& r) const noexcept { return p > r.p; }
		bool operator<=(const Path& r) const noexcept { return p <= r.p; }
		bool operator>=(const Path& r) const noexcept { return p >= r.p; }

		template <class T = char8_t, class Al = PoolAllocator<char8_t>>
		[[nodiscard]] auto Str() const { return p->string<T>(Al{}); }
		
		operator String8() const { return Str(); }
		operator const fs::path&() const noexcept { return *p; }
		const fs::path& operator*() const noexcept { return *p; }
		const fs::path* operator->() const noexcept { return p; }

	private:
		[[nodiscard]] static PathSet& Set() noexcept;
		const fs::path* p;
	};
	
	CORE_API void to_json(Json& json, const Path& path);
	CORE_API void from_json(const Json& json, Path& path);
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
struct std::hash<oeng::Path>
{
	size_t operator()(const oeng::Path& key) const noexcept
	{
		return size_t(&*key);
	}
};
#endif
