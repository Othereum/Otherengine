#pragma once

#include <stdexcept>
#include <NEG.h>

NEG_BEGIN

class sdl_error : public std::runtime_error
{
public:
    explicit sdl_error(const char* what_arg);
    explicit sdl_error(const std::string& what_arg);
};

NEG_END
