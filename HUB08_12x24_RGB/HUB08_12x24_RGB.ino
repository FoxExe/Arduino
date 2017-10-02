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

	~470 FPS with 1 module
	~248 FPS with 2 modules

	TODO:
	 * Draw frame by timer (in interrupt).
	 * Option: When we have some work - swith update frame to interrupt mode, but dont send data, only A/B/OE swith
	 * Add back buffer?
	 * Add more colors (RGB565 format for 65535 colors). How to do that?
*/

//#define DEBUG
#define DEBUG_REPORT 1000
#define TIMER_DELAY  1000


#include "RGB_Matrix.h"
#include "font.h"

#include <RTClib.h>
RTC_Millis rtc;

#define PIN_OE		13
#define PIN_CLK		12
#define PIN_LATCH	11
#define PIN_LINE_A	7
#define PIN_LINE_B	6
#define PIN_RED		8
#define PIN_GREEN	9
#define PIN_BLUE	10

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

	//panel.init(PIN_CLK, PIN_LATCH, PIN_OE, PIN_LINE_A, PIN_LINE_B, PIN_RED, PIN_GREEN, PIN_BLUE);
	panel.init();
	panel.setBrightness(20);	// 25 if not set (Default)
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
	//panel.drawFillRect(0, 0, panel.Width(), panel.Height(), panel.red);	// Turn all pixels ON
	//panel.drawRect(1, 1, panel.Width() - 2, panel.Height() - 2, panel.black);

	panel.drawRect(0, 0, panel.Width(), panel.Height(), panel.blue);

	// Set date/time to scketch upload date
	rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {

	if (millis() > timer_delay) {
		//char buffer[8];
		//panel.drawString(12, 2, itoa(millis() / 1000, buffer, 10), panel.green);

		DateTime now = rtc.now();
		char buffer[8];
		sprintf(buffer, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
		panel.drawString(0, 2, buffer, panel.green);

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
		//panel.dumpBuffer('b');	// DEBUG

		fps_count = 0;
		fps_delay = millis() + DEBUG_REPORT - (millis() % DEBUG_REPORT);	// Better delay. ~1 second.
	}
#endif // DEBUG
}