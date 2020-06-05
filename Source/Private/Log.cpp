#include "Log.hpp"
#include <filesystem>
#include <spdlog/async.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace oeng::log
{
	static std::shared_ptr<spdlog::logger> logger_ptr;
	
	static spdlog::logger& InitializeLogger()
	{
		std::filesystem::create_directory("../Logs");
		
		auto daily_file = std::make_shared<spdlog::sinks::daily_file_sink_st>("../Logs/Log.log", 0, 0);
		auto stdout_color = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
		
		spdlog::sinks_init_list list{std::move(daily_file), std::move(stdout_color)};
		spdlog::init_thread_pool(8192, 1);
		
		logger_ptr = std::make_shared<spdlog::async_logger>(std::string{}, list, spdlog::thread_pool());
		logger_ptr->flush_on(spdlog::level::critical);

		return *logger_ptr;
	}
	
	OEAPI spdlog::logger& logger = InitializeLogger();
}
