#include "Path.hpp"
#include "Assert.hpp"
#include "Templates/Sync.hpp"
#include "Templates/HashMap.hpp"
#include "Json.hpp"
#include "Core.hpp"

namespace oeng
{
	static auto& GetMap()
	{
		CHECK(OE_PATH_THREADSAFE || IsGameThread());
		using PathMap = HashMap<Name, std::filesystem::path, std::hash<Name>, std::equal_to<>, RawAllocator<Path::Pair>>;
		static Monitor<PathMap, CondMutex<OE_PATH_THREADSAFE>> map{Path::Pair{}};
		return map;
	};

	Path::Path() noexcept
	{
		static const Path default_path{&*GetMap()->find({})};
		p = default_path.p;
	}

	Path::Path(Name path)
		:p{&*GetMap()->try_emplace(path, *path).first}
	{
	}

	Path::Path(std::filesystem::path&& path)
		:p{&*GetMap()->try_emplace(Name{path.string()}, std::move(path)).first}
	{
	}

	Path::Path(const std::filesystem::path& path)
		:p{&*GetMap()->try_emplace(Name{path.string()}, path).first}
	{
	}

	void to_json(Json& json, const Path& path)
	{
		to_json(json, path.AsName());
	}

	void from_json(const Json& json, Path& path)
	{
		path = Path{json.get<Name>()};
	}
}
