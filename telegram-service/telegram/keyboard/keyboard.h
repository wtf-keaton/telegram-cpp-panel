#pragma once
#include <vector>

#include <tgbot/tgbot.h>

namespace sh::telegram
{
	using button_data_t = std::pair<std::string /*button name*/, std::string /*button action*/>;
	class c_inline_keyboard
	{
		TgBot::InlineKeyboardMarkup::Ptr m_keyboard;
		std::vector<TgBot::InlineKeyboardButton::Ptr> m_keyboard_row[32];
		int m_row_index;

	public:
		c_inline_keyboard( );

		c_inline_keyboard add( button_data_t const& button_data );
		c_inline_keyboard next_row( );
		TgBot::InlineKeyboardMarkup::Ptr create( );
	};
}