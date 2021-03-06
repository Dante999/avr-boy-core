#ifndef DAOS_H
#define DAOS_H

#include <stdint.h>

enum result_e { RESULT_OK, RESULT_NOK, RESULT_WRONG_COMMAND };

enum commands_e {
	/* zero is reserverd for START_BYTE */
	CMD_SYNC = 1,
	CMD_ACK,
	CMD_NACK,
	CMD_NOT_SUPPORTED,
	CMD_PING,
	CMD_PONG,
	CMD_CHECK_VERSION,
	CMD_SET_PIXEL,
	CMD_SET_TEXT,
	CMD_SET_CHAR,
	CMD_SET_TILE_8X8,
	CMD_DRAW_BUFFER,
	CMD_CLEAR_BUFFER,
	CMD_GET_BUTTONS
};

namespace avrboy_payload {

constexpr uint8_t MAX_TEXT_LENGTH = 20;

enum color_e : uint8_t { COLOR_WHITE, COLOR_BLACK };
enum button_e : uint8_t {
	BUTTON_UP,
	BUTTON_DOWN,
	BUTTON_LEFT,
	BUTTON_RIGHT,
	BUTTON_A,
	BUTTON_B,
	BUTTON_START,
	BUTTON_SELECT
};

struct text_s {
	uint8_t x;
	uint8_t y;
	char    text[MAX_TEXT_LENGTH];
};

struct pixel_s {
	uint8_t x;
	uint8_t y;
	color_e color;
};

struct char_s {
	uint8_t x;
	uint8_t y;
	char    letter;
};

struct tile_8x8_s {
	uint8_t x;
	uint8_t y;
	uint8_t tile[8];
};

struct buttons_s {
	uint16_t states;
};

} // namespace avrboy_payload

#endif /* DAOS_H */
