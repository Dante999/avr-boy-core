#include "avr-boy-core/cartridge.hpp"
#include <string.h>

cartridge_c::cartridge_c(protocol_c::transmit_cb cb_transmit,
                         protocol_c::receive_cb  cb_receive)
    : m_protocol(cb_transmit, cb_receive)
{
}

void cartridge_c::transmit(uint8_t cmd, uint8_t length, const uint8_t *data)
{
	if (m_cb_before_transmit != nullptr)
		m_cb_before_transmit();

	m_protocol.send_package(cmd, length, data);

	if (m_cb_after_transmit != nullptr)
		m_cb_after_transmit();
}

void cartridge_c::waitfor_receive()
{
	m_protocol.waitfor_package(m_received);
}

result_e cartridge_c::ping()
{
	transmit_and_wait_for_answer(CMD_PING, 0, nullptr);

	return m_received.cmd == CMD_PONG ? RESULT_OK : RESULT_WRONG_COMMAND;
}

void cartridge_c::sync_with_handheld()
{
	m_protocol.sync();
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

	strncpy(data.text, text, avrboy_payload::MAX_TEXT_LENGTH);

	transmit_and_wait_for_answer(CMD_SET_TEXT, sizeof(data),
	                             reinterpret_cast<uint8_t *>(&data));

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
