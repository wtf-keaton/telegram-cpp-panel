#include "database.h"

sh::c_database::c_database( std::string_view const& db_data )
	:m_connection( pqxx::connection( db_data.data( ) ) ), m_worker( m_connection )
{
	std::printf( "Success connected to database: \"%s\"\n", m_connection.dbname( ) );
}

sh::c_database::operator bool( ) const
{
	return m_connection.is_open( );
}
