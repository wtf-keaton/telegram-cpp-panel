#pragma once
#include <spdlog/spdlog.h>

namespace sh
{
	using logger_ptr_t = std::shared_ptr<spdlog::logger>;

	class c_logger
	{
		logger_ptr_t m_logger;
	public:
		c_logger( ) = default;
		explicit c_logger( bool to_file );

		logger_ptr_t get( );

		explicit operator bool( ) const;
	};

	inline c_logger g_logger;
}