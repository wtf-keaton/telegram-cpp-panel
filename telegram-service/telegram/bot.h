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

		TgBot::Bot m_bot;
	public:
		explicit c_bot( std::string const& token, uint64_t super_admin );

		void make_handler( );
		void long_poll_loop( ) const;

		explicit operator bool( ) const;
		TgBot::Bot const& operator()( ) const;
		TgBot::Bot& operator()( );
	};
}
