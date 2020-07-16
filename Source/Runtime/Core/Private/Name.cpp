#include "Name.hpp"
#include <unordered_set>
#include <otm/Hash.hpp>
#include "Json.hpp"
#include "Core.hpp"
#include "Templates/Sync.hpp"

namespace oeng::core
{
	struct NameHasher
	{
		[[nodiscard]] constexpr size_t operator()(std::u8string_view s) const noexcept
		{
			return HashRange(s.begin(), s.end(), tolower);
		}
	};

	struct NameEqual
	{
		[[nodiscard]] constexpr bool operator()(std::u8string_view s1, std::u8string_view s2) const noexcept
		{
			if (s1.size() != s2.size()) return false;
			for (size_t i = 0; i < s1.size(); ++i)
				if (tolower(s1[i]) != tolower(s2[i]))
					return false;
			return true;
		}
	};

	static auto GetSet()
	{
#ifdef OE_NAME_THREADSAFE
		constexpr auto thread_safe = true;
#else
		constexpr auto thread_safe = false;
		assert(IsGameThread());
#endif
		
		using NameSet = std::unordered_set<std::u8string, NameHasher, NameEqual>;
		static CondMonitor<NameSet, thread_safe> set{std::u8string{}};
		return set.Lock();
	}
	
	Name::Name() noexcept
	{
		static const auto def = &*GetSet()->find({});
		sp = def;
	}

	Name::Name(const std::u8string& s)
		:sp{&*GetSet()->insert(s).first}
	{
	}

	Name::Name(std::u8string&& s)
		:sp{&*GetSet()->insert(std::move(s)).first}
	{
	}

	Name::Name(const char8_t* s)
	{
		auto set = GetSet();
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
		json = *name;
	}

	void from_json(const Json& json, Name& name)
	{
		auto str = json.get<std::string>();
		name = Name{AsString8(std::move(str))};
	}
}
