#include "Path.hpp"
#include <unordered_set>

namespace oeng
{
inline namespace core
{
#ifdef OE_PATH_THREADSAFE
constexpr auto kPathThreadSafe = true;
#else
constexpr auto kPathThreadSafe = false;
#endif

struct PathHasher
{
    [[nodiscard]] size_t operator()(const fs::path& p) const noexcept
    {
        const auto& s = p.native();
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

using PathSet = CondMonitor<std::unordered_set<fs::path, PathHasher, PathEqual>, kPathThreadSafe>;

[[nodiscard]] PathSet& Set()
{
    if constexpr (!kPathThreadSafe)
    {
        assert(IsGameThread());
    }

    static PathSet set;
    return set;
}

Path::Path() noexcept
    : p_{&*Set()->find({})}
{
}

Path::Path(const fs::path& path)
    : p_{&*Set()->emplace(proximate(path)).first}
{
}

void to_json(Json& json, const Path& path)
{
    json = AsString(path.Str());
}

void from_json(const Json& json, Path& path)
{
    path = AsString8(json.get<std::string>());
}
}
}
