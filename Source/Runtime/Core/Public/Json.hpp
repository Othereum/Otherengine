#pragma once

#undef JSON_USE_IMPLICIT_CONVERSIONS
#define JSON_USE_IMPLICIT_CONVERSIONS 0

#include <nlohmann/json.hpp>

namespace oeng
{
namespace core
{
using Json = nlohmann::json;
using JsonType = nlohmann::detail::value_t;
}

using namespace core;
}
