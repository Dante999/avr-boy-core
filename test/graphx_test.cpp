#include "gtest/gtest.h"

#include "avr-boy-core/graphx.hpp"

class graphx_test : public ::testing::Test {
protected:
	graphx_c m_graphx;

public:
	void check_pixel(uint8_t x, uint8_t y, uint8_t color)
	{
		ASSERT_EQ(color, m_graphx.get_pixel(x, y))
		    << "coordinates: (" << static_cast<int>(x) << "|"
		    << static_cast<int>(y) << ")";
	}

	void check_hline(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color)
	{
		for (uint8_t x = x0; x <= x1; ++x) {
			check_pixel(x, y, color);
		}
	}

	void check_vline(uint8_t x, uint8_t y0, uint8_t y1, uint8_t color)
	{
		for (uint8_t y = y0; y <= y1; ++y) {
			check_pixel(x, y, color);
		}
	}
};

TEST_F(graphx_test, set_pixel_and_get_pixel)
{

	for (uint8_t x = 0; x < m_graphx.width; ++x) {

		for (uint8_t y = 0; y < m_graphx.height; ++y) {

			m_graphx.draw_pixel(x, y, m_graphx.PIXEL_OFF);
			check_pixel(x, y, m_graphx.PIXEL_OFF);

			m_graphx.draw_pixel(x, y, m_graphx.PIXEL_ON);
			check_pixel(x, y, m_graphx.PIXEL_ON);

			m_graphx.draw_pixel(x, y, m_graphx.PIXEL_TOGGLE);
			check_pixel(x, y, m_graphx.PIXEL_OFF);

			m_graphx.draw_pixel(x, y, m_graphx.PIXEL_TOGGLE);
			check_pixel(x, y, m_graphx.PIXEL_ON);
		}
	}
}

TEST_F(graphx_test, draw_hline)
{

	for (uint8_t y = 0; y < m_graphx.height; ++y) {

		for (uint8_t x = 0; x < m_graphx.width; ++x) {

			m_graphx.draw_hline(0, x, y, m_graphx.PIXEL_ON);
			check_hline(0, x, y, m_graphx.PIXEL_ON);

			m_graphx.draw_hline(0, x, y, m_graphx.PIXEL_OFF);
			check_hline(0, x, y, m_graphx.PIXEL_OFF);
		}
	}
}

TEST_F(graphx_test, draw_vline)
{

	for (uint8_t x = 0; x < m_graphx.width; ++x) {

		for (uint8_t y = 0; y < m_graphx.height; ++y) {

			m_graphx.draw_vline(x, 0, y, m_graphx.PIXEL_ON);
			check_vline(x, 0, y, m_graphx.PIXEL_ON);

			m_graphx.draw_vline(x, 0, y, m_graphx.PIXEL_OFF);
			check_vline(x, 0, y, m_graphx.PIXEL_OFF);
		}
	}
}