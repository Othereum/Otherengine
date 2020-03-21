#include "Name.h"

namespace Game
{
	static std::unordered_set<std::string> gSet{{}};
	
	FName::FName()
		:s_{&*gSet.find({})}
	{
	}

	FName::FName(const char* s)
		:FName{std::string{s}}
	{
	}

	FName::FName(const std::string& s)
	{
		// It's best way until C++17
		// find(string_view) is way better since C++20
		auto [it, bInserted] = gSet.insert(s);
		s_ = &*it;
	}

	FName::FName(std::string&& s)
	{
		auto [it, bInserted] = gSet.insert(std::move(s));
		s_ = &*it;
	}
}
