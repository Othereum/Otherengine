#pragma once
#include <spdlog/logger.h>
#include "API.hpp"

namespace oeng::log
{
	namespace level = spdlog::level;
	
	extern OEAPI spdlog::logger& logger;

	template <class... Args>
	void Log(level::level_enum level, std::string_view fmt, const Args&... args)
	{
		logger.log(level, fmt, args...);
	}
	
	template <class... Args>
	void Trace(std::string_view fmt, const Args&... args)
	{
		logger.trace(fmt, args...);
	}

	template <class... Args>
	void Debug(std::string_view fmt, const Args&... args)
	{
		logger.debug(fmt, args...);
	}

	template <class... Args>
	void Info(std::string_view fmt, const Args&... args)
	{
		logger.info(fmt, args...);
	}

	template <class... Args>
	void Warn(std::string_view fmt, const Args&... args)
	{
		logger.warn(fmt, args...);
	}

	template <class... Args>
	void Error(std::string_view fmt, const Args&... args)
	{
		logger.error(fmt, args...);
	}

	template <class... Args>
	void Critical(std::string_view fmt, const Args&... args)
	{
		logger.critical(fmt, args...);
	}
}
