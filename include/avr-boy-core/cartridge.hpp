#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "avr-boy-core/commands.hpp"
#include "avr-boy-core/protocol.hpp"

class cartridge_c {

private:
	protocol_c            m_protocol;
	protocol_c::package_s m_received;

private:
	inline void transmit(uint8_t cmd, uint8_t length, const uint8_t *data)
	{
		m_protocol.send_package(cmd, length, data);
	}

	inline void waitfor_receive()
	{
		m_protocol.waitfor_package(m_received);
	}

public:
	cartridge_c(protocol_c::transmit_cb cb_transmit,
	            protocol_c::receive_cb  cb_receive);

	result_e ping();
};

#endif /* CARTRIDGE_H */
