#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "avr-boy-core/commands.hpp"
#include "avr-boy-core/protocol.hpp"

class cartridge_c {

public:
	typedef void (*before_transmit_cb)(void);
	typedef void (*after_transmit_cb)(void);

private:
	protocol_c            m_protocol;
	protocol_c::package_s m_received;

	before_transmit_cb m_cb_before_transmit = nullptr;
	after_transmit_cb  m_cb_after_transmit  = nullptr;

private:
	void transmit(uint8_t cmd, uint8_t length, const uint8_t *data);
	void waitfor_receive();

	inline void transmit_and_wait_for_answer(uint8_t cmd, uint8_t length,
	                                         const uint8_t *data)
	{
		transmit(cmd, length, data);
		waitfor_receive();
	}

public:
	cartridge_c(protocol_c::transmit_cb cb_transmit,
	            protocol_c::receive_cb  cb_receive);

	void set_before_transmit_callback(before_transmit_cb cb);
	void set_after_transmit_callback(after_transmit_cb cb);

	result_e ping();
	void     sync_with_handheld();
	result_e set_pixel(uint8_t x, uint8_t y, avrboy_payload::color_e color);
	result_e set_text(uint8_t x, uint8_t y, const char *text);
	result_e draw_buffer();
	result_e clear_buffer();
};

inline void cartridge_c::set_before_transmit_callback(before_transmit_cb cb)
{
	m_cb_before_transmit = cb;
}

inline void cartridge_c::set_after_transmit_callback(after_transmit_cb cb)
{
	m_cb_after_transmit = cb;
}

#endif /* CARTRIDGE_H */
