#include "config/config.h"

#include <thread>

#include "logger/logger.h"
#include "telegram/bot.h"

#define SLEEP(time) std::this_thread::sleep_for( time )

using namespace std::chrono_literals;

int main( )
{
	util::log::info( "Parsing configuration..." );

	auto config = sh::c_config( "config.json" );
	if ( !config )
	{
		util::log::warn( "First launch detected. Please configure bot at: \"config.json\"" );

		if ( !config.create( ) )
		{
			util::log::error( "Failed to create config." );
			SLEEP( 5s );

			return 2;
		}

		SLEEP( 5s );

		return 1;
	}

	if ( !config.load( ) )
	{
		util::log::error( "Failed to load config." );
		SLEEP( 5s );

		return 0;
	}

	util::log::info( "Config success parsed. Starting telegram bot" );

	auto bot = sh::telegram::c_bot( config( ).m_telegram_bot_token, config( ).m_super_admin_idx );
	if ( !bot )
	{
		util::log::error( "Failed to authorize in bot with token: {}.", config( ).m_telegram_bot_token );
		SLEEP( 5s );

		return 1;
	}

	util::log::info( "Creating telegram bot handlers." );

	// @note: Create command / events / callback handlers
	bot.make_handler( );

	util::log::info( "Telegram bot handlers success created." );

	signal( SIGINT, [ ]( int s )
	{
		printf( "SIGINT got\n" );
		exit( 0 );
	} );

	try
	{
		bot.long_poll_loop( );
	}
	catch ( std::exception const& ex )
	{
		util::log::critical( "Exception handled: {}", ex.what( ) );
	}

}