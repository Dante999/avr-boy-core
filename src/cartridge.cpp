#include "avr-boy-core/cartridge.hpp"

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
	transmit(CMD_PING, 0, nullptr);
	waitfor_receive();

	return m_received.cmd == CMD_PONG ? RESULT_OK : RESULT_WRONG_COMMAND;
}

result_e cartridge_c::set_pixel(uint8_t x, uint8_t y, color_dao_e color)
{
	payload_pixel_s pixel = {x, y, color};

	transmit(CMD_SET_PIXEL, sizeof(payload_pixel_s),
	         reinterpret_cast<uint8_t *>(&pixel));

	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

result_e cartridge_c::draw_buffer()
{
	transmit(CMD_DRAW_BUFFER, 0, nullptr);
	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}

result_e cartridge_c::clear_buffer()
{
	transmit(CMD_CLEAR_BUFFER, 0, nullptr);
	return m_received.cmd == CMD_ACK ? RESULT_OK : RESULT_NOK;
}