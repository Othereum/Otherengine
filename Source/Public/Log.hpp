#pragma once
#include <spdlog/logger.h>
#include "API.hpp"

namespace oeng::log
{
	namespace level = spdlog::level;
	
	OEAPI spdlog::logger& GetLogger();

	template <class... Args>
	void Log(level::level_enum level, std::string_view fmt, const Args&... args)
	{
		GetLogger().log(level, fmt, args...);
	}
	
	template <class... Args>
	void Trace(std::string_view fmt, const Args&... args)
	{
		GetLogger().trace(fmt, args...);
	}

	template <class... Args>
	void Debug(std::string_view fmt, const Args&... args)
	{
		GetLogger().debug(fmt, args...);
	}

	template <class... Args>
	void Info(std::string_view fmt, const Args&... args)
	{
		GetLogger().info(fmt, args...);
	}

	template <class... Args>
	void Warn(std::string_view fmt, const Args&... args)
	{
		GetLogger().warn(fmt, args...);
	}

	template <class... Args>
	void Error(std::string_view fmt, const Args&... args)
	{
		GetLogger().error(fmt, args...);
	}

	template <class... Args>
	void Critical(std::string_view fmt, const Args&... args)
	{
		GetLogger().critical(fmt, args...);
	}
}
