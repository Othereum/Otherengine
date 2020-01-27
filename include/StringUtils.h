#pragma once

#include "NEG.h"
#include <sstream>

NEG_BEGIN

template <class... Ts>
std::string join(Ts&&... args)
{
	std::string str;
	(str += ... += std::forward<Ts>(args));
	return str;
}

NEG_END