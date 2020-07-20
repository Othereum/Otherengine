#include "Name.hpp"
#include "EngineBase.hpp"
#include "Json.hpp"

#define NAME_SET (assert(kEngineBase), kEngineBase->names_)

namespace oeng::core
{
	Name::Name() noexcept
		:sp{&*NAME_SET->find(String8{})}
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
	
	template <class Container, class KeyEquivalent>
	struct IsTransparent
	{
	private:
	    template <class C, class = decltype(std::declval<C>().find(std::declval<KeyEquivalent>()))>
	    static std::true_type Test(int);
		
	    template <class C>
	    static std::false_type Test(...);

	public:
	    static constexpr bool value = decltype(Test<Container>(0))::value;
	};

	template <class Set>
	auto Find(const Set& set, std::u8string_view s)
	{
		if constexpr (IsTransparent<Set, std::u8string_view>::value)
		{
			return set.find(s);
		}
		else
		{
			return set.find(s.data());
		}
	}

	Name::Name(std::u8string_view s)
	{
		auto set = NAME_SET.Lock();
		const auto found = Find(*set, s);
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
