#pragma once

#undef JSON_USE_IMPLICIT_CONVERSIONS
#define JSON_USE_IMPLICIT_CONVERSIONS 0

#include "File.hpp"
#include <nlohmann/json.hpp>

namespace oeng
{
namespace core
{
namespace fs = std::filesystem;
using Json = nlohmann::json;
using JsonType = nlohmann::detail::value_t;
using JsonParseError = nlohmann::detail::parse_error;

/**
 * Read json file.
 * @param filepath File path.
 * @return Json object.
 * @throw std::ifstream::failure If failed to open file.
 * @throw JsonParseError If failed to parse.
 */
[[nodiscard]] inline Json ReadJsonFile(const fs::path& filepath)
{
    return Json::parse(ReadFile<char>(filepath), nullptr, true, true);
}

/**
 * Write json file.
 * If there are no directories corresponding to the specified path, they are automatically created.
 * @param filepath File path.
 * @param json Json object.
 * @throw std::ofstream::failure If failed to open file.
 * @throw std::filesystem::filesystem_error If the path is invalid.
 */
inline void WriteJsonFile(const fs::path& filepath, const Json& json)
{
    WriteFile<char>(filepath) << json.dump(4);
}
}

using namespace core;
}
