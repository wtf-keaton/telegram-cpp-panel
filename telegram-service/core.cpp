#include "config/config.h"
#include "database/database.h"
#include "localization/localization.h"

#include <iostream>
#include <thread>

// vcpkg includes
#include <tgbot/tgbot.h>

#define SLEEP_S(second) std::this_thread::sleep_for( std::chrono::seconds( second ) )

using namespace TgBot;

int main( )
{
	setlocale( LC_ALL, "ru" );
	auto config = sh::c_config( "config.json" );
	if ( !config )
	{
		std::printf( "First launch detected. Please configure bot at: \"config.json\"\n" );

		if ( !config.create( ) )
		{
			std::printf( "Failed to create config.\n" );
			SLEEP_S( 5 );

			return 2;
		}

		SLEEP_S( 5 );

		return 1;
	}

	if ( !config.load( ) )
	{
		std::printf( "Failed to load config.\n" );
		SLEEP_S( 5 );

		return 0;
	}

	std::cout << "Database name: " << config( ).m_database_name << std::endl
		<< "Database username: " << config( ).m_database_username << std::endl
		<< "Database password: " << config( ).m_database_password << std::endl
		<< "Telegram token: " << config( ).m_telegram_bot_token << std::endl
		<< "Telegram super admin: " << config( ).m_super_admin_idx << std::endl;

	auto const db_data = std::format( "host=localhost port=5432 dbname={} user={} password={}",
									  config( ).m_database_name, config( ).m_database_username, config( ).m_database_password );

	sh::g_localization.add_translation( "ru"_h, "#kt_hello"_h, "Привет" );
	sh::g_localization.add_translation( "ru"_h, "#kt_world"_h, "Мир" );

	sh::g_localization.add_translation( "en"_h, "#kt_hello"_h, "Hello" );
	sh::g_localization.add_translation( "en"_h, "#kt_world"_h, "World" );

	std::printf( "%s", GL( "ru", "#kt_hello" ) );
	std::printf( " %s\n", GL( "ru", "#kt_world" ) );

	std::printf( "%s", GL( "en", "#kt_hello" ) );
	std::printf( " %s\n", GL( "en", "#kt_world" ) );

	try
	{
		auto const database = sh::c_database( db_data );
		if ( !database )
			return 0;
	}
	catch ( ... )
	{
		// @todo: Create exception handler
	}

}