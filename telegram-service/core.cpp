#include "core.h"

#include "config/config.h"

#include <thread>

#include "localization/localization.h"

#include "logger/logger.h"
#include "telegram/bot.h"

#include "getoptw/getoptw.h"

#define SLEEP(time) std::this_thread::sleep_for( time )

using namespace std::chrono_literals;
using namespace std::string_literals;

std::string g_config_filename = "config.json";

int main(int argc, char** argv)
{
	util::log::info( "Parsing command line arguments" );

	getoptw::args cmd_args{argc, argv};

	if (auto cfg_name = cmd_args["--config"]; !cfg_name.empty()) {
		g_config_filename = cfg_name;
		util::log::info( "Setting config filename to \""s + g_config_filename + "\""s );
	}


	util::log::info( "Parsing configuration..." );

	auto config = sh::c_config( g_config_filename );
	if ( !config )
	{
		util::log::warn( "First launch detected. Please configure bot at: \"s" + g_config_filename + "\""s );

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