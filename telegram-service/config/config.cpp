#include "config.h"

#include <filesystem>
#include <fstream>

#include "../json/json.h"

namespace fs = std::filesystem;

sh::c_config::c_config( std::string const& config_name )
	: m_config_data( {} ), m_config_name( config_name )
{}

bool sh::c_config::load( )
{
	std::string output;

	std::ifstream file( m_config_name );
	if ( !file.is_open( ) )
		return false;

	file >> output;

	file.close( );

	if ( !nlohmann::json::accept( output ) )
	{
		std::printf( "Output is not JSON\n" );

		return false;
	}

	auto const j = nlohmann::json::parse( output );
	m_config_data.m_super_admin_idx = j[ "super_admin" ].get<uint64_t>( );
	m_config_data.m_telegram_bot_token = j[ "telegram_token" ];

	return true;
}

bool sh::c_config::create( ) const
{
	std::ofstream file( m_config_name );
	if ( !file.is_open( ) )
		return false;

	nlohmann::json j;
	j[ "super_admin" ] = UINT64_MAX;
	j[ "telegram_token" ] = "token";

	file << j.dump( ) << std::endl;

	file.close( );

	return true;
}

sh::c_config::operator bool( ) const
{
	return fs::exists( m_config_name );
}

sh::c_config::config_data_t sh::c_config::operator()( ) const
{
	return m_config_data;
}