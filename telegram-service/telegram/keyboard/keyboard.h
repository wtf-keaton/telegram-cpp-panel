#pragma once
#include <vector>

#include <tgbot/tgbot.h>

namespace sh::telegram
{
	class c_inline_keyboard
	{
		TgBot::InlineKeyboardMarkup::Ptr m_keyboard;
	public:
		c_inline_keyboard( ) = default;
	};
}