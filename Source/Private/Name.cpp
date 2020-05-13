#include "Name.hpp"
#include <unordered_set>
#include "Json.hpp"

namespace oeng
{
	static std::unordered_set<std::string> str_set{{}};
	
	Name::Name()
		:sp{&*str_set.find({})}
	{
	}

	Name::Name(const char* s)
		:Name{std::string{s}}
	{
	}

	Name::Name(const std::string& s)
	{
		// It's best way until C++17
		// find(string_view) is way better since C++20
		auto [it, bInserted] = str_set.insert(s);
		sp = &*it;
	}

	Name::Name(std::string&& s)
	{
		auto [it, bInserted] = str_set.insert(std::move(s));
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
