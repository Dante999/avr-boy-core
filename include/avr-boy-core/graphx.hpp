#ifndef GRAPHX_HPP
#define GRAPHX_HPP

#include <stdint.h>

#include "font.hpp"

class graphx_c {

public:
	enum color_e { PIXEL_OFF, PIXEL_ON, PIXEL_TOGGLE };

	static constexpr uint8_t  width  = 128;
	static constexpr uint8_t  height = 64;
	static constexpr uint16_t length = width * (height / 8);

	static constexpr uint8_t min_x = 0;
	static constexpr uint8_t min_y = 0;
	static constexpr uint8_t max_x = width - 1;
	static constexpr uint8_t max_y = height - 1;

private:
	uint8_t m_buffer[length] = {0x00};

private:
	uint16_t get_index(uint8_t x, uint8_t y);

	void    draw_byte(uint8_t x, uint8_t y, uint8_t byte);
	uint8_t get_byte(uint8_t x, uint8_t y);

public:
	uint8_t get_pixel(uint8_t x, uint8_t y);

	void get_tile(uint8_t x, uint8_t y, uint8_t *tile, uint8_t w,
	              uint8_t h);

	void fill(uint8_t color);
	void draw_char(const uint8_t font[], uint8_t x, uint8_t y,
	               const char c);
	void draw_string(const uint8_t font[], uint8_t x, uint8_t y,
	                 const char *s);
	void draw_pixel(uint8_t x, uint8_t y, uint8_t color);
	void draw_hline(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color);
	void draw_vline(uint8_t x, uint8_t y0, uint8_t y1, uint8_t color);
	void draw_rect(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1,
	               uint8_t color);
	void draw_tile(uint8_t x, uint8_t y, const uint8_t *tile, uint8_t w,
	               uint8_t h);
};

#endif