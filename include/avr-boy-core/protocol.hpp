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

class protocol_c {

public:
	static constexpr uint8_t START_BYTE   = 0x00;
	static constexpr uint8_t MAX_DATA_LEN = 50;

	typedef void (*transmit_cb)(uint8_t byte);
	typedef uint8_t (*receive_cb)(void);

	struct package_s {
		uint8_t cmd;
		uint8_t length;
		uint8_t data[MAX_DATA_LEN];
	};

private:
	enum state_e {
		WAITFOR_SYNC,
		WAITFOR_CMD,
		WAITFOR_LENGTH,
		WAITFOR_DATA
	};

	state_e     m_state{WAITFOR_SYNC};
	transmit_cb m_cb_transmit;
	receive_cb  m_cb_receive;
	bool        m_receive_complete{false};
	package_s   m_package;

private:
	bool parse_received_byte(uint8_t byte);
	void copy_received_to(package_s &result);

public:
	protocol_c(transmit_cb cb_transmit, receive_cb cb_receive);

	void sync();
	void reset();
	void send_package(uint8_t cmd, uint8_t length, const uint8_t *data);
	void waitfor_package(package_s &result);
};

#endif /* AVRBOY_PROTOCOL_H */
