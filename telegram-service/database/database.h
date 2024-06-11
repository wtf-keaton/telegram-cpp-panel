#pragma once
#include <string>
namespace sh
{
	class c_database
	{

	public:
		explicit c_database( std::string_view const& db_data );

		explicit operator bool( ) const;
	};
}