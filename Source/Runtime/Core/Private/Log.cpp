#include "Log.hpp"
#include <filesystem>
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Templates/Sync.hpp"

namespace oeng::log
{
	static constexpr bool kThreadSafe = OE_LOG_THREADSAFE;
	
	static std::shared_ptr<spdlog::logger> InitializeLogger()
	{
		std::filesystem::create_directory("../Logs");

		using Daily = std::conditional_t<kThreadSafe,
			spdlog::sinks::daily_file_sink_mt,
			spdlog::sinks::daily_file_sink_st>;
		
		using Stdout = std::conditional_t<kThreadSafe,
			spdlog::sinks::stdout_color_sink_mt,
			spdlog::sinks::stdout_color_sink_st>;

		auto daily_file = std::make_shared<Daily>("../Logs/Log.log", 0, 0);
		auto stdout_color = std::make_shared<Stdout>();
		
		spdlog::sinks_init_list list{std::move(daily_file), std::move(stdout_color)};
		auto logger_ptr = std::make_shared<spdlog::logger>(std::string{}, list);
		logger_ptr->flush_on(level::critical);

		return logger_ptr;
	}

	OEAPI spdlog::logger& GetLogger()
	{
		static const auto logger_ptr = InitializeLogger();
		return *logger_ptr;
	}
}
