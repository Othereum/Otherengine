#include "Name.hpp"
#include "EngineBase.hpp"

CORE_BEGIN

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

CORE_END
