#pragma once
#include <vector>

#include <tgbot/tgbot.h>

namespace sh::telegram
{
	using button_data_t = std::pair<std::string /*button name*/, std::string /*button action*/>;
	using inline_keyboard_button_t = TgBot::InlineKeyboardButton::Ptr;
	using inline_keyboard_markup_t = TgBot::InlineKeyboardMarkup::Ptr;

	class c_inline_keyboard
	{
		inline_keyboard_markup_t m_keyboard;
		std::vector<inline_keyboard_button_t> m_keyboard_row[32];
		int m_row_index;

	public:
		c_inline_keyboard( );

		c_inline_keyboard add( button_data_t const& button_data );
		c_inline_keyboard next_row( );
		inline_keyboard_markup_t create( );
	};
}