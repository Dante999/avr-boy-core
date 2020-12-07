#ifndef FONT_H
#define FONT_H

#include <stdint.h>

#ifdef __AVR_ARCH__
#	include <avr/pgmspace.h>
#endif

class font_c {
private:
	static constexpr uint8_t OFFSET_WIDTH  = 0;
	static constexpr uint8_t OFFSET_HEIGHT = 1;
	static constexpr uint8_t OFFSET_DATA   = 2;

public:
	static uint8_t width(const uint8_t font[]);
	static uint8_t height(const uint8_t font[]);
	static uint8_t byte(const uint8_t font[], uint16_t index);
};

inline uint8_t font_c::width(const uint8_t font[])
{
#ifdef __AVR_ARCH__
	return pgm_read_byte(font[OFFSET_WIDTH]);
#else
	return font[OFFSET_WIDTH];
#endif
}

inline uint8_t font_c::height(const uint8_t font[])
{
#ifdef __AVR_ARCH__
	return pgm_read_byte(font[OFFSET_HEIGHT]);
#else
	return font[OFFSET_HEIGHT];
#endif
}

inline uint8_t font_c::byte(const uint8_t font[], uint16_t index)
{
#ifdef __AVR_ARCH__
	return pgm_read_byte(&(font[OFFSET_DATA + index]));
#else
	return font[OFFSET_DATA + index];
#endif
}

#endif // FONT_H
