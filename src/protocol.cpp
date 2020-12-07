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

#define PROTOCOL_LOG 0

#if PROTOCOL_LOG
#	include "util/logger.h"
#endif

enum state { WAITFOR_SYNC, WAITFOR_CMD, WAITFOR_LENGTH, WAITFOR_DATA };

static enum state              m_state = WAITFOR_SYNC;
static struct protocol_package m_package;

static volatile bool m_finished = false;

static protocol_callback_transmit m_callback_transmit = NULL;
static protocol_callback_receive  m_callback_receive  = NULL;

static void parse_byte(uint8_t byte)
{
	static uint8_t i = 0;

	switch (m_state) {
	case WAITFOR_SYNC:

		if (byte == PRTCL_START_BYTE) {
			i          = 0;
			m_state    = WAITFOR_CMD;
			m_finished = false;
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
			i          = 0;
			m_state    = WAITFOR_SYNC;
			m_finished = true;
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
			m_state    = WAITFOR_SYNC;
			i          = 0;
			m_finished = true;
		}

		break;
	}
}

/** ****************************************************************************
 * @brief returns if the receive of a data package is complete or not
 *
 * @return true  the data package is complete
 * @return false the data package is not yet complete
 **************************************************************************** */
bool protocol_receive_complete(void)
{
	return m_finished;
}

/** ****************************************************************************
 * @brief parses the given byte to the protocol
 *
 * @param c the received byte
 *
 * @return true  if the data package is complete
 * @return false if the data package is not yet complete
 **************************************************************************** */
bool protocol_parse_received(uint8_t c)
{
	parse_byte(c);
	return m_finished;
}

/** ****************************************************************************
 * @brief sends the given parameters to a receiver
 *
 * The method uses the callback which was given through the protocol_init(..)
 * for sending data to the receiver
 *
 * @param cmd    the command byte for the receiver
 * @param length the length of the following data
 * @param data   the data or payload itself
 **************************************************************************** */
void protocol_send_package(uint8_t cmd, uint8_t length, const uint8_t *data)
{
	m_callback_transmit(PRTCL_START_BYTE);
	m_callback_transmit(cmd);
	m_callback_transmit(length);

#if PROTOCOL_LOG
	char msg[35];
	snprintf(msg, 35, "send     : [c=%d|l=%d]", cmd, length);
	LOG_DEBUG(msg);
#endif

	for (uint8_t i = 0; i < length; i++) {
		m_callback_transmit(data[i]);
	}
}

void protocol_waitfor_package(struct protocol_package *package)
{

	while (!m_finished) {
		uint8_t c = m_callback_receive();
		protocol_parse_received(c);
	}

#if PROTOCOL_LOG
	char msg[35];
	snprintf(msg, 35, "received : [c=%d|l=%d]", m_package.cmd,
	         m_package.length);
	LOG_DEBUG(msg);
#endif

	protocol_copy_received(package);
	protocol_reset();
}

void protocol_sync(void)
{
	for (uint8_t i = 0; i <= PROTOCOL_MAX_LENGTH; i++) {
		m_callback_transmit(PRTCL_START_BYTE);
	}

	protocol_reset();
}

/** ****************************************************************************
 * @brief initialises the module and sets the transmit callback
 *
 * @param callback the callback method for transmitting data
 **************************************************************************** */
void protocol_init(protocol_callback_transmit cb_transmit,
                   protocol_callback_receive  cb_receive)
{
	m_callback_transmit = cb_transmit;
	m_callback_receive  = cb_receive;
}

/** ****************************************************************************
 * @brief resets the module to the initial state
 *
 * After that the protocol waits for receiving the SYNC byte again
 *
 **************************************************************************** */
void protocol_reset(void)
{
	m_state    = WAITFOR_SYNC;
	m_finished = false;
}

/** ****************************************************************************
 * @brief copies the internal received data to the given structure pointer
 *
 * @param dest_package the destination package into which the data should be
 *                     copied
 **************************************************************************** */
void protocol_copy_received(struct protocol_package *dest_package)
{
	dest_package->cmd    = m_package.cmd;
	dest_package->length = m_package.length;

	for (uint8_t i = 0; i < m_package.length; i++) {
		dest_package->data[i] = m_package.data[i];
	}
}
