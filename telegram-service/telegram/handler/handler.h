#pragma once
#include <tgbot/tgbot.h>

namespace sh::telegram
{
	// @note: Custom type name
	using callback_query_t = TgBot::CallbackQuery::Ptr;
	using message_t = TgBot::Message::Ptr;

	namespace handler
	{
		namespace command
		{
			void handle_start( message_t const& message );
		}

		namespace callback
		{
			void handle( callback_query_t const& query );
		}
	}
}
