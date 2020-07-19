#include "Config.hpp"
#include <filesystem>
#include <fstream>
#include "Json.hpp"
#include "Log.hpp"
#include "Platform.hpp"

namespace oeng::core
{
	namespace fs = std::filesystem;
	
	[[nodiscard]] static fs::path GetUserConfigDir()
	{
		return GetUserDataPath() /= u8"Config";
	}

	static void LoadConfig(TreeMap<Name, Json>& configs, const fs::path& file)
	{
		if (!is_regular_file(file) || file.extension() != u8".json") return;
		
		auto name = file.stem().string<char8_t>(PoolAllocator<char8_t>{});
		auto parsed = ReadFileAsJson(file);

		auto [it, inserted] = configs.try_emplace(std::move(name), std::move(parsed));
		if (inserted) return;
		
		for (auto& [key, value] : parsed.items())
		{
			it->second[key] = std::move(value);
		}
	}
	
	static void LoadConfigs(TreeMap<Name, Json>& configs, const fs::path& directory)
	{
		if (!exists(directory)) return;
		
		for (const auto& entry : fs::directory_iterator{directory})
		{
			try
			{
				LoadConfig(configs, entry);
			}
			catch (const std::exception& e)
			{
				log::Error(u8"Failed to load config '{}': {}", entry.path().string<char8_t>(PoolAllocator<char8_t>{}), What(e));
			}
		}
	}
	
	ConfigManager::ConfigManager()
	{
		LoadConfigs(configs_, u8"../Engine/Config"sv);
		LoadConfigs(configs_, u8"../Config"sv);
		LoadConfigs(configs_, GetUserConfigDir());
	}

	Json& ConfigManager::Config(Name name)
	{
		return configs_[name];
	}

	bool ConfigManager::SaveConfig(Name name)
	{
		try
		{
			auto dir = GetUserConfigDir();
			create_directories(dir);

			std::ofstream file{dir /= Format(u8"{}.json"sv, *name)};
			file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
			file << configs_[name].dump(4);
			
			return true;
		}
		catch (const std::exception& e)
		{
			log::Error(u8"Failed to save config '{}': {}"sv, *name, What(e));
			return false;
		}
	}
}
