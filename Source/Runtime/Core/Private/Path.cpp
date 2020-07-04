#include "Path.hpp"
#include "Templates/Sync.hpp"
#include "Json.hpp"
#include "Core.hpp"

namespace oeng
{
	static auto GetMap()
	{
		constexpr bool thread_safe = OE_PATH_THREADSAFE;
		assert(thread_safe || IsGameThread());
		
		using PathMap = std::unordered_map<Name, std::filesystem::path>;
		static CondMonitor<PathMap, thread_safe> map{Path::Pair{}};
		return map.Lock();
	}

	Path::Path() noexcept
	{
		static const Path default_path{&*GetMap()->find({})};
		p = default_path.p;
	}

	Path::Path(const std::filesystem::path& path)
	{
		auto normalized = proximate(path);
		const Name key = normalized.string();
		auto [it, has_inserted] = GetMap()->try_emplace(key, std::move(normalized));
		p = &*it;
	}

	void to_json(Json& json, const Path& path)
	{
		to_json(json, path.AsName());
	}

	void from_json(const Json& json, Path& path)
	{
		path = Path{json.get<std::string>()};
	}
}
