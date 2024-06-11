#pragma once
#include <tgbot/tgbot.h>
#include "../../hash/fnv1a.h"

namespace sh::telegram
{
	// @note: Custom type name
	using callback_query_t = TgBot::CallbackQuery::Ptr;
	using message_t = TgBot::Message::Ptr;
	using callback_function_t = std::function<void( callback_query_t const& )>;

	namespace handler
	{
		namespace command
		{
			void handle_start( message_t const& message );
		}

		class c_callback
		{
			std::unordered_map<hash_t, callback_function_t> m_handlers;
		public:
			c_callback( );

			void handle( callback_query_t const& query );
		};
	}
}
