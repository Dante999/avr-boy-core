#ifndef HANDHELD_H
#define HANDHELD_H

#include "avr-boy-core/commands.hpp"
#include "avr-boy-core/graphx.hpp"
#include "avr-boy-core/protocol.hpp"

class handheld_c : public protocol_c {

public:
	virtual void publish_ready_for_communication() {}
	virtual void publish_status_busy() {}
	virtual void draw_buffer(graphx_c &graphx)                   = 0;
	virtual void get_buttons(avrboy_payload::buttons_s &buttons) = 0;

private:
	protocol_c::package_s m_received;
	graphx_c              m_graphx;

private:
	void transmit(uint8_t cmd, uint8_t length, const uint8_t *data);
	void waitfor_receive();
	void cmd_ping();
	void cmd_set_pixel(uint8_t *data);
	void cmd_set_text(uint8_t *data);
	void cmd_set_char(uint8_t *data);
	void cmd_set_tile_8x8(uint8_t *data);
	void cmd_draw_buffer();
	void cmd_clear_buffer();
	void cmd_get_buttons();

	inline void response_with(uint8_t cmd)
	{
		transmit(cmd, 0, nullptr);
	}

public:
	handheld_c();

	void waitfor_instructions();
};

#endif /* HANDHELD_H */
