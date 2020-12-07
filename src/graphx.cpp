#include "avr-boy-core/graphx.hpp"

#include <string.h> // strlen()

uint16_t graphx_c::get_index(uint8_t x, uint8_t y)
{
	const uint16_t result = x + (y / 8u) * width;

	return (result > length) ? length : result;
}

uint8_t graphx_c::get_pixel(uint8_t x, uint8_t y)
{
	const uint8_t  y_bit = y % 8;
	const uint16_t index = get_index(x, y);

	return (m_buffer[index] & (1 << y_bit)) ? PIXEL_ON : PIXEL_OFF;
}

void graphx_c::draw_pixel(uint8_t x, uint8_t y, uint8_t color)
{
	const uint8_t  y_bit = y % 8;
	const uint16_t index = get_index(x, y);

	if (color == PIXEL_ON)
		m_buffer[index] |= (1u << y_bit);
	else if (color == PIXEL_TOGGLE)
		m_buffer[index] ^= (1 << y_bit);
	else
		m_buffer[index] &= ~(1 << y_bit);
}

void graphx_c::draw_byte(uint8_t x, uint8_t y, uint8_t byte)
{
	for (uint8_t j = 0; j < 8; j++) {

		uint8_t color = (byte & (1 << j)) ? PIXEL_ON : PIXEL_OFF;

		draw_pixel(x, y + j, color);
	}
}

uint8_t graphx_c::get_byte(uint8_t x, uint8_t y)
{
	uint8_t byte = 0x00;

	for (uint8_t j = 0; j < 8; j++) {

		if (get_pixel(x, y + j) == PIXEL_ON) {
			byte |= (1 << j);
		}
	}

	return byte;
}

void graphx_c::draw_char(const uint8_t font[], uint8_t x, uint8_t y,
                         const char c)
{
	const uint8_t font_width  = font_c::width(font);
	const uint8_t font_height = font_c::height(font);

	uint8_t  char_index  = static_cast<uint8_t>(c - 0x20);
	uint16_t start_index = char_index * font_width;

	for (uint8_t row = 0; row < (font_height / 8); row++) {

		uint16_t row_offset = row * font_width;

		for (uint8_t col = 0; col < font_width; col++) {

			uint16_t i_new = start_index + row_offset + col;
			uint8_t  x_new = x + col;
			uint8_t  y_new = y + (row * 8);

			draw_byte(x_new, y_new, font_c::byte(font, i_new));
		}
	}
}

void graphx_c::draw_string(const uint8_t font[], uint8_t x, uint8_t y,
                           const char *s)
{
	const uint8_t font_width = font_c::width(font);

	for (uint8_t i = 0; i < strlen(s); i++) {
		draw_char(font, x + (i * (font_width + 1)), y, s[i]);
	}
}

void graphx_c::draw_hline(uint8_t x0, uint8_t x1, uint8_t y, uint8_t color)
{
	uint8_t delta = x1 - x0;

	for (uint8_t i = 0; i <= delta; i++) {
		draw_pixel(x0 + i, y, color);
	}
}

void graphx_c::draw_vline(uint8_t x, uint8_t y0, uint8_t y1, uint8_t color)
{
	uint8_t delta = y1 - y0;

	for (uint8_t i = 0; i <= delta; i++) {
		draw_pixel(x, y0 + i, color);
	}
}

void graphx_c::draw_rect(uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1,
                         uint8_t color)
{
	draw_hline(x0, x1, y0, color);
	draw_hline(x0, x1, y1, color);
	draw_vline(x0, y0, y1, color);
	draw_vline(x1, y0, y1, color);
}

void graphx_c::draw_tile(uint8_t x, uint8_t y, const uint8_t *tile, uint8_t w,
                         uint8_t h)
{

	for (uint8_t row = 0; row < (h / 8); row++) {

		for (uint8_t col = 0; col < w; col++) {

			const uint16_t offset = row * w;
			const uint8_t  data   = tile[offset + col];
			const uint8_t  x_new  = x + col;
			const uint8_t  y_new  = y + (row * 8);

			draw_byte(x_new, y_new, data);
		}
	}
}

void graphx_c::get_tile(uint8_t x, uint8_t y, uint8_t *tile, uint8_t w,
                        uint8_t h)
{

	for (uint8_t row = 0; row < (h / 8); row++) {

		for (uint8_t col = 0; col < w; col++) {

			const uint16_t offset = row * w;
			const uint8_t  x_new  = x + col;
			const uint8_t  y_new  = y + (row * 8);

			tile[offset + col] = get_byte(x_new, y_new);
		}
	}
}

void graphx_c::fill(uint8_t color)
{
	if (color == PIXEL_ON) {
		memset(m_buffer, 0xFF, length);
	}
	else {
		memset(m_buffer, 0x00, length);
	}
}