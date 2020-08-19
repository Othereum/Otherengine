#pragma once
#include <filesystem>
#include <fstream>

namespace oeng
{
namespace fs = std::filesystem;
}

namespace oeng::core
{
/**
 * Open the file in read mode.
 * @param filepath File path.
 * @param extra_mode Extra open modes.
 * @throw std::ifstream::failure If failed to open file.
 * @return Opened input file stream.
 */
[[nodiscard]] inline std::ifstream ReadFile(const fs::path& filepath, std::ifstream::openmode extra_mode = {})
{
    std::ifstream file{filepath, std::ifstream::in | extra_mode};
    if (!file.is_open())
        throw std::ifstream::failure{
            fmt::format(FMT_COMPILE("Failed to open file '{}' for read"sv), filepath.string())};
    return file;
}

/**
 * Open the file in write mode.
 * If there are no directories corresponding to the specified path, they are automatically created.
 * @param filepath File path.
 * @param extra_mode Extra open modes.
 * @throw std::ofstream::failure If failed to open file.
 * @throw std::filesystem::filesystem_error If the path is invalid.
 * @return Opened output file stream.
 */
[[nodiscard]] inline std::ofstream WriteFile(const fs::path& filepath, std::ofstream::openmode extra_mode = {})
{
    create_directories(filepath.parent_path());
    std::ofstream file{filepath, std::ofstream::out | extra_mode};
    if (!file.is_open())
        throw std::ofstream::failure{
            fmt::format(FMT_COMPILE("Failed to open file '{}' for write"sv), filepath.string())};
    return file;
}
}
