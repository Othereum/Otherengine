#include "Path.hpp"
#include <otm/Hash.hpp>
#include "Assert.hpp"
#include "Templates/Monitor.hpp"
#include "Templates/HashSet.hpp"
#include "Json.hpp"
#include "Core.hpp"

namespace oeng
{
	struct PathHasher
	{
		[[nodiscard]] size_t operator()(const std::filesystem::path& path) const noexcept
		{
			auto&& s = path.native();
			return otm::HashRange(s.begin(), s.end(), tolower);
		}
	};

	struct PathEqual
	{
		[[nodiscard]] bool operator()(const std::filesystem::path& p1, const std::filesystem::path& p2) const
		{
			auto &&s1 = p1.native(), &&s2 = p2.native();
			return std::equal(s1.begin(), s1.end(), s2.begin(), s2.end(), [](auto c1, auto c2)
			{
				return tolower(c1) == tolower(c2);
			});
		}
	};

	static auto& GetSet()
	{
		CHECK(OE_PATH_THREADSAFE || IsGameThread());
		using PathSet = HashSet<std::filesystem::path, PathHasher, PathEqual, RawAllocator<std::filesystem::path>>;
		static Monitor<PathSet, CondMutex<OE_PATH_THREADSAFE>> set{std::filesystem::path{}};
		return set;
	};

	Path::Path() noexcept
		:p{&*GetSet()->find({})}
	{
	}

	Path::Path(const char* path)
		:Path{std::filesystem::path{path}}
	{
	}

	Path::Path(const std::filesystem::path& path)
		:p{&*GetSet()->insert(proximate(path)).first}
	{
	}

	void to_json(Json& json, const Path& path)
	{
		json = path.Get().string();
	}

	void from_json(const Json& json, Path& path)
	{
		path = {json.get<std::string>()};
	}
}
