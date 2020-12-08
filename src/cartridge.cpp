#include "avr-boy-core/cartridge.hpp"

cartridge_c::cartridge_c(protocol_c::transmit_cb cb_transmit,
                         protocol_c::receive_cb  cb_receive)
    : m_protocol(cb_transmit, cb_receive)
{
}

result_e cartridge_c::ping()
{
	transmit(CMD_PING, 0, nullptr);
	waitfor_receive();

	return m_received.cmd == CMD_PONG ? RESULT_OK : RESULT_WRONG_COMMAND;
}