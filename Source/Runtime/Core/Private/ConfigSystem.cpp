#include "ConfigSystem.hpp"
#include <filesystem>
#include <fstream>
#include "EngineBase.hpp"
#include "Log.hpp"
#include "Platform.hpp"

namespace oeng::core
{
	namespace fs = std::filesystem;
	
	[[nodiscard]] static fs::path GetUserConfigDir()
	{
		return GetUserDataPath() /= u8"Config"sv;
	}

	class ConfigLoader
	{
	public:
		explicit ConfigLoader(Path path) :path_{path} {}
		
		void Load(Json& config, Json&& parsed)
		{
			// Remove properties first
			for (auto& [key, value] : parsed.items())
			{
				if (key[0] == '-')
					RemoveProperties(config, key, value);
			}

			// And then add/override
			for (auto& [key, value] : parsed.items())
			{
				if (key[0] == '-') continue;
				if (key[0] == '+')
				{
					AddProperties(config, key, std::move(value));
				}
				else
				{
					config[key] = std::move(value);
				}
			}
		}
		
	private:
		class Pusher
		{
		public:
			Pusher(DyArr<std::u8string_view>& stack, std::u8string_view name)
				:stack_{&stack}
			{
				stack_->emplace_back(name);
			}

			~Pusher() { if (stack_) stack_->pop_back(); }
			Pusher(Pusher&& r) noexcept :stack_{r.stack_} { r.stack_ = nullptr; }
			Pusher(const Pusher&) = delete;
			
			Pusher& operator=(const Pusher&) = delete;
			Pusher& operator=(Pusher&& r) noexcept
			{
				auto p = std::move(r);
				std::swap(stack_, p.stack_);
				return *this;
			}
			
		private:
			DyArr<std::u8string_view>* stack_;
		};

		void AddProperties(Json& config, const std::string& key, Json&& properties)
		{
			auto& out = config[key.substr(1)];
			auto stack = Push(key);
			
			switch (properties.type())
			{
			case JsonType::object:
				Load(out, std::move(properties));
				break;
				
			case JsonType::array:
				for (auto& elem : properties)
					out.emplace_back(std::move(elem));
				break;
				
			default:
				Log(log::Level::kErr, u8"The type of the property with add flag '+' must be object or array. (actual: {})"sv,
					reinterpret_cast<const char8_t*>(properties.type_name()));
			}
		}
		
		void RemoveProperties(Json& config, const std::string& key, Json& keys)
		{
			auto stack = Push(key);
			const auto real_key = key.substr(1);
			const auto found = config.find(real_key);
			if (found != config.end())
			{
				RemoveFrom(found.value(), keys);
			}
			else
			{
				Log(log::Level::kWarn, u8"Could not find '{}'"sv, AsString8(real_key));
			}
		}

		void RemoveFrom(Json& from, Json& keys) const
		{
			switch (from.type())
			{
			case JsonType::object:
				RemoveFromObject(from, keys);
				break;
				
			case JsonType::array:
				RemoveFromArray(from, keys);
				break;
				
			default:
				Log(log::Level::kErr, u8"Property removal can only be done on object or array. (actual: {})"sv,
					reinterpret_cast<const char8_t*>(from.type_name()));
			}
		}

		void RemoveFromObject(Json& from, const Json& keys) const
		{
			for (auto& k : keys) try
			{
				const auto ks = k.get<std::string>();
				if (!from.erase(ks))
					Log(log::Level::kWarn, u8"Could not remove '{}' (not found)"sv, AsString8(ks));
			}
			catch (const std::exception& e)
			{
				Log(log::Level::kErr, What(e));
			}
		}

		void RemoveFromArray(Json& from, Json& keys) const
		{
			// Erasing must start from back to keep indices valid
			std::sort(keys.begin(), keys.end(), std::greater<size_t>{});
			
			for (auto& idx : keys) try
			{
				const auto i = idx.get<size_t>();
				if (i < from.size()) from.erase(i);
				else Log(log::Level::kWarn, u8"Could not remove [{}] (out of range)"sv, i);
			}
			catch (const std::exception& e)
			{
				Log(log::Level::kErr, What(e));
			}
		}

		[[nodiscard]] Pusher Push(std::string_view name)
		{
			return {stack_, AsString8(name)};
		}

		[[nodiscard]] String8 Prop() const
		{
			String8 prop;
			for (const auto p : stack_)
			{
				prop += u8'[';
				prop += p;
				prop += u8']';
			}
			return prop;	
		}

		template <class... Args>
		void Log(log::Level level, std::u8string_view msg, const Args&... args) const noexcept
		{
			ASSERT_TRY(log::Log(level, u8"While loading \"{}\"{}: {}",
				path_.Str(), Prop(), Format(msg, args...)));
		}
		
		Path path_;
		DyArr<std::u8string_view> stack_;
	};

	static void LoadConfig(HashMap<Name, Json>& configs, const fs::path& file)
	{
		if (!is_regular_file(file) || file.extension() != u8".json"sv) return;

		const Path path = file;
		auto name = file.stem().string<char8_t>(PoolAllocator<char8_t>{});
		
		ConfigLoader loader{path};
		loader.Load(configs[std::move(name)], ReadFileAsJson(path));
	}
	
	static void LoadConfigs(HashMap<Name, Json>& configs, const fs::path& directory)
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
				const auto path = entry.path().string<char8_t>(PoolAllocator<char8_t>{});
				log::Error(u8"Failed to load config '{}': {}"sv, path, What(e));
			}
		}
	}
	
	ConfigSystem::ConfigSystem()
	{
		LoadConfigs(configs_, u8"../Engine/Config"sv);
		LoadConfigs(configs_, u8"../Config"sv);
		LoadConfigs(configs_, GetUserConfigDir());
	}

	ConfigSystem& ConfigSystem::Get() noexcept
	{
		return EngineBase::Get().GetConfig();
	}

	bool ConfigSystem::Save(Name name) const
	{
		const auto cfg = configs_.find(name);
		if (cfg == configs_.end()) return false;
		
		try
		{
			auto dir = GetUserConfigDir();
			create_directories(dir);

			std::ofstream file{dir /= Format(u8"{}.json"sv, *name)};
			file.exceptions(std::ios_base::failbit | std::ios_base::badbit);
			file << cfg->second.dump(4);
			
			return true;
		}
		catch (const std::exception& e)
		{
			log::Error(u8"Failed to save config '{}': {}"sv, *name, What(e));
			return false;
		}
	}
}
