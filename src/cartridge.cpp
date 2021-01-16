#include "avr-boy-core/cartridge.hpp"
#include <string.h>

void cartridge_c::transmit(uint8_t cmd, uint8_t length, const uint8_t *data)
{
	transmitting_start();
	send_package(cmd, length, data);
	transmitting_end();
}

void cartridge_c::waitfor_receive()
{
	waitfor_package(m_received);
}

result_e cartridge_c::ping()
{
	transmit_and_wait_for_answer(CMD_PING, 0, nullptr);

	return m_received.cmd == CMD_PONG ? RESULT_OK : RESULT_WRONG_COMMAND;
}

void cartridge_c::sync_with_handheld()
{
	sync();
}

result_e cartridge_c::set_pixel(uint8_t x, uint8_t y,
                                avrboy_payload::color_e color)
{
	avrboy_payload::pixel_s pixel = {x, y, color};

	transmit_and_wait_for_answer(CMD_SET_PIXEL, sizeof(pixel),
	                             reinterpret_cast<uint8_t *>(&pixel));

	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

result_e cartridge_c::set_text(uint8_t x, uint8_t y, const char *text)
{
	avrboy_payload::text_s data = {x, y, ""};

	strncpy(data.text, text, avrboy_payload::MAX_TEXT_LENGTH - 1);

	transmit_and_wait_for_answer(CMD_SET_TEXT, sizeof(data),
	                             reinterpret_cast<uint8_t *>(&data));

	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

result_e cartridge_c::set_char(uint8_t x, uint8_t y, char c)
{
	avrboy_payload::char_s data = {x, y, c};

	transmit_and_wait_for_answer(CMD_SET_CHAR, sizeof(data),
	                             reinterpret_cast<uint8_t *>(&data));

	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

result_e cartridge_c::set_tile_8x8(uint8_t x, uint8_t y, const uint8_t tile[])
{
	avrboy_payload::tile_8x8_s data;

	data.x = x;
	data.y = y;

	memcpy(data.tile, tile, 8);

	transmit_and_wait_for_answer(CMD_SET_TILE_8X8, sizeof(data),
	                             reinterpret_cast<uint8_t *>(&data));

	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

result_e cartridge_c::get_buttons(avrboy_payload::buttons_s &buttons)
{
	transmit_and_wait_for_answer(CMD_GET_BUTTONS, 0, nullptr);

	memcpy(&buttons, m_received.data, sizeof(buttons));

	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

result_e cartridge_c::draw_buffer()
{
	transmit_and_wait_for_answer(CMD_DRAW_BUFFER, 0, nullptr);

	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

result_e cartridge_c::clear_buffer()
{
	transmit_and_wait_for_answer(CMD_CLEAR_BUFFER, 0, nullptr);

	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}
