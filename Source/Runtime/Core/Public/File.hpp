#pragma once
#include <filesystem>
#include <fstream>

namespace oeng
{
inline namespace core
{
namespace fs = std::filesystem;

/**
 * Open the file in read mode.
 * @param filepath File path.
 * @param extra_mode Extra open modes.
 * @throw std::ios::failure If failed to open file.
 * @return Opened input file stream.
 */
template <class Char = char8_t>
[[nodiscard]] std::basic_ifstream<Char> ReadFile(const fs::path& filepath, std::ios::openmode extra_mode = {})
{
    std::basic_ifstream<Char> file{filepath, std::ios::in | extra_mode};
    if (!file.is_open())
        throw std::ios::failure{
            fmt::format(FMT_COMPILE("Failed to open file '{}' for read"sv), filepath.string())};
    return file;
}

/**
 * Open the file in write mode.
 * If there are no directories corresponding to the specified path, they are automatically created.
 * @param filepath File path.
 * @param extra_mode Extra open modes.
 * @throw std::ios::failure If failed to open file.
 * @throw fs::filesystem_error If the path is invalid.
 * @return Opened output file stream.
 */
template <class Char = char8_t>
[[nodiscard]] std::basic_ofstream<Char> WriteFile(const fs::path& filepath, std::ios::openmode extra_mode = {})
{
    create_directories(filepath.parent_path());
    std::basic_ofstream<Char> file{filepath, std::ios::out | extra_mode};
    if (!file.is_open())
        throw std::ios::failure{
            fmt::format(FMT_COMPILE("Failed to open file '{}' for write"sv), filepath.string())};
    return file;
}

/**
 * Read the whole file as string.
 * @param filepath File path.
 * @throw std::ios::failure If failed to open file.
 * @return Read string.
 */
template <class Char = char8_t>
[[nodiscard]] std::basic_string<Char> ReadFileAsString(const fs::path& filepath)
{
    auto file = ReadFile(filepath, std::ios::ate);
    std::basic_string<Char> str;
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    str.assign(std::istreambuf_iterator<Char>{file}, {});
    return str;
}
}
}
