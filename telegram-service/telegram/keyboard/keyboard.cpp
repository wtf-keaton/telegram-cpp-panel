#include "keyboard.h"

sh::telegram::c_inline_keyboard::c_inline_keyboard()
{
	m_keyboard = std::make_shared_for_overwrite<TgBot::InlineKeyboardMarkup>();
}

sh::telegram::c_inline_keyboard sh::telegram::c_inline_keyboard::add_button( button_data_t const& button_data )
{
	TgBot::InlineKeyboardButton::Ptr new_button( new TgBot::InlineKeyboardButton );
	new_button->text = button_data.first;
	new_button->callbackData = button_data.second;

	m_keyboard_row.push_back( new_button );

	return *this;
}

#pragma optimize("", off)
TgBot::InlineKeyboardMarkup::Ptr sh::telegram::c_inline_keyboard::create( )
{
	m_keyboard->inlineKeyboard.push_back( m_keyboard_row );

	return m_keyboard;
}
