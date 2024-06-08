#pragma once
#include <string>
#include <unordered_map>

#include "../hash/fnv1a.h"

namespace sh
{
	using localization_map_t = std::unordered_map<hash_t, std::unordered_map<hash_t, std::string>>;

	class c_localization
	{
		localization_map_t m_localization;
	public:
		c_localization( ) = default;

		void add_translation( hash_t lang, hash_t key, std::string const& translation );
		std::string get_translation( hash_t lang, hash_t key ) const;
	};

	inline c_localization g_localization;
}

#define GL(lang, key) sh::g_localization.get_translation( HASH( lang ), HASH( key ) ).c_str()