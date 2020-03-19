#include "Name.h"

namespace Game
{
	std::unordered_set<std::string> FName::set_{{}};
	
	FName::FName()
		:s_{&*set_.find({})}
	{
	}

	FName::FName(const std::string& s)
	{
		auto found = set_.find(s);
		if (found != set_.end()) found = set_.emplace(s).first;
		s_ = &*found;
	}

	FName::FName(std::string&& s)
	{
		auto found = set_.find(s);
		if (found != set_.end()) found = set_.emplace(std::move(s)).first;
		s_ = &*found;
	}
}
