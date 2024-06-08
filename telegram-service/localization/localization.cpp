#include "localization.h"

void sh::c_localization::add_translation( hash_t const lang, hash_t const key, std::string const& translation )
{
	m_localization[ lang ][ key ] = translation;
}

std::string sh::c_localization::get_translation( hash_t const lang, hash_t const key ) const
{
	auto const language_it = m_localization.find( lang );
	if ( language_it != m_localization.end( ) )
	{
		auto const& translations = language_it->second;
		auto const key_it = translations.find( key );
		if ( key_it != translations.end( ) )
			return key_it->second;
	}

	return "Failed to find translation";
}
