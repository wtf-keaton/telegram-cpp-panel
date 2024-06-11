#include "handler.h"
#include "../bot.h"

#include "../../core.h"
#include "../keyboard/keyboard.h"

void sh::telegram::handler::command::handle_start( message_t const& message )
{
	auto const keyboard = c_inline_keyboard( )
		.add( { "RU", "ru_lang" } )
		.add( { "EN", "en_lang" } )
		.create( );

	g_data.m_bot->getApi( ).sendMessage( message->chat->id, "Welcome to Telegram-cpp-bot! Select language", false, NULL, keyboard );
}

void sh::telegram::handler::callback::handle( callback_query_t const& query )
{
	if ( StringTools::startsWith( query->data, "ru_lang" ) )
	{
		g_data.m_bot->getApi( ).sendMessage( query->message->chat->id, "Вы выбрали русский язык! Поменять язык можно будет в настройках", false, 0, nullptr, "Markdown" );
	}
	else if ( StringTools::startsWith( query->data, "en_lang" ) )
	{
		g_data.m_bot->getApi( ).sendMessage( query->message->chat->id, "You have chosen English! You can change the language in the settings", false, 0, nullptr, "Markdown" );
	}

}
