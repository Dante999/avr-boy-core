#ifndef HANDHELD_H
#define HANDHELD_H

#include "avr-boy-core/commands.hpp"
#include "avr-boy-core/protocol.hpp"

class handheld_c {

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

	void handle_ping();

public:
	handheld_c(protocol_c::transmit_cb cb_transmit,
	           protocol_c::receive_cb  cb_receive);

	void waitfor_instructions();
};

#endif /* HANDHELD_H */
