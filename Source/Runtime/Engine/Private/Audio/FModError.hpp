#pragma once
#include "fmod.hpp"

namespace oeng
{
inline namespace engine
{
std::u8string_view FModErrorString(FMOD_RESULT result);

inline void FModCheck(FMOD_RESULT result)
{
    if (result != FMOD_OK)
        throw std::runtime_error{AsString(FModErrorString(result)).data()};
}

template <class Str, class... Args>
void FModCheck(FMOD_RESULT result, const Str& message, const Args&... args)
{
    if (result != FMOD_OK)
        throw std::runtime_error{
            AsString(fmt::format(u8"{}: {} ({})"sv, fmt::format(message, args...), FModErrorString(result), result))};
}
}
