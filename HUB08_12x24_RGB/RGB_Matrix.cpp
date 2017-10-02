#include "RGB_Matrix.h"

#define PANEL_SCAN_N	3
#define BitSetFast(b, num, val) { b ^= (-val ^ b) & (1 << num); }

#define swap(a, b)     \
	{                  \
		uint8_t t = a; \
		a = b;         \
		b = t;         \
	}

RGB_Matrix::RGB_Matrix(uint8_t width, uint8_t height)
{
	_drawSizeX = width;
	_drawSizeY = height;
	unsigned int arraySize = width / 8 * height;	// Compress every 8 horisontal pixels to 1 byte (8 bit)

	_frame_buffer_r = new uint8_t[arraySize]{ 0, };
	_frame_buffer_g = new uint8_t[arraySize]{ 0, };
	_frame_buffer_b = new uint8_t[arraySize]{ 0, };
}


RGB_Matrix::~RGB_Matrix()
{
}

uint8_t RGB_Matrix::Width()
{
	return _drawSizeX;
}

uint8_t RGB_Matrix::Height()
{
	return _drawSizeY;
}

//void RGB_Matrix::init(uint8_t clk, uint8_t lat, uint8_t oe, uint8_t line_a, uint8_t line_b, uint8_t pin_r, uint8_t pin_g, uint8_t pin_b)
void RGB_Matrix::init()
{
	/*
	
	_clk = clk;
	_lat = lat;
	_oe = oe;
	_line_a = line_a;
	_line_b = line_b;
	_pin_r = pin_r;
	_pin_g = pin_g;
	_pin_b = pin_b;
	*/

	_clk = 12;
	_lat = 11;
	_oe = 13;
	_line_a = 7;
	_line_b = 6;
	_pin_r = 8;
	_pin_g = 9;
	_pin_b = 10;


	// Configure ports
	// TODO: Use registers for speed!
	pinMode(_clk, OUTPUT);
	pinMode(_lat, OUTPUT);
	pinMode(_oe, OUTPUT);
	pinMode(_line_a, OUTPUT);
	pinMode(_line_b, OUTPUT);
	pinMode(_pin_r, OUTPUT);
	pinMode(_pin_g, OUTPUT);
	pinMode(_pin_b, OUTPUT);

	digitalWrite(_pin_r, LOW);
	digitalWrite(_pin_g, LOW);
	digitalWrite(_pin_b, LOW);

	digitalWrite(_oe, HIGH);
	digitalWrite(_lat, LOW);
	digitalWrite(_clk, LOW);

	// PORTB = Ports: -, -, 13, 12, 11, 10, 9, 8	(Digital)
	// PORTC = Ports: -, -, A5, A4, A3, A2, A1, A0	(Analog)
	// PORTD = Ports: 7, 6, 5, 4, 3, 2, 1, 0		(Digital)
	// DDRB -> Data Direction Register B (Set Input/Output mode)
	//DDRD  |= B11000000;		// Set 6 and 7 to output
	//PORTD &= ~B11000000;	// Set 6 and 7 LOW
	//DDRB  = B00111111;		// Set 13 to 8 output
	//PORTB = B00100000;		// Set OE to HIGH (Because its inverted)
}

void RGB_Matrix::drawFrame()
{
	uint8_t panel_x = _drawSizeX / 8; // Bacause 8 pixel in one byte

	for (uint8_t scan = 0; scan < PANEL_SCAN_N; scan++)
	{
		// Row select
		digitalWrite(_line_a, !!(scan & B00000001));
		digitalWrite(_line_b, !!(scan & B00000010));

		for (uint8_t panels_h = 0; panels_h < _drawSizeY / PANEL_SIZE_Y; panels_h++)
		{
			for (uint8_t column = panel_x; column > 0; column--)	// 3 blocks
			{
				for (uint8_t row = 0; row < PANEL_SIZE_Y / PANEL_SCAN_N; row++)
				{
					int pos = panel_x * PANEL_SIZE_Y - column - row * panel_x * PANEL_SCAN_N - (-scan + PANEL_SCAN_N - 1) * panel_x + panels_h * panel_x * PANEL_SIZE_Y;

					for (uint8_t byte = 0; byte < 8; byte++)	// 8 byte/pixel per chip
					{
						uint8_t pos2 = 1 << byte;
						//digitalWrite(_pin_r, !!(_frame_buffer_r[pos] & pos2));
						//digitalWrite(_pin_g, !!(_frame_buffer_g[pos] & pos2));
						//digitalWrite(_pin_b, !!(_frame_buffer_b[pos] & pos2));
						BitSetFast(PORTB, 0, !!(_frame_buffer_r[pos] & pos2));
						BitSetFast(PORTB, 1, !!(_frame_buffer_g[pos] & pos2));
						BitSetFast(PORTB, 2, !!(_frame_buffer_b[pos] & pos2));


						// Clock pin HIGH, LOW
						PORTB |= B00010000;
						PORTB &= ~B00010000;
					}
				}
			}
		}

		// update frame
		//digitalWrite(_lat, HIGH);
		//digitalWrite(_lat, LOW);

		// Latch pin HIGH, LOW
		PORTB |= B00001000;
		PORTB &= ~B00001000;

		//digitalWrite(_oe, LOW);

		// OE pin LOW, HIGH (Inverted)
		PORTB &= ~B00100000;
		delayMicroseconds(_brightness);
		PORTB |= B00100000;

		//digitalWrite(_oe, HIGH);
	}
}

