#pragma once
#include <tgbot/tgbot.h>

namespace sh
{
	inline struct global_data_t
	{
		std::shared_ptr<TgBot::Bot> m_bot;
	} g_data;
}