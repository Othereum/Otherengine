#pragma once
#include "ConfigSystem.hpp"
#include "Log.hpp"
#include "Path.hpp"
#include "Platform.hpp"
#include "Stat.hpp"

namespace logcat
{
	extern CORE_API const LogCategory kEngine;
}

namespace oeng::engine
{
	class Engine;
}

namespace oeng::core
{
	class EngineBase;
	
	class CORE_API RegisterEngineBase
	{
		friend EngineBase;
		DELETE_CPMV(RegisterEngineBase);
		explicit RegisterEngineBase(EngineBase* engine);
		~RegisterEngineBase();
	};
	
	class CORE_API CoreSystem
	{
	public:
		DELETE_CPMV(CoreSystem);
		
		[[nodiscard]] Logger& GetLogger() noexcept { return logger_; }
		[[nodiscard]] const Dll& GetGameDll() const noexcept { return game_dll_; }
		[[nodiscard]] std::u8string_view GetGameName() const noexcept { return game_name_; }
		
	private:
		friend EngineBase;
		explicit CoreSystem(std::u8string game_module_path);
		~CoreSystem();
		
		Dll game_dll_;
		std::u8string_view game_name_;
		Logger logger_;
	};
	
	class CORE_API EngineBase : RegisterEngineBase, public CoreSystem
	{
	public:
		DELETE_CPMV(EngineBase);

		[[nodiscard]] static EngineBase& Get() noexcept;
		[[nodiscard]] static bool Exists() noexcept;
		
		[[nodiscard]] ConfigSystem& GetConfig() noexcept { return config_; }
		[[nodiscard]] uint64_t GetTickCount() const noexcept { return ticks_; }
		
	protected:
		uint64_t ticks_;

	private:
		friend Name;
		friend Path;
		friend engine::Engine;
		friend CounterManager;
		friend ScopeCounter;
		friend ScopeStackCounter;

		explicit EngineBase(std::u8string game_module_path);
		~EngineBase() = default;

		NameSet names_;
		PathSet paths_;
		ConfigSystem config_;
		CounterManager counters_;
	};
}
