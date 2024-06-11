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
	spdlog::set_pattern( "[%H:%M:%S] [thread: %t] [%^ %l %$] %v" );

	spdlog::info( "Parsing command line arguments" );

	getoptw::args const cmd_args{argc, argv};

	if (auto const cfg_name = cmd_args["--config"]; !cfg_name.empty()) {
		g_config_filename = cfg_name;
		spdlog::info( "Setting config filename to \"{}\"", g_config_filename );
	}

	spdlog::info( "Parsing configuration..." );

	auto config = sh::c_config( g_config_filename );
	if ( !config )
	{
		spdlog::warn( "First launch detected. Please configure bot at: \"{}\"", g_config_filename );

		if ( !config.create( ) )
		{
			spdlog::error( "Failed to create config." );
			SLEEP( 5s );

			return 2;
		}

		SLEEP( 5s );

		return 1;
	}

	if ( !config.load( ) )
	{
		spdlog::error( "Failed to load config." );
		SLEEP( 5s );

		return 0;
	}

	spdlog::info( "Config success parsed. Starting telegram bot" );

	auto const bot = sh::telegram::c_bot( config( ).m_telegram_bot_token, config( ).m_super_admin_idx );
	if ( !bot )
	{
		spdlog::error( "Failed to authorize in bot with token: {}.", config( ).m_telegram_bot_token );
		SLEEP( 5s );

		return 1;
	}

	spdlog::info( "Creating telegram bot handlers." );

	// @note: Create command / events / callback handlers
	bot.make_handler( );

	spdlog::info( "Telegram bot handlers success created." );

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
		spdlog::critical( "Exception handled: {}", ex.what( ) );
	}
}