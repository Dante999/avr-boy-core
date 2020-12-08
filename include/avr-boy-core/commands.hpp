#ifndef DAOS_H
#define DAOS_H

#include <stdint.h>

enum result_e { RESULT_OK, RESULT_NOK, RESULT_WRONG_COMMAND };

enum commands_e {
	/* zero is reserverd for START_BYTE */
	CMD_SYNC = 1,
	CMD_ACK,
	CMD_NACK,
	CMD_PING,
	CMD_PONG,
	CMD_CHECK_VERSION,
	CMD_GET_BUTTONS,
	CMD_DISPLAY_BUFFER,
	CMD_DISPLAY_SPRITES,
	CMD_DRAW_TEXT,
	CMD_DRAW_PIXEL,
	CMD_CLEAR_SCREEN,
	CMD_SPRITE,
};

enum color_dao_e : uint8_t { COLOR_WHITE, COLOR_BLACK };

struct payload_pixel_s {
	uint8_t x;
	uint8_t y;
	uint8_t color;
};

#endif /* DAOS_H */
