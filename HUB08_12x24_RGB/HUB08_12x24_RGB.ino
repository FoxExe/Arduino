/*
	Name:		HUB08_12x24_RGB.ino
	Created:	9/29/2017 8:19:44 PM
	Author:	Fox

	HUB08 pinouts P13.33-3S-1R1G1B-24X12
	GND		A
	GND		B
	GND		C
	OE		D
	RA1		G1
	B1		RB1
	GND		LAT
	GND		CLK

	RGB-565:
	RRRR RGGG GGGB BBBB

	RGB233
	RRGG GBBB

*/

//#define DEBUG
#define DEBUG_REPORT 1000
#define TIMER_DELAY  1000


#include "RGB_Matrix.h"
#include "font.h"

#define PIN_CLK		3
#define PIN_LATCH	4
#define PIN_OE		5
#define PIN_LINE_A	10
#define PIN_LINE_B	9
#define PIN_RED		8
#define PIN_GREEN	6
#define PIN_BLUE	7

RGB_Matrix panel(PANEL_SIZE_X * 2, PANEL_SIZE_Y);	// Panels size

#ifdef DEBUG
unsigned long fps_delay = DEBUG_REPORT;
unsigned long fps_last = 0;
unsigned long fps_start, fps_stop;
unsigned int fps_count = 0;
#endif // DEBUG

unsigned long timer_delay = TIMER_DELAY;

void setup() {
#ifdef DEBUG
	Serial.begin(115200);
#endif // DEBUG

	panel.init(PIN_CLK, PIN_LATCH, PIN_OE, PIN_LINE_A, PIN_LINE_B, PIN_RED, PIN_GREEN, PIN_BLUE);
	panel.setFont(5, 8, font5x8);

	/*
	// Color test pattern
	panel.drawFillRect(0, 0, 8, 6, panel.COLOR_RED);
	panel.drawFillRect(8, 0, 8, 6, panel.COLOR_GREEN);
	panel.drawFillRect(16, 0, 8, 6, panel.COLOR_BLUE);
	panel.drawFillRect(0, 6, 8, 6, panel.COLOR_CYAN);
	panel.drawFillRect(8, 6, 8, 6, panel.COLOR_MAGENTA);
	panel.drawFillRect(16, 6, 8, 6, panel.COLOR_YELLOW);
	
	panel.drawFillRect(24, 0, 8, 6, panel.COLOR_RED);
	panel.drawFillRect(24, 6, 8, 6, panel.COLOR_WHITE);

	//panel.drawRect(2, 1, 19, 9, panel.COLOR_BLACK);

	//panel.drawChar(3, 2, 'F', panel.COLOR_WHITE);
	//panel.drawChar(9, 2, 'o', panel.COLOR_WHITE);
	//panel.drawChar(15, 2, 'X', panel.COLOR_WHITE);
	//char * str1 = "123";
	//panel.drawString(0, 0, "1234", panel.COLOR_GREEN);
	//panel.drawString(24, 0, "456", panel.COLOR_GREEN);
	*/

	//panel.drawPixel(0, 0, panel.green);
	//panel.drawPixel(0, 1, panel.green);
	//panel.drawPixel(1, 0, panel.green);
	//panel.drawPixel(25, 3, panel.green);
	/*
	panel.drawFillRect(0, 0, 8, PANEL_SIZE_Y, panel.yellow);
	panel.drawFillRect(8, 0, 8, PANEL_SIZE_Y, panel.magenta);
	panel.drawFillRect(16, 0, 8, PANEL_SIZE_Y, panel.cyan);
	panel.drawFillRect(24, 0, 8, PANEL_SIZE_Y, panel.red);
	panel.drawFillRect(32, 0, 8, PANEL_SIZE_Y, panel.green);
	panel.drawFillRect(40, 0, 8, PANEL_SIZE_Y, panel.blue);
	*/

	//panel.drawFillRect(0, 0, PANEL_SIZE_X, PANEL_SIZE_Y, panel.white);
	//panel.drawPixel(PANEL_SIZE_X - 1, PANEL_SIZE_Y - 1, panel.black);

	//panel.drawRect(1, 1, PANEL_SIZE_X - 2, PANEL_SIZE_Y - 2, panel.black);
	panel.drawRect(0, 0, panel.Width(), panel.Height(), panel.blue);
}

void loop() {

	if (millis() > timer_delay) {
		char buffer[8];
		panel.drawString(12, 2, itoa(millis() / 1000, buffer, 10), panel.white);

		timer_delay = millis() + TIMER_DELAY - (millis() % TIMER_DELAY);
	}



#ifdef DEBUG
	fps_start = millis();
#endif // DEBUG

	panel.drawFrame();

#ifdef DEBUG
	fps_stop = millis();
	fps_count++;

	if (millis() > fps_delay) {
		Serial.print(fps_count);
		Serial.print(F(" FPS, "));
		Serial.print(((fps_stop - fps_start) + fps_last) / 2);
		Serial.print(F(" ms. Uptime: "));
		Serial.print(millis() / 1000);
		Serial.print(F(" sec\n"));

		//panel.dumpBuffer('r');	// DEBUG
		//panel.dumpBuffer('g');	// DEBUG
		panel.dumpBuffer('b');	// DEBUG

		fps_count = 0;
		fps_delay = millis() + DEBUG_REPORT - (millis() % DEBUG_REPORT);	// Better delay. ~1 second.
	}
#endif // DEBUG
}