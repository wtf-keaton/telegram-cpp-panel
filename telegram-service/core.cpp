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

int main(int const argc, char** argv)
{
	// @note: setting up spdlog
	sh::g_logger = sh::c_logger( true );
	if ( !sh::g_logger )
	{
		std::printf( "Failed to initialize default logger\n" );
		SLEEP( 5s );
		return 1;
	}

	sh::g_logger.get()->info( "Parsing command line arguments" );

	getoptw::args const cmd_args{argc, argv};

	if (auto const cfg_name = cmd_args["--config"]; !cfg_name.empty()) {
		g_config_filename = cfg_name;
		sh::g_logger.get()->info( "Setting config filename to \"{}\"", g_config_filename );
	}

	sh::g_logger.get()->info( "Parsing configuration..." );

	auto config = sh::c_config( g_config_filename );
	if ( !config )
	{
		sh::g_logger.get()->warn( "First launch detected. Please configure bot at: \"{}\"", g_config_filename );

		if ( !config.create( ) )
		{
			sh::g_logger.get()->error( "Failed to create config." );
			SLEEP( 5s );

			return 2;
		}

		SLEEP( 5s );

		return 1;
	}

	if ( !config.load( ) )
	{
		sh::g_logger.get()->error( "Failed to load config." );
		SLEEP( 5s );

		return 0;
	}

	sh::g_logger.get()->info( "Config success parsed. Starting telegram bot" );

	auto const bot = sh::telegram::c_bot( config( ).m_telegram_bot_token, config( ).m_super_admin_idx );
	if ( !bot )
	{
		sh::g_logger.get()->error( "Failed to authorize in bot with token: {}.", config( ).m_telegram_bot_token );
		SLEEP( 5s );

		return 1;
	}

	sh::g_logger.get()->info( "Creating telegram bot handlers." );

	// @note: Create command / events / callback handlers
	bot.make_handler( );

	sh::g_logger.get()->info( "Telegram bot handlers success created." );

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
		sh::g_logger.get()->critical( "Exception handled: {}", ex.what( ) );
	}
}