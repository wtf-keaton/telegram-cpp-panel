#include "bot.h"

#include "../logger/logger.h"

sh::telegram::c_bot::c_bot( std::string const& token, uint64_t const super_admin )
	: m_super_admin( super_admin ), m_bot( token )
{
	if ( !m_bot.getApi( ).getMe( )->username.empty( ) )
	{
		util::log::info( "Bot success authorized with name: {}", m_bot.getApi( ).getMe( )->username );
		util::log::info( "Super admin telegram ID: {}. Username: {}", m_super_admin, m_bot.getApi( ).getChat( m_super_admin )->username );
	}
}

void sh::telegram::c_bot::make_handler( )
{
	// @note: okay it's shit code
	auto& bot = m_bot;

	m_bot.getEvents( ).onCommand( "start", [ &bot ]( TgBot::Message::Ptr const& message )
	{
		bot.getApi( ).sendMessage( message->chat->id, "Hi!", false, 0, nullptr );
	} );
}

void sh::telegram::c_bot::long_poll_loop( ) const
{
	TgBot::TgLongPoll long_poll( m_bot );
	while ( true )
	{
		util::log::info( "Long poll started" );
		long_poll.start( );
	}
}

sh::telegram::c_bot::operator bool( ) const
{
	return m_bot.getApi( ).getMe( )->username.empty( ) == false;
}

const TgBot::Bot& sh::telegram::c_bot::operator()( ) const
{
	return m_bot;
}

TgBot::Bot& sh::telegram::c_bot::operator()( )
{
	return m_bot;
}
