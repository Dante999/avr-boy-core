#include "avr-boy-core/handheld.hpp"
#include "avr-boy-core/font5x7.hpp"

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
		cmd_ping();
		break;
	case CMD_SET_PIXEL:
		cmd_set_pixel(m_received.data);
		break;
	case CMD_SET_TEXT:
		cmd_set_text(m_received.data);
		break;
	case CMD_SET_CHAR:
		cmd_set_char(m_received.data);
		break;
	case CMD_SET_TILE_8X8:
		cmd_set_tile_8x8(m_received.data);
		break;
	case CMD_DRAW_BUFFER:
		cmd_draw_buffer();
		break;
	case CMD_CLEAR_BUFFER:
		cmd_clear_buffer();
		break;
	default:
		response_with(CMD_NOT_SUPPORTED);
		break;
	}
}

void handheld_c::cmd_ping()
{
	transmit(CMD_PONG, 0, nullptr);
}

void handheld_c::cmd_set_pixel(uint8_t *data)
{
	auto pixel = reinterpret_cast<avrboy_payload::pixel_s *>(data);

	m_graphx.draw_pixel(
	    pixel->x, pixel->y,
	    (pixel->color == avrboy_payload::color_e::COLOR_BLACK)
	        ? graphx_c::PIXEL_ON
	        : graphx_c::PIXEL_OFF);

	response_with(CMD_ACK);
}

void handheld_c::cmd_set_text(uint8_t *data)
{
	auto text = reinterpret_cast<avrboy_payload::text_s *>(data);

	text->text[avrboy_payload::MAX_TEXT_LENGTH - 1] = '\0'; // safety first
	m_graphx.draw_string(font5x7, text->x, text->y, text->text);

	response_with(CMD_ACK);
}

void handheld_c::cmd_set_char(uint8_t *data)
{
	auto character = reinterpret_cast<avrboy_payload::char_s *>(data);
	m_graphx.draw_char(font5x7, character->x, character->y,
	                   character->letter);

	response_with(CMD_ACK);
}

void handheld_c::cmd_set_tile_8x8(uint8_t *data)
{
	auto tile = reinterpret_cast<avrboy_payload::tile_8x8_s *>(data);

	m_graphx.draw_tile(tile->x, tile->y, tile->tile, 8, 8);

	response_with(CMD_ACK);
}

void handheld_c::cmd_draw_buffer()
{
	if (m_cb_draw_buffer != nullptr) {
		m_cb_draw_buffer(m_graphx);
		response_with(CMD_ACK);
	}
	else {
		response_with(CMD_NOT_SUPPORTED);
	}
}

void handheld_c::cmd_clear_buffer()
{
	m_graphx.fill(graphx_c::PIXEL_OFF);

	response_with(CMD_ACK);
}

void handheld_c::cmd_get_buttons()
{
	if (m_cb_get_buttons != nullptr) {
		avrboy_payload::buttons_s buttons;
		m_cb_get_buttons(buttons);

		transmit(CMD_ACK, sizeof(buttons),
		         reinterpret_cast<uint8_t *>(&buttons));
	}
	else {
		response_with(CMD_NOT_SUPPORTED);
	}
}
