#include "handler.h"
#include "../bot.h"

#include "../../core.h"
#include "../keyboard/keyboard.h"

#include "../../details/format.h"

void sh::telegram::handler::command::handle_start( message_t const& message )
{
	// @note: Example keyboard builder
	auto const keyboard = c_inline_keyboard( )
		.add( { "RU", "ru_lang" } )
		.add( { "EN", "en_lang" } )
		.create( );

	g_data.m_bot->getApi( ).sendMessage( message->chat->id, "Welcome to Telegram-cpp-bot! Select language", false, NULL, keyboard );
}

sh::telegram::handler::c_callback::c_callback()
{
	m_handlers[ "ru_lang"_h ] = [ ]( callback_query_t const& query ) -> void
	{
		g_data.m_bot->getApi( ).sendMessage( query->message->chat->id, "meh", false, 0, nullptr, "Markdown" );
		// @todo: add information about user into database
	};

	m_handlers[ "en_lang"_h ] = [ ]( callback_query_t const& query ) -> void
	{
		g_data.m_bot->getApi( ).sendMessage( query->message->chat->id, "You have chosen English! You can change the language in the settings", false, 0, nullptr, "Markdown" );
		// @todo: add information about user into database
	};
}

void sh::telegram::handler::c_callback::handle(callback_query_t const& query)
{
	if ( auto const handler = m_handlers.find( HASH( query->data.c_str( ) ) ); handler != m_handlers.end( ) )
	{
		if ( !handler->second )
			throw std::runtime_error( details::format(
				"in [", details::source_location_format( std::source_location::current( ) ), "] cannot find callback='", handler->first, "'" ) );

		handler->second( query );
	}
}
