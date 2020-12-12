#include "gtest/gtest.h"

#include "avr-boy-core/protocol.hpp"

static constexpr uint8_t DUMMY_CMD  = 0x10;
static constexpr uint8_t START_BYTE = protocol_c::START_BYTE;
static constexpr uint8_t BUFFER_LEN = 255;

static uint8_t m_receive_buffer[BUFFER_LEN];
static uint8_t m_transmit_buffer[BUFFER_LEN];

static uint8_t m_transmit_index;
static uint8_t m_receive_index;

class protocol_test : public ::testing::Test {

public:
	static void init()
	{
		m_transmit_index = 0;
		m_receive_index  = 0;

		memset(m_receive_buffer, 0x00, BUFFER_LEN);
		memset(m_transmit_buffer, 0x00, BUFFER_LEN);
	}

	static void transmit(uint8_t byte)
	{
		m_transmit_buffer[m_transmit_index++] = byte;
	}

	static uint8_t receive()
	{
		return m_receive_buffer[m_receive_index++];
	}
};

TEST_F(protocol_test, test_transmit_callback)
{
	init();

	const uint8_t data[2] = {3, 4};
	protocol_c    prot(&this->transmit, &this->receive);

	prot.send_package(1, 2, data);

	for (uint8_t i = 0; i < 5; ++i) {
		ASSERT_EQ(i, m_transmit_buffer[i])
		    << "Different values for index " << static_cast<int>(i);
	}
}

TEST_F(protocol_test, receive_callback)
{
	protocol_c m_protocol(&this->transmit, &this->receive);
	init();

	m_receive_buffer[0] = START_BYTE;
	m_receive_buffer[1] = DUMMY_CMD;
	m_receive_buffer[2] = 1;
	m_receive_buffer[3] = 2;

	ASSERT_EQ(0, m_receive_index);

	protocol_c::package_s received;
	memset(&received, 0x00, sizeof(received));

	m_protocol.waitfor_package(received);

	ASSERT_EQ(4, m_receive_index);

	ASSERT_EQ(DUMMY_CMD, received.cmd);
	ASSERT_EQ(1, received.length);
	ASSERT_EQ(2, received.data[0]);
}

TEST_F(protocol_test, transmit_callback)
{
	protocol_c m_protocol(&this->transmit, &this->receive);
	init();

	uint8_t data[4] = {1, 2, 3, 4};

	m_protocol.send_package(data[0], data[1], &data[2]);

	ASSERT_EQ(5, m_transmit_index);

	ASSERT_EQ(START_BYTE, m_transmit_buffer[0]);

	ASSERT_EQ(data[0], m_transmit_buffer[1]);
	ASSERT_EQ(data[1], m_transmit_buffer[2]);
	ASSERT_EQ(data[2], m_transmit_buffer[3]);
	ASSERT_EQ(data[3], m_transmit_buffer[4]);
}

TEST_F(protocol_test, receive_package_with_min_length)
{
	protocol_c m_protocol(&this->transmit, &this->receive);
	init();

	uint8_t sync   = START_BYTE;
	uint8_t cmd    = DUMMY_CMD;
	uint8_t length = 0;

	m_receive_buffer[0] = sync;
	m_receive_buffer[1] = cmd;
	m_receive_buffer[2] = length;

	// check received data
	protocol_c::package_s received;
	memset(&received, 0x00, sizeof(received));

	m_protocol.waitfor_package(received);

	ASSERT_EQ(3, m_receive_index);
}

TEST_F(protocol_test, receive_package_with_max_length)
{
	ASSERT_TRUE(protocol_c::MAX_DATA_LEN < 255)
	    << "Protocol length already exceed 8bit limit! Cannot test";

	protocol_c m_protocol(&this->transmit, &this->receive);
	init();

	uint8_t sync   = START_BYTE;
	uint8_t cmd    = DUMMY_CMD;
	uint8_t length = protocol_c::MAX_DATA_LEN + 1;

	m_receive_buffer[0] = sync;
	m_receive_buffer[1] = cmd;
	m_receive_buffer[2] = length;

	// check received data
	protocol_c::package_s received;
	memset(&received, 0x00, sizeof(received));

	m_protocol.waitfor_package(received);

	ASSERT_EQ(protocol_c::MAX_DATA_LEN, received.length);
}

TEST_F(protocol_test, reset_when_multible_start_bytes)
{
	protocol_c m_protocol(&this->transmit, &this->receive);
	init();

	m_receive_buffer[0] = 3;
	m_receive_buffer[1] = 3;
	m_receive_buffer[2] = 3;
	m_receive_buffer[3] = START_BYTE;
	m_receive_buffer[4] = START_BYTE;
	m_receive_buffer[5] = START_BYTE;
	m_receive_buffer[6] = START_BYTE;
	m_receive_buffer[7] = START_BYTE;
	m_receive_buffer[8] = DUMMY_CMD;
	m_receive_buffer[9] = 0;

	// check received data
	protocol_c::package_s received;
	memset(&received, 0x00, sizeof(received));

	m_protocol.waitfor_package(received);

	ASSERT_EQ(DUMMY_CMD, received.cmd);
	ASSERT_EQ(0, received.length);
}

TEST_F(protocol_test, sync)
{
	protocol_c m_protocol(&this->transmit, &this->receive);
	init();

	memset(m_transmit_buffer, 0xFF, BUFFER_LEN);

	m_protocol.sync();

	for (uint8_t i = 0; i <= protocol_c::MAX_DATA_LEN; i++) {
		ASSERT_EQ(START_BYTE, m_transmit_buffer[i]);
	}

	ASSERT_EQ(0xFF, m_transmit_buffer[protocol_c::MAX_DATA_LEN + 1]);
}