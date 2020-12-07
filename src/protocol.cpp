/**
 * Copyright (C) 2020 dante999
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
#include "avr-boy-core/protocol.hpp"

#include <stdio.h>

protocol_c::protocol_c(transmit_cb cb_transmit,
                       receive_cb  cb_receive)
    : m_cb_transmit(cb_transmit), //
      m_cb_receive(cb_receive)
{
}

bool protocol_c::parse_received_byte(uint8_t byte)
{
	static uint8_t i = 0;

	switch (m_state) {
	case WAITFOR_SYNC:

		if (byte == PRTCL_START_BYTE) {
			i                  = 0;
			m_state            = WAITFOR_CMD;
			m_receive_complete = false;
		}

		break;

	case WAITFOR_CMD:

		if (byte != PRTCL_START_BYTE) {
			m_package.cmd = byte;
			m_state       = WAITFOR_LENGTH;
		}

		break;

	case WAITFOR_LENGTH:

		m_package.length = byte;

		if (m_package.length > PROTOCOL_MAX_LENGTH)
			m_package.length = PROTOCOL_MAX_LENGTH;

		if (m_package.length == 0) {
			i                  = 0;
			m_state            = WAITFOR_SYNC;
			m_receive_complete = true;
		}
		else {
			m_state = WAITFOR_DATA;
		}

		break;

	case WAITFOR_DATA:

		if (i < m_package.length) {
			m_package.data[i] = byte;
			i++;
		}

		if (i >= m_package.length || i >= PROTOCOL_MAX_LENGTH) {
			m_state            = WAITFOR_SYNC;
			i                  = 0;
			m_receive_complete = true;
		}

		break;
	}

	return m_receive_complete;
}

void protocol_c::waitfor_package(package_s &result)
{

	while (!m_receive_complete) {
		const uint8_t c = m_cb_receive();
		parse_received_byte(c);
	}

	copy_received_to(result);
	reset();
}

void protocol_c::sync(void)
{
	for (uint8_t i = 0; i <= PROTOCOL_MAX_LENGTH; i++) {
		m_cb_transmit(PRTCL_START_BYTE);
	}

	reset();
}

void protocol_c::reset(void)
{
	m_state            = WAITFOR_SYNC;
	m_receive_complete = false;
}

void protocol_c::copy_received_to(package_s &result)
{
	result.cmd    = m_package.cmd;
	result.length = m_package.length;

	for (uint8_t i = 0; i < m_package.length; i++) {
		result.data[i] = m_package.data[i];
	}
}

void protocol_c::send_package(uint8_t cmd, uint8_t length, const uint8_t *data)
{
	m_cb_transmit(PRTCL_START_BYTE);
	m_cb_transmit(cmd);
	m_cb_transmit(length);

	for (uint8_t i = 0; i < length; ++i) {
		m_cb_transmit(data[i]);
	}
}