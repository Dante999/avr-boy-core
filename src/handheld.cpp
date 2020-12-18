#include "avr-boy-core/handheld.hpp"

handheld_c::handheld_c(protocol_c::transmit_cb cb_transmit,
                       protocol_c::receive_cb  cb_receive)
    : m_protocol(cb_transmit, cb_receive)
{
	m_graphx.fill(0x00);
}

void handheld_c::transmit(uint8_t cmd, uint8_t length, const uint8_t *data)
{
	if (m_cb_before_transmit != nullptr)
		m_cb_before_transmit();

	m_protocol.send_package(cmd, length, data);

	if (m_cb_after_transmit != nullptr)
		m_cb_after_transmit();
}

void handheld_c::waitfor_receive()
{
	m_protocol.waitfor_package(m_received);
}

void handheld_c::waitfor_instructions()
{
	waitfor_receive();

	switch (m_received.cmd) {
	case CMD_PING:
		handle_ping();
		break;
	case CMD_SET_PIXEL:
		handle_set_pixel(
		    reinterpret_cast<payload_pixel_s *>(m_received.data));
		break;
	}
}

void handheld_c::handle_ping()
{
	transmit(CMD_PONG, 0, nullptr);
}

void handheld_c::handle_set_pixel(const payload_pixel_s *pixel)
{
	m_graphx.draw_pixel(pixel->x, pixel->y,
	                    (pixel->color == color_dao_e::COLOR_BLACK)
	                        ? graphx_c::PIXEL_ON
	                        : graphx_c::PIXEL_OFF);

	transmit(CMD_ACK, 0, nullptr);
}