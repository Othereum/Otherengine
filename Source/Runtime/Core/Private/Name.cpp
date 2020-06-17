#include "Name.hpp"
#include <unordered_set>
#include <otm/Hash.hpp>
#include "Assert.hpp"
#include "Json.hpp"
#include "Core.hpp"
#include "Templates/Sync.hpp"

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
		constexpr bool thread_safe = OE_NAME_THREADSAFE;
		CHECK(thread_safe || IsGameThread());
		
		using NameSet = std::unordered_set<Name::Str, NameHasher, NameEqual>;
		static CondMonitor<NameSet, thread_safe> set{Name::Str{}};
		return set;
	}
	
	Name::Name() noexcept
	{
		static const Name default_name{&*GetSet()->find({})};
		sp = default_name.sp;
	}

	Name::Name(Str&& s)
		:sp{&*GetSet()->insert(std::move(s)).first}
	{
	}

	Name::Name(const Str& s)
		:sp{&*GetSet()->insert(s).first}
	{
	}

	void to_json(Json& json, const Name& name)
	{
		json = *name;
	}

	void from_json(const Json& json, Name& name)
	{
		name = Name{json.get<Name::Str>()};
	}
}
