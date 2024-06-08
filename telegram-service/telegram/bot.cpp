#include "bot.h"

#include "../logger/logger.h"
#include "keyboard/keyboard.h"

#pragma optimize("", off)
sh::telegram::c_bot::c_bot( std::string const& token, uint64_t const super_admin )
	: m_super_admin( super_admin ), m_bot( token )
{
	if ( !m_bot.getApi( ).getMe( )->username.empty( ) )
	{
		util::log::info( "Bot success authorized with name: {}", m_bot.getApi( ).getMe( )->username );
		util::log::info( "Super admin telegram ID: {}. Username: {}", m_super_admin, m_bot.getApi( ).getChat( m_super_admin )->username );
	}
}

#pragma optimize("", off)
void sh::telegram::c_bot::make_handler( )
{
	auto const keyboard_test = sh::telegram::c_inline_keyboard( )
		.add_button( { "test1", "test1" } )
		.add_button( { "test2", "test2" } )
		.add_button( { "test3", "test3" } )
		.add_button( { "test4", "test4" } )
		.add_button( { "test5", "test5" } )
		.add_button( { "test6", "test6" } )
		.add_button( { "test7", "test7" } )
		.add_button( { "test8", "test8" } )
		.create( );

	m_bot.getEvents( ).onCommand( "start", [ & ]( TgBot::Message::Ptr const& message )
	{
		m_bot.getApi( ).sendMessage( message->chat->id, "Hi!", false, 0, keyboard_test );
	} );

	//m_bot.getEvents( ).onCommand( "test1", [ &, keyboard = keyboard_test ](TgBot::Message::Ptr message )
	//{
	//	std::string response = "ok";
	//	m_bot.getApi( ).sendMessage( message->chat->id, response, false, 0, keyboard, "Markdown" );
	//} );

	//m_bot.getEvents( ).onCallbackQuery( [ &, keyboard = keyboard_test ](TgBot::CallbackQuery::Ptr query )
	//{
	//	if ( StringTools::startsWith( query->data, "test1" ) )
	//	{
	//		std::string response = "ok";
	//		m_bot.getApi( ).sendMessage( query->message->chat->id, response, false, 0, keyboard, "Markdown" );
	//	}
	//} );
}

void sh::telegram::c_bot::long_poll_loop( ) const
{
	m_bot.getApi( ).deleteWebhook( );

	auto long_poll = TgBot::TgLongPoll( m_bot );
	util::log::info( "Long poll started" );

	while ( true )
	{
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
