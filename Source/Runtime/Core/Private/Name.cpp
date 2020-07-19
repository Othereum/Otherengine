#include "Name.hpp"
#include "EngineBase.hpp"
#include "Json.hpp"

#define NAME_SET (assert(kEngineBase), kEngineBase->names_)

namespace oeng::core
{
	Name::Name() noexcept
		:sp{&*NAME_SET->find(std::u8string_view{})}
	{
	}

	Name::Name(const String8& s)
		:sp{&*NAME_SET->insert(s).first}
	{
	}

	Name::Name(String8&& s)
		:sp{&*NAME_SET->insert(std::move(s)).first}
	{
	}

	Name::Name(std::u8string_view s)
	{
		auto set = NAME_SET.Lock();
		const auto found = set->find(s);
		if (found != set->end())
		{
			sp = &*found;
		}
		else
		{
			auto [it, has_inserted] = set->emplace(s);
			assert(has_inserted);
			sp = &*it;
		}
	}

	void to_json(Json& json, const Name& name)
	{
		json = AsString(*name);
	}

	void from_json(const Json& json, Name& name)
	{
		name = Name{String8{AsString8(json.get<std::string>())}};
	}
}
