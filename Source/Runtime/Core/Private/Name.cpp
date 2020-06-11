#include "Name.hpp"
#include <otm/Hash.hpp>
#include "Assert.hpp"
#include "Json.hpp"
#include "Core.hpp"
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

	static auto& GetSet()
	{
		CHECK(OE_NAME_THREADSAFE || IsGameThread());
		using NameSet = HashSet<NameStr, NameHasher, NameEqual, RawAllocator<NameStr>>;
		static Monitor<NameSet, CondMutex<OE_NAME_THREADSAFE>> set{NameStr{}};
		return set;
	}
	
	Name::Name() noexcept
		:sp{&*GetSet()->find({})}
	{
	}

	Name::Name(const char* s)
		:Name{NameStr{s}}
	{
	}

	Name::Name(NameStr s)
		:sp{&*GetSet()->insert(std::move(s)).first}
	{
	}

	void to_json(Json& json, const Name& name)
	{
		json = *name;
	}

	void from_json(const Json& json, Name& name)
	{
		name = json.get<NameStr>();
	}
}
