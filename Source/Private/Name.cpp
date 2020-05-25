#include "Name.hpp"
#include <unordered_set>
#include "Json.hpp"
#include "Templates/Monitor.hpp"
#include "Templates/Wrapper.hpp"

namespace oeng
{
	struct NameHasher
	{
		[[nodiscard]] constexpr size_t operator()(std::string_view s) const noexcept
		{
			constexpr auto offset_basis = []{ if constexpr(sizeof size_t >= 8) { return 14695981039346656037ULL; } else { return 2166136261U; } }();
			constexpr auto prime = []{ if constexpr(sizeof size_t >= 8) { return 1099511628211ULL; } else { return 16777619U; } }();

			size_t val = offset_basis;
		    for (auto c : s)
			{
		        val ^= tolower(c);
		        val *= prime;
		    }

		    return val;
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

	using Set = std::unordered_set<std::string, NameHasher, NameEqual>;
	using StrSet = std::conditional_t<OENG_NAME_THREADSAFE, Monitor<Set>, Wrapper<Set>>;
	
	static StrSet str_set{std::string{}};
	
	Name::Name()
		:sp{&*str_set->find({})}
	{
	}

	Name::Name(const char* s)
		:Name{std::string{s}}
	{
	}

	Name::Name(const std::string& s)
	{
		auto [it, inserted] = str_set->insert(s);
		sp = &*it;
	}

	Name::Name(std::string&& s)
	{
		auto [it, inserted] = str_set->insert(std::move(s));
		sp = &*it;
	}

	void to_json(Json& json, const Name& name)
	{
		json = name.Str();
	}

	void from_json(const Json& json, Name& name)
	{
		name = json.get<std::string>();
	}
}
