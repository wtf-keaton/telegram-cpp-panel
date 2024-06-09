#pragma once
#include <vector>

#include <tgbot/tgbot.h>

namespace sh::telegram
{
	using button_data_t = std::pair<std::string, std::string>;
	class c_inline_keyboard
	{
		TgBot::InlineKeyboardMarkup::Ptr m_keyboard;
		std::vector<TgBot::InlineKeyboardButton::Ptr> m_keyboard_row;

	public:
		c_inline_keyboard( );

		c_inline_keyboard add_button( button_data_t const& button_data );
		TgBot::InlineKeyboardMarkup::Ptr create( );
	};
}