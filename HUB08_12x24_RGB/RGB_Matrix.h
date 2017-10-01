#pragma once

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#define PANEL_SIZE_X	24
#define PANEL_SIZE_Y	12

class RGB_Matrix
{
public:

	// Supported Color
	enum Color
	{
		// Color_name	// HEX  = --RGB (Byte, RGB-111, 8 Color)
		black,	// 0x00 = B0000
		blue,	// 0x01 = B0001
		green,	// 0x02 = B0010
		cyan,	// 0x03 = B0011
		red,	// 0x04 = B0100
		magenta,// 0x05 = B0101
		yellow,	// 0x06 = B0110
		white,	// 0x07 = B0111
	};

	struct Font
	{
		uint8_t char_width;
		uint8_t char_height;
		const unsigned char * font;
	};

	RGB_Matrix(uint8_t width, uint8_t height);
	~RGB_Matrix();
	
	uint8_t Width();
	uint8_t Height();

	void init(uint8_t clk, uint8_t lat, uint8_t oe, uint8_t line_a, uint8_t line_b, uint8_t pin_r, uint8_t pin_g, uint8_t pin_b);
	void drawFrame();

	void setFont(uint8_t w, uint8_t h, const unsigned char * font);
	void setBrightness(uint8_t brightness);

	bool setCursor(unsigned int x, unsigned int y);
	void drawPixel(unsigned int x, unsigned int y, Color color);
	void drawChar(unsigned int x, unsigned int y, char c, Color color);
	void drawString(unsigned int x, unsigned int y, char *string, Color color);
	void drawString(unsigned int x, unsigned int y, const char *string, Color color);
	void drawImage(unsigned int x, unsigned int y, unsigned int w, unsigned int h, char * bitmap, Color color);

	void drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, Color color);
	void drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, Color color);
	void drawFillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, Color color);
	void drawCircle(unsigned int x, unsigned int y, unsigned int r, Color color);
	void drawFillCircle(unsigned int x, unsigned int y, unsigned int r, Color color);
	void dumpBuffer(char color);
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

	uint8_t _cursor_x;
	uint8_t _cursor_y;
	uint8_t _drawSizeX;
	uint8_t _drawSizeY;

	Font _font;
	void _drawChar(char c, Color color);

	uint8_t *_frame_buffer_r;
	uint8_t *_frame_buffer_g;
	uint8_t *_frame_buffer_b;
};

