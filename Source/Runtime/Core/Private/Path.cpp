#include "Path.hpp"
#include "EngineBase.hpp"

namespace oeng::core
{
	PathSet& Path::Set() noexcept
	{
		return EngineBase::Get().paths_;
	}

	void to_json(Json& json, const Path& path)
	{
		json = AsString(path.Str());
	}

	void from_json(const Json& json, Path& path)
	{
		path = AsString8(json.get<String>());
	}
}
