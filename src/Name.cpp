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
		auto found = gSet.find(s);
		if (found == gSet.end()) found = gSet.emplace(s).first;
		s_ = &*found;
	}

	FName::FName(std::string&& s)
	{
		auto found = gSet.find(s);
		if (found == gSet.end()) found = gSet.emplace(std::move(s)).first;
		s_ = &*found;
	}
}
