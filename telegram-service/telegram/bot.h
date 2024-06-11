#pragma once
#include <cstdint>
#include <string>

#include <tgbot/tgbot.h>

namespace sh::telegram
{
	class c_bot
	{
		uint64_t m_super_admin;
		std::string m_telegram_token;
	public:
		explicit c_bot( std::string const& token, uint64_t super_admin );

		static void make_handler( );
		static void long_poll_loop( );

		explicit operator bool( ) const;
	};
}
