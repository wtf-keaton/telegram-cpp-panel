#pragma once
#include <pqxx/pqxx>

namespace sh
{
	class c_database
	{
		pqxx::connection m_connection;
		pqxx::work m_worker;
	public:
		explicit c_database( std::string_view const& db_data );

		explicit operator bool( ) const;
	};
}