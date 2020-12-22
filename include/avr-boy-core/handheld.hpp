#ifndef HANDHELD_H
#define HANDHELD_H

#include "avr-boy-core/commands.hpp"
#include "avr-boy-core/graphx.hpp"
#include "avr-boy-core/protocol.hpp"

class handheld_c {

public:
	typedef void (*before_transmit_cb)(void);
	typedef void (*after_transmit_cb)(void);
	typedef void (*draw_buffer_cb)(graphx_c &gfx);

private:
	protocol_c            m_protocol;
	protocol_c::package_s m_received;

	before_transmit_cb m_cb_before_transmit = nullptr;
	after_transmit_cb  m_cb_after_transmit  = nullptr;
	draw_buffer_cb     m_cb_draw_buffer     = nullptr;

	graphx_c m_graphx;

private:
	void transmit(uint8_t cmd, uint8_t length, const uint8_t *data);
	void waitfor_receive();
	void cmd_ping();
	void cmd_set_pixel(uint8_t *data);
	void cmd_set_text(uint8_t *data);
	void cmd_set_tile_8x8(uint8_t *data);
	void cmd_draw_buffer();
	void cmd_clear_buffer();

	inline void response_with(uint8_t cmd)
	{
		transmit(cmd, 0, nullptr);
	}

public:
	handheld_c(protocol_c::transmit_cb cb_transmit,
	           protocol_c::receive_cb  cb_receive);

	void set_before_transmit_callback(before_transmit_cb cb);
	void set_after_transmit_callback(after_transmit_cb cb);
	void set_draw_buffer_callback(draw_buffer_cb cb);
	void waitfor_instructions();
};

inline void handheld_c::set_before_transmit_callback(before_transmit_cb cb)
{
	m_cb_before_transmit = cb;
}

inline void handheld_c::set_after_transmit_callback(after_transmit_cb cb)
{
	m_cb_after_transmit = cb;
}

inline void handheld_c::set_draw_buffer_callback(draw_buffer_cb cb)
{
	m_cb_draw_buffer = cb;
}

#endif /* HANDHELD_H */
