#pragma once

#include "NEG.h"
#include <sstream>

NEG_BEGIN

template <class... Ts>
std::string fmt(Ts&&... args)
{
	std::ostringstream os;
	(os << ... << std::forward<Ts>(args));
	return os.str();
}

NEG_END