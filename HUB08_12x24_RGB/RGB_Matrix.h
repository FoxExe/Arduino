#pragma once

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define abs(a) (((a) < 0) ? -(a) : (a))
#define swap(a, b)     \
	{                  \
		uint8_t t = a; \
		a = b;         \
		b = t;         \
	}


#define PANEL_SIZE_X	24
#define PANEL_SIZE_Y	12
#define PANEL_SCAN_N	3

class RGB_Matrix
{
public:

	// Supported colors
	enum Colors
	{
		// Color_name	// HEX  = --RGB (Byte, RGB-111, 8 colors)
		COLOR_BLACK,	// 0x00 = B0000
		COLOR_BLUE,		// 0x01 = B0001
		COLOR_GREEN,	// 0x02 = B0010
		COLOR_CYAN,		// 0x03 = B0011
		COLOR_RED,		// 0x04 = B0100
		COLOR_MAGENTA,	// 0x05 = B0101
		COLOR_YELLOW,	// 0x06 = B0110
		COLOR_WHITE,	// 0x07 = B0111
	};

	struct Font
	{
		uint8_t char_width;
		uint8_t char_height;
		const unsigned char * font;
	};

	Font _font;

	RGB_Matrix();
	~RGB_Matrix();
	
	void init(uint8_t clk, uint8_t lat, uint8_t oe, uint8_t line_a, uint8_t line_b, uint8_t pin_r, uint8_t pin_g, uint8_t pin_b);
	void drawFrame();
	void setFont(uint8_t w, uint8_t h, const unsigned char * font);
	bool setCursor(unsigned int x, unsigned int y);
	void setBrightness(uint8_t brightness);
	void drawPixel(unsigned int x, unsigned int y, Colors color);
	void drawChar(unsigned int x, unsigned int y, char c, Colors color);
	void drawString(unsigned int x, unsigned int y, char *string, Colors color);
	void drawString(unsigned int x, unsigned int y, const char *string, Colors color);
	void drawImage(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char * bitmap, Colors color);

	void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, Colors color);
	void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, Colors color);
	void drawFillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, Colors color);
	void drawCircle(unsigned int x, unsigned int y, unsigned int r, Colors color);
	void drawFillCircle(unsigned int x, unsigned int y, unsigned int r, Colors color);
private:
	uint8_t _clk;
	uint8_t _lat;
	uint8_t _oe;
	uint8_t _line_a;
	uint8_t _line_b;
	uint8_t _pin_r;
	uint8_t _pin_g;
	uint8_t _pin_b;
	uint8_t _brightness = 25;	// by default

	unsigned int _cursor_x;
	unsigned int _cursor_y;

	void _drawChar(char c, Colors color);

	uint8_t frame_buffer_r[PANEL_SIZE_X * PANEL_SIZE_Y / 8];
	uint8_t frame_buffer_g[PANEL_SIZE_X * PANEL_SIZE_Y / 8];
	uint8_t frame_buffer_b[PANEL_SIZE_X * PANEL_SIZE_Y / 8];

};

