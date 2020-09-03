#pragma once
#include <filesystem>

namespace oeng
{
inline namespace core
{
/**
 * Lightweight representation of path.
 * Very fast O(1) copy and comparison.
 * No heap allocation on copy.
 * Good to use as key for map/set.
 * @note Case-insensitive on any platform
 * @note Comparisons are not lexical.
 */
struct CORE_API Path
{
    Path() noexcept;
    Path(const fs::path& path);

    Path(std::u8string_view s)
        : Path{fs::path{s}}
    {
    }

    template <class Al>
    Path(const std::basic_string<char8_t, std::char_traits<char8_t>, Al>& s)
        : Path{fs::path{s}}
    {
    }

    bool operator==(const Path& r) const noexcept
    {
        return p_ == r.p_;
    }

    auto operator<=>(const Path& r) const noexcept
    {
        return p_ <=> r.p_;
    }

    template <class T = char8_t>
    [[nodiscard]] auto Str() const
    {
        return p_->string<T>();
    }

    operator const fs::path&() const noexcept
    {
        return *p_;
    }

    const fs::path& operator*() const noexcept
    {
        return *p_;
    }

    const fs::path* operator->() const noexcept
    {
        return p_;
    }

private:
    const fs::path* p_;
};

CORE_API void to_json(Json& json, const Path& path);
CORE_API void from_json(const Json& json, Path& path);
}
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
