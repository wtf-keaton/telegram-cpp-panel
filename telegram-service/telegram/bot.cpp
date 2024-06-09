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
		util::log::info( "Super admin telegram ID: {}. Username: {}", m_super_admin, g_data.m_bot->getApi( ).getChat( m_super_admin )->username );
	}
}

void sh::telegram::c_bot::make_handler( )
{
	g_data.m_bot->getEvents( ).onCommand( "start", handler::command::handle_start );

	g_data.m_bot->getEvents( ).onAnyMessage( [ & ]( TgBot::Message::Ptr message )
	{
		printf( "User wrote %s\n", message->text.c_str( ) );
		if ( StringTools::startsWith( message->text, "/start" ) )
		{
			return;
		}
		g_data.m_bot->getApi( ).sendMessage( message->chat->id, "Your message is: " + message->text );
	} );

	g_data.m_bot->getEvents( ).onCallbackQuery( handler::callback::handle );
}

void sh::telegram::c_bot::long_poll_loop( ) const
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