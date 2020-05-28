#include "Path.hpp"
#include <unordered_set>
#include <otm/Hash.hpp>
#include "Templates/Monitor.hpp"
#include "Templates/Wrapper.hpp"
#include "Json.hpp"

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

	using Set = std::unordered_set<std::filesystem::path, PathHasher, PathEqual>;
	using PathSet = std::conditional_t<OE_PATH_THREADSAFE, Monitor<Set>, Wrapper<Set>>;
	
	static PathSet path_set{std::filesystem::path{}};

	Path::Path() noexcept
		:p{&*path_set->find({})}
	{
	}

	Path::Path(const char* path)
		:Path{std::filesystem::path{path}}
	{
	}

	Path::Path(const std::filesystem::path& path)
	{
		auto [it, has_inserted] = path_set->insert(proximate(path));
		p = &*it;
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
