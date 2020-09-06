#pragma once
#include "Log.hpp"
#include "Name.hpp"
#include <filesystem>
#include <set>

namespace logcat
{
extern CORE_API const LogCategory kConfig;
}

namespace oeng
{
inline namespace core
{
namespace fs = std::filesystem;

class CORE_API ConfigSystem
{
public:
    [[nodiscard]] static ConfigSystem& Get();

    /**
     * Find config or create if not found.
     * @param name Config name
     * @return Reference to non-const json config object
     */
    [[nodiscard]] Json& operator()(Name name)
    {
        return configs_[name];
    }

    /**
     * Save config as file into the user data directory.
     * Existing files will be overwritten.
     * If a config with the given name cannot be found, it will fails silently.
     * If failed to serialize/write to a file, an error will be logged.
     * @param name Config name
     * @return true if successful
     */
    bool Save(Name name) const;

    /**
     * Load configurations from file.
     */
    void LoadConfig(const fs::path& filepath);

    /**
     * Load configuration files from given directory (not recursive).
     * @param directory Directory path containing configuration files
     * @param extensions Extension filter. Load only files with the specified extensions if not empty.
     */
    void LoadConfigs(const fs::path& directory, const std::set<fs::path>& extensions = {u8".json"sv, u8".jsonc"sv});

private:
    ConfigSystem();
    std::unordered_map<Name, Json> configs_;
};
}
}
