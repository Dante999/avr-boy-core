#ifndef FONT_H
#define FONT_H

#include <cstdint>

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

#ifdef __AVR_ARCH__
inline const uint8_t font_c::width(const struct font_bytes *f)
{
	return pgm_read_byte(font[OFFSET_WIDTH]);
}

inline const uint8_t font_c::height(const struct font_bytes *f)
{
	return pgm_read_byte(font[OFFSET_HEIGHT]);
}

inline const uint8_t font_c::byte(const struct font_bytes *f, uint16_t index)
{
	return pgm_read_byte(&(font[OFFSET_DATA + index]));
}
#else

inline uint8_t font_c::width(const uint8_t font[])
{
	return font[OFFSET_WIDTH];
}

inline uint8_t font_c::height(const uint8_t font[])
{
	return font[OFFSET_HEIGHT];
}

inline uint8_t font_c::byte(const uint8_t font[], uint16_t index)
{
	return font[OFFSET_DATA + index];
}
#endif

#endif // FONT_H