void RGB_Matrix::setFont(uint8_t w, uint8_t h, const unsigned char * font)
{
	_font.char_width = w;
	_font.char_height = h;
	_font.font = font;
}

bool RGB_Matrix::setCursor(unsigned int x, unsigned int y)
{
	if ((x >= _drawSizeX) || (y >= _drawSizeY))
		return false;
	else {
		_cursor_x = x;
		_cursor_y = y;
		return true;
	}
}

// Set brightness. Less = Bright. 255 max.
void RGB_Matrix::setBrightness(uint8_t brightness)
{
	_brightness = brightness;
}

void RGB_Matrix::drawPixel(unsigned int x, unsigned int y, Color color)
{
	int position = y * _drawSizeX / 8 + x / 8;

	if (!!(color & red))
		_frame_buffer_r[position] |= B00000001 << x % 8;
	else
		_frame_buffer_r[position] &= ~(B00000001 << x % 8);

	if (!!(color & green))
		_frame_buffer_g[position] |= B00000001 << x % 8;
	else
		_frame_buffer_g[position] &= ~(B00000001 << x % 8);

	if (!!(color & blue))
		_frame_buffer_b[position] |= B00000001 << x % 8;
	else
		_frame_buffer_b[position] &= ~(B00000001 << x % 8);
}

void RGB_Matrix::drawChar(unsigned int x, unsigned int y, char c, Color color)
{
	if ((y + _font.char_height) >= _drawSizeY)
		return;

	if ((x + _font.char_width) >= _drawSizeX)
		return;

	uint8_t i, j;
	for (i = 0; i < _font.char_width; i++)
	{
		uint8_t d = pgm_read_ptr(_font.font + (c * _font.char_width) + i);
		uint8_t j;
		for (j = 0; j < 8; j++)
		{
			if (d & (B00000001 << j))
			{
				drawPixel(x + i, y + j, color);
			}
			else
			{
				drawPixel(x + i, y + j, black);
			}
		}
	}

	// Add bottom padding (Horisontal blank line)
	for (j = 0; j < 8; j++)
	{
		drawPixel(x + _font.char_width, y + j, black);
	}
}

void RGB_Matrix::drawString(unsigned int x, unsigned int y, const char *string, Color color)
{
	if (!setCursor(x, y))
		return;

	while (1)
	{
		char c = (*string++);
		if (!c)
			return;
		_drawChar(c, color);
	}
}

void RGB_Matrix::drawString(unsigned int x, unsigned int y, char *string, Color color)
{
	if (!setCursor(x, y))
		return;

	while (*string)
	{
		_drawChar(*string++, color);
	}
}

void RGB_Matrix::_drawChar(char c, Color color)
{
	if (c == '\n' || c == '\r')
	{
		_cursor_x = 0;
		_cursor_y += _font.char_height;
		if (_cursor_y > _drawSizeY)
			_cursor_y = 0;
	}
	else
	{
		drawChar(_cursor_x, _cursor_y, c, color);
		_cursor_x += _font.char_width + 1;

		if (_cursor_x + _font.char_width > _drawSizeX)
		{
			_cursor_x = 0;
			_cursor_y += _font.char_height;
		}
		if (_cursor_y > _drawSizeY)
			_cursor_y = 0;
	}
}


