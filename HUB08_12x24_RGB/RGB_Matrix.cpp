#include "RGB_Matrix.h"

RGB_Matrix::RGB_Matrix()
{
}


RGB_Matrix::~RGB_Matrix()
{
}

void RGB_Matrix::init(uint8_t clk, uint8_t lat, uint8_t oe, uint8_t line_a, uint8_t line_b, uint8_t pin_r, uint8_t pin_g, uint8_t pin_b)
{
	_clk = clk;
	_lat = lat;
	_oe = oe;
	_line_a = line_a;
	_line_b = line_b;
	_pin_r = pin_r;
	_pin_g = pin_g;
	_pin_b = pin_b;

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
}

void RGB_Matrix::drawFrame()
{
	for (uint8_t row = 0; row < PANEL_SCAN_N; row++)
	{
		// Row select
		digitalWrite(_line_a, !!(row & B00000001));
		digitalWrite(_line_b, !!(row & B00000010));

		for (uint8_t block = 0; block < PANEL_SCAN_N; block++)	// 3 blocks
		{
			for (uint8_t rowInBlock = 0; rowInBlock < 4; rowInBlock++)
			{
				for (uint8_t byte = 0; byte < 8; byte++)	// 8 byte/pixel per chip
				{
					int pos = (rowInBlock + 1) * PANEL_SIZE_X / 8 * PANEL_SCAN_N - row * PANEL_SIZE_X / 8 - PANEL_SIZE_X / 8 / PANEL_SCAN_N * block - 1;

					digitalWrite(_pin_r, !!(frame_buffer_r[pos] & (1 << byte)));
					digitalWrite(_pin_g, !!(frame_buffer_g[pos] & (1 << byte)));
					digitalWrite(_pin_b, !!(frame_buffer_b[pos] & (1 << byte)));

					digitalWrite(_clk, HIGH);
					digitalWrite(_clk, LOW);
				}
			}
		}

		// update frame
		digitalWrite(_lat, HIGH);
		digitalWrite(_lat, LOW);

		digitalWrite(_oe, LOW);
		delayMicroseconds(_brightness);
		digitalWrite(_oe, HIGH);
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
	if ((x >= PANEL_SIZE_X) || (y >= PANEL_SIZE_Y))
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

void RGB_Matrix::drawPixel(unsigned int x, unsigned int y, Colors color)
{
	int position = x / 8 + (y * PANEL_SIZE_X / 8);

	if (!!(color & COLOR_RED))
		frame_buffer_r[position] |= B10000000 >> x % 8;
	else
		frame_buffer_r[position] &= ~B10000000 >> x % 8;

	if (!!(color & COLOR_GREEN))
		frame_buffer_g[position] |= B10000000 >> x % 8;
	else
		frame_buffer_g[position] &= ~B10000000 >> x % 8;

	if (!!(color & COLOR_BLUE))
		frame_buffer_b[position] |= B10000000 >> x % 8;
	else
		frame_buffer_b[position] &= ~B10000000 >> x % 8;
}

void RGB_Matrix::drawChar(unsigned int x, unsigned int y, char c, Colors color)
{
	if ((y + _font.char_height) >= PANEL_SIZE_Y)
		return;

	if ((x + _font.char_width) >= PANEL_SIZE_X)
		return;

	uint8_t i, j;
	for (i = 0; i < _font.char_width; i++)
	{
		uint8_t d = pgm_read_ptr(_font.font + (c * _font.char_width) + i);
		uint8_t j;
		for (j = 0; j < 8; j++)
		{
			if (d & _BV(j))
			{
				drawPixel(x + i, y + j, color);
			}
			else
			{
				drawPixel(x + i, y + j, COLOR_BLACK);
			}
		}
	}

	// Add bottom padding (Horisontal blank line)
	for (j = 0; j < 8; j++)
	{
		drawPixel(x + _font.char_width, y + j, COLOR_BLACK);
	}
}

void RGB_Matrix::drawString(unsigned int x, unsigned int y, const char *string, Colors color)
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

void RGB_Matrix::drawString(unsigned int x, unsigned int y, char *string, Colors color)
{
	if (!setCursor(x, y))
		return;

	while (*string)
	{
		_drawChar(*string++, color);
	}
}

void RGB_Matrix::_drawChar(char c, Colors color)
{
	if (c == '\n' || c == '\r')
	{
		_cursor_x = 0;
		_cursor_y += _font.char_height;
		if (_cursor_y > PANEL_SIZE_Y)
			_cursor_y = 0;
	}
	else
	{
		drawChar(_cursor_x, _cursor_y, c, color);
		_cursor_x += _font.char_width + 1;

		if (_cursor_x + _font.char_width > PANEL_SIZE_X)
		{
			_cursor_x = 0;
			_cursor_y += _font.char_height;
		}
		if (_cursor_y > PANEL_SIZE_Y)
			_cursor_y = 0;
	}
}


void RGB_Matrix::drawLine(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2, Colors color)
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

void RGB_Matrix::drawRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, Colors color)
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

void RGB_Matrix::drawFillRect(unsigned int x, unsigned int y, unsigned int w, unsigned int h, Colors color)
{
	uint8_t i, j;
	for (i = x; i < x + w; i++)
	{
		for (j = y; j < y + h; j++)
		{
			drawPixel(i, j, color);
		}
	}
}

void RGB_Matrix::drawCircle(unsigned int x, unsigned int y, unsigned int r, Colors color)
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

void RGB_Matrix::drawFillCircle(unsigned int x, unsigned int y, unsigned int r, Colors color)
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
