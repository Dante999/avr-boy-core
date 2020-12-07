/**
 * Copyright (C) 2020 Matthias Escher
 *
 * This file is part of avr-boy.
 *
 * avr-boy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * avr-boy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with avr-boy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef AVRBOY_PROTOCOL_H
#define AVRBOY_PROTOCOL_H

#include <stdbool.h>
#include <stdint.h>

#define PROTOCOL_MAX_LENGTH 50
#define PROTOCOL_VERSION    1

enum commands {
	PRTCL_START_BYTE,
	PRTCL_CMD_SYNC,
	PRTCL_CMD_ACK,
	PRTCL_CMD_NACK,
	PRTCL_CMD_PING,
	PRTCL_CMD_PONG,
	PRTCL_CMD_CHECK_VERSION,
	PRTCL_CMD_GET_BUTTONS,
	PRTCL_CMD_DISPLAY_BUFFER,
	PRTCL_CMD_DISPLAY_SPRITES,
	PRTCL_CMD_DRAW_TEXT,
	PRTCL_CMD_DRAW_PIXEL,
	PRTCL_CMD_CLEAR_SCREEN,
	PRTCL_CMD_SPRITE,
};

typedef void (*protocol_callback_transmit)(uint8_t byte);
typedef uint8_t (*protocol_callback_receive)(void);

struct protocol_package {
	uint8_t cmd;
	uint8_t length;
	uint8_t data[PROTOCOL_MAX_LENGTH];
};

void protocol_init(protocol_callback_transmit cb_transmit,
                   protocol_callback_receive  cb_receive);

void protocol_sync(void);
void protocol_send_package(uint8_t cmd, uint8_t length, const uint8_t *data);
void protocol_waitfor_package(struct protocol_package *package);
bool protocol_parse_received(uint8_t c);
bool protocol_receive_complete(void);
void protocol_copy_received(struct protocol_package *package);
void protocol_reset(void);

#endif /* AVRBOY_PROTOCOL_H */
