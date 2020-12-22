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
	CMD_DRAW_BUFFER,
	CMD_CLEAR_BUFFER
};

namespace avrboy_payload {

constexpr uint8_t MAX_TEXT_LENGTH = 20;

enum color_e : uint8_t { COLOR_WHITE, COLOR_BLACK };

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

} // namespace avrboy_payload

#endif /* DAOS_H */
