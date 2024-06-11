#include "bot.h"

#include "../logger/logger.h"
#include "keyboard/keyboard.h"

#include "handler/handler.h"

#include "../core.h"

sh::telegram::c_bot::c_bot( std::string const& token, uint64_t const super_admin )
	: m_super_admin( super_admin )
{
	g_data.m_bot = std::make_shared<TgBot::Bot>( token );
	if ( !g_data.m_bot->getApi( ).getMe( )->username.empty( ) )
	{
		util::log::info( "Bot success authorized with name: {}", g_data.m_bot->getApi( ).getMe( )->username );
	}
}

void sh::telegram::c_bot::make_handler( )
{
	g_data.m_bot->getEvents( ).onCommand( "start", handler::command::handle_start );

	g_data.m_bot->getEvents( ).onAnyMessage( [ & ]( TgBot::Message::Ptr const& message )
	{
		util::log::debug( "User wrote {}\n", message->text.c_str( ) );
		if ( StringTools::startsWith( message->text, "/start" ) )
			return;

		g_data.m_bot->getApi( ).sendMessage( message->chat->id, "Your message is: " + message->text );
	} );

	g_data.m_bot->getEvents( ).onCallbackQuery( [ ]( callback_query_t const& query )
	{
		// @note: we only need to initialize it once
		static handler::c_callback callback;

		callback.handle( query );
	} );
}

void sh::telegram::c_bot::long_poll_loop( )
{
	if ( !g_data.m_bot->getApi( ).deleteWebhook( ) )
		util::log::warn( "Failed to delete webhook" );

	auto long_poll = TgBot::TgLongPoll( *g_data.m_bot );
	util::log::info( "Long poll started" );

	while ( true )
	{
		long_poll.start( );
	}
}

sh::telegram::c_bot::operator bool( ) const
{
	return g_data.m_bot->getApi( ).getMe( )->username.empty( ) == false;
}