#include "Name.hpp"
#include <unordered_set>
#include <otm/Hash.hpp>
#include "Json.hpp"
#include "Templates/Monitor.hpp"
#include "Templates/Wrapper.hpp"

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

	using Set = std::unordered_set<std::string, NameHasher, NameEqual>;
	using StrSet = std::conditional_t<OE_NAME_THREADSAFE, Monitor<Set>, Wrapper<Set>>;
	
	static StrSet str_set{std::string{}};
	
	Name::Name() noexcept
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
