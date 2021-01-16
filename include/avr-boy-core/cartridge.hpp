#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include "avr-boy-core/commands.hpp"
#include "avr-boy-core/protocol.hpp"

class cartridge_c : public protocol_c {

public:
	virtual void transmitting_start() {}
	virtual void transmitting_end() {}

private:
	protocol_c::package_s m_received;

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
	result_e ping();
	void     sync_with_handheld();
	result_e set_pixel(uint8_t x, uint8_t y, avrboy_payload::color_e color);
	result_e set_text(uint8_t x, uint8_t y, const char *text);
	result_e set_char(uint8_t x, uint8_t y, char c);
	result_e set_tile_8x8(uint8_t x, uint8_t y, const uint8_t tile[8]);
	result_e get_buttons(avrboy_payload::buttons_s &buttons);
	result_e draw_buffer();
	result_e clear_buffer();
};

#endif /* CARTRIDGE_H */
