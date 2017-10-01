/*
	Name:		HUB08_12x24_RGB.ino
	Created:	9/29/2017 8:19:44 PM
	Author:	Fox

	Panel name: P13.33-3S-1R1G1B-24X12
	Panel base: 6x JXI5020 for leds and LeadASIC PR4538 as main controller
	Panel interface: HUB08
	Pinouts:
	GND		A
	GND		B
	GND		C
	OE		D
	RA1		G1
	B1		RB1
	GND		LAT
	GND		CLK
*/

//#define DEBUG
//#define DEBUG_REPORT 1000
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
	panel.setBrightness(50);	// 25 if not set (Default)
	panel.setFont(5, 8, font5x8);

	/*
	// Color test pattern
	panel.drawFillRect(0, 0, 8, 6, panel.red);
	panel.drawFillRect(8, 0, 8, 6, panel.green);
	panel.drawFillRect(16, 0, 8, 6, panel.blue);
	panel.drawFillRect(0, 6, 8, 6, panel.cyan);
	panel.drawFillRect(8, 6, 8, 6, panel.magenta);
	panel.drawFillRect(16, 6, 8, 6, panel.yellow);

	*/
	panel.drawFillRect(0, 0, panel.Width(), panel.Height(), panel.white);	// Turn all pixels ON
	panel.drawRect(1, 1, PANEL_SIZE_X - 2, PANEL_SIZE_Y - 2, panel.black);

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