#pragma once
#include <filesystem>
#include "Name.hpp"

namespace oeng::core
{
	namespace fs = std::filesystem;
	
	/**
	 * \brief Lightweight representation of filepath.
	 * Very fast O(1) copy and comparison.
	 * No heap allocation on copy.
	 * Good to use as key for Map/Set.
	 * \note Case-insensitive on any platform
	 * \note Comparisons are not lexical.
	 */
	struct CORE_API Path
	{
		Path() noexcept;
		Path(const fs::path& path);
		Path(std::u8string_view s) :Path{fs::path{s}} {}
		Path(fs::path::string_type&& s) :Path{fs::path{std::move(s)}} {}

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

		[[nodiscard]] String8 Str() const { return p->string<char8_t>(PoolAllocator<char8_t>{}); }
		operator String8() const { return Str(); }

		operator const fs::path&() const noexcept { return *p; }
		const fs::path& operator*() const noexcept { return *p; }
		const fs::path* operator->() const noexcept { return p; }

	private:
		const fs::path* p;
	};
	
	struct PathHasher
	{
		[[nodiscard]] constexpr size_t operator()(const fs::path& p) const noexcept
		{
			auto& s = p.native();
			return HashRange(s.begin(), s.end(), tolower);
		}
	};

	struct PathEqual
	{
		[[nodiscard]] constexpr bool operator()(const fs::path& a, const fs::path& b) const noexcept
		{
			return StrEqCi(a.native(), b.native());
		}
	};
	
#ifdef OE_PATH_THREADSAFE
	constexpr auto kPathThreadSafe = true;
#else
	constexpr auto kPathThreadSafe = false;
#endif
	
	using PathSet = CondMonitor<HashSet<fs::path, PathHasher, PathEqual>, kPathThreadSafe>;
	
	CORE_API void to_json(Json& json, const Path& path);
	CORE_API void from_json(const Json& json, Path& path);
}

template <>
struct std::hash<oeng::Path>
{
	size_t operator()(const oeng::Path& key) const noexcept
	{
		return size_t(&*key);
	}
};
