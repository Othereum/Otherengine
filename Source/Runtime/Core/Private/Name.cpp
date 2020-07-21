#include "Name.hpp"
#include "EngineBase.hpp"

namespace oeng::core
{
	NameSet& Name::Set() noexcept
	{
		return EngineBase::Get().names_;
	}

	void to_json(Json& json, const Name& name)
	{
		json = AsString(*name);
	}

	void from_json(const Json& json, Name& name)
	{
		name = AsString8(json.get<String>());
	}
}
