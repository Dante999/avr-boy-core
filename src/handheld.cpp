#include "avr-boy-core/handheld.hpp"

handheld_c::handheld_c(protocol_c::transmit_cb cb_transmit,
                       protocol_c::receive_cb  cb_receive)
    : m_protocol(cb_transmit, cb_receive)
{
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
	}
}

void handheld_c::handle_ping()
{
	transmit(CMD_PONG, 0, nullptr);
}