#include "keyboard.h"

sh::telegram::c_inline_keyboard::c_inline_keyboard( )
	: m_row_index( 0 )
{
	m_keyboard = std::make_shared_for_overwrite<TgBot::InlineKeyboardMarkup>();
}

sh::telegram::c_inline_keyboard sh::telegram::c_inline_keyboard::add_button( button_data_t const& button_data )
{
	TgBot::InlineKeyboardButton::Ptr const new_button( new TgBot::InlineKeyboardButton );
	new_button->text = button_data.first;
	new_button->callbackData = button_data.second;

	m_keyboard_row[ m_row_index ].push_back( new_button );

	return *this;
}

sh::telegram::c_inline_keyboard sh::telegram::c_inline_keyboard::next_row( )
{
	++m_row_index;

	return *this;
}

TgBot::InlineKeyboardMarkup::Ptr sh::telegram::c_inline_keyboard::create( )
{
	// @note: If we're not call next_row function after create buttons
	++m_row_index;

	for ( auto i = 0; i < m_row_index; i++ )
	{
		m_keyboard->inlineKeyboard.push_back( m_keyboard_row[ i ] );
	}

	return m_keyboard;
}
