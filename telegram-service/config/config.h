#pragma once
#include <string>

namespace sh
{
	class c_config
	{
		struct config_data_t
		{
			/*database info*/
			std::string m_database_name;
			std::string m_database_username;
			std::string m_database_password;

			/*telegram bot info*/
			uint64_t  m_super_admin_idx;
			std::string m_telegram_bot_token;
		} m_config_data;

		std::string m_config_name;

	public:
		explicit c_config( std::string const& config_name );

		bool load( );
		bool create( ) const;

		explicit operator bool( ) const;
		config_data_t operator()( ) const;
	};
}