void RGB_Matrix::drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, Color color)
{
	uint8_t steep = abs(y2 - y1) > abs(x2 - x1);
	if (steep)
	{
		swap(x1, y1);
		swap(x2, y2);
	}

	if (x1 > x2)
	{
		swap(x1, x2);
		swap(y1, y2);
	}

	uint8_t dx, dy;
	dx = x2 - x1;
	dy = abs(y2 - y1);

	int8_t err = dx / 2;
	int8_t ystep;

	if (y1 < y2)
	{
		ystep = 1;
	}
	else
	{
		ystep = -1;
	}

	for (; x1 <= x2; x1++)
	{
		if (steep)
		{
			drawPixel(y1, x1, color);
		}
		else
		{
			drawPixel(x1, y1, color);
		}
		err -= dy;
		if (err < 0)
		{
			y1 += ystep;
			err += dx;
		}
	}
}

void RGB_Matrix::drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, Color color)
{
	uint8_t i;
	for (i = x; i < x + w; i++)
	{
		drawPixel(i, y, color);
		drawPixel(i, y + h - 1, color);
	}
	for (i = y; i < y + h; i++)
	{
		drawPixel(x, i, color);
		drawPixel(x + w - 1, i, color);
	}
}

void RGB_Matrix::drawFillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, Color color)
{
	uint8_t row, col;
	for (row = y; row < y + h; row++)
	{
		for (col = x; col < x + w; col++)
		{
			drawPixel(col, row, color);
		}
	}
}

void RGB_Matrix::drawCircle(unsigned int x, unsigned int y, unsigned int r, Color color)
{
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t tmp_x = 0;
	int8_t tmp_y = r;

	drawPixel(x, y + r, color);
	drawPixel(x, y - r, color);
	drawPixel(x + r, y, color);
	drawPixel(x - r, y, color);

	while (tmp_x < tmp_y)
	{
		if (f >= 0)
		{
			tmp_y--;
			ddF_y += 2;
			f += ddF_y;
		}
		tmp_x++;
		ddF_x += 2;
		f += ddF_x;

		drawPixel(x + tmp_x, y + tmp_y, color);
		drawPixel(x - tmp_x, y + tmp_y, color);
		drawPixel(x + tmp_x, y - tmp_y, color);
		drawPixel(x - tmp_x, y - tmp_y, color);

		drawPixel(x + tmp_y, y + tmp_x, color);
		drawPixel(x - tmp_y, y + tmp_x, color);
		drawPixel(x + tmp_y, y - tmp_x, color);
		drawPixel(x - tmp_y, y - tmp_x, color);
	}
}

void RGB_Matrix::drawFillCircle(unsigned int x, unsigned int y, unsigned int r, Color color)
{
	int8_t f = 1 - r;
	int8_t ddF_x = 1;
	int8_t ddF_y = -2 * r;
	int8_t tmp_x = 0;
	int8_t tmp_y = r;
	uint8_t i;

	for (i = y - r; i <= y + r; i++)
	{
		drawPixel(x, i, color);
	}

	while (tmp_x < tmp_y)
	{
		if (f >= 0)
		{
			tmp_y--;
			ddF_y += 2;
			f += ddF_y;
		}
		tmp_x++;
		ddF_x += 2;
		f += ddF_x;

		for (i = y - tmp_y; i <= y + tmp_y; i++)
		{
			drawPixel(x + tmp_x, i, color);
			drawPixel(x - tmp_x, i, color);
		}
		for (i = y - tmp_x; i <= y + tmp_x; i++)
		{
			drawPixel(x + tmp_y, i, color);
			drawPixel(x - tmp_y, i, color);
		}
	}
}

void RGB_Matrix::dumpBuffer(char color)
{
	Serial.println(F("################"));

	uint8_t x_size = _drawSizeX / 8;
	for (uint8_t row = 0; row < _drawSizeY; row++)
	{
		for (uint8_t col = 0; col < x_size; col++)
		{
			for (uint8_t byte = 0; byte < 8; byte++)
			{
				//Serial.print(color);
				switch (color)
				{
					case 'r': Serial.print(!!(_frame_buffer_r[col + x_size * row] & (1 << byte))); break;
					case 'g': Serial.print(!!(_frame_buffer_g[col + x_size * row] & (1 << byte))); break;
					case 'b': Serial.print(!!(_frame_buffer_b[col + x_size * row] & (1 << byte))); break;
					default: break;
				}
				
			}
			
		}
		Serial.println();
	}

	Serial.println(F("\n################"));
}
