#include "Path.hpp"
#include "EngineBase.hpp"
#include "Json.hpp"

#define PATH_SET (assert(kEngineBase), kEngineBase->paths_)

namespace oeng::core
{
	Path::Path() noexcept
		:p{&*PATH_SET->find({})}
	{
	}

	Path::Path(const fs::path& path)
		:p{&*PATH_SET->insert(proximate(path)).first}
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
