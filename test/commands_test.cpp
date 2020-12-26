
#include "gtest/gtest.h"

#include <atomic>
#include <iostream>
#include <mutex>
#include <thread>

#include "avr-boy-core/cartridge.hpp"
#include "avr-boy-core/handheld.hpp"

std::atomic_bool m_data_ready{false};

static result_e m_result = result_e::RESULT_NOK;

static constexpr uint8_t BUFFER_LEN = 255;

static uint8_t m_buffer[BUFFER_LEN];
static uint8_t m_transmit_index;
static uint8_t m_receive_index;

static void reset()
{
	m_transmit_index = 0;
	m_receive_index  = 0;

	memset(m_buffer, 0x00, BUFFER_LEN);
}

static void transmit(uint8_t byte)
{
	m_buffer[m_transmit_index++] = byte;
}

static uint8_t receive()
{
	return m_buffer[m_receive_index++];
}

static void cartridge_before_transmit()
{
	reset();
	m_data_ready = false;
}

static void cartridge_after_transmit()
{
	m_data_ready = true;
	reset();
}

void thread_cartridge_send()
{
	cartridge_c cartridge(transmit, receive);

	cartridge.set_before_transmit_callback(cartridge_before_transmit);
	cartridge.set_after_transmit_callback(cartridge_after_transmit);

	m_result = cartridge.ping();
}

void thread_handheld_response()
{
	handheld_c handheld(transmit, receive);

	while (!m_data_ready.load()) {
	}

	handheld.waitfor_instructions();
}

TEST(command_test, ping_pong_roundtrip)
{
	// TODO: Fix race condition
//	std::thread thread1(thread_cartridge_send);
//	std::thread thread2(thread_handheld_response);

//	thread1.join(); // pauses until first finishes
//	thread2.join(); // pauses until second finishes

	//ASSERT_EQ(result_e::RESULT_OK, m_result);
}
