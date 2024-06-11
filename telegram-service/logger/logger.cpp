#include "logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/daily_file_sink.h"

sh::c_logger::c_logger( bool const to_file )
{
	auto const sink = std::make_shared<spdlog::sinks::wincolor_stdout_sink_mt>( );
	sink->set_pattern( "[%H:%M:%S] [thread: %t] [%^ %l %$] %v" );

	auto const file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>( "logs/service.log" );
	file_sink->set_pattern( "[%d/%m/%Y %H:%M:%S] [%n] [%l] %v" );

	// @note: Create log file every 24 hours
	auto const daily_sink = std::make_shared<spdlog::sinks::daily_file_sink_mt>( "logs/daily.log", 24, 0 );
	daily_sink->set_pattern( "[%d/%m/%Y %H:%M:%S] [daily-%n] [%l] %v" );

	std::vector<spdlog::sink_ptr> log_sinks;
	log_sinks.emplace_back( sink );
	log_sinks.emplace_back( daily_sink );
	if ( to_file )
		log_sinks.emplace_back( file_sink );

	m_logger = std::make_shared<spdlog::logger>( "server", log_sinks.begin( ),
												 log_sinks.end( ) );

	spdlog::register_logger( m_logger );

	spdlog::flush_every( std::chrono::seconds( 1 ) );
}

sh::logger_ptr_t sh::c_logger::get()
{
	return m_logger;
}

sh::c_logger::operator bool() const
{
	return m_logger != nullptr;
}
