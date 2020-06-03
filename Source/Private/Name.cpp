#include "Name.hpp"
#include <otm/Hash.hpp>
#include "Assert.hpp"
#include "Json.hpp"
#include "Thread.hpp"
#include "Templates/HashSet.hpp"
#include "Templates/Monitor.hpp"

namespace oeng
{
	struct NameHasher
	{
		[[nodiscard]] constexpr size_t operator()(std::string_view s) const noexcept
		{
			return otm::HashRange(s.begin(), s.end(), tolower);
		}
	};

	struct NameEqual
	{
		[[nodiscard]] constexpr bool operator()(std::string_view s1, std::string_view s2) const noexcept
		{
			if (s1.size() != s2.size()) return false;
			for (size_t i = 0; i < s1.size(); ++i)
				if (tolower(s1[i]) != tolower(s2[i]))
					return false;
			return true;
		}
	};

	using NameSet = HashSet<NameString, NameHasher, NameEqual, std::allocator<NameString>>;
	static Monitor<NameSet, CondMutex<OE_NAME_THREADSAFE>> str_set{NameString{}};
	
	Name::Name() noexcept
		:sp{&*str_set->find({})}
	{
	}

	Name::Name(const char* s)
		:Name{NameString{s}}
	{
	}

	Name::Name(NameString s)
	{
#if !OE_NAME_THREADSAFE
		CHECK(IsGameThread());
#endif
		
		auto [it, inserted] = str_set->insert(std::move(s));
		sp = &*it;
	}

	void to_json(Json& json, const Name& name)
	{
		json = name.Str();
	}

	void from_json(const Json& json, Name& name)
	{
		name = json.get<NameString>();
	}
}
