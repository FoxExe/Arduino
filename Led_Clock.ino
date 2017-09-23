/*
 Name:		Led_Clock.ino
 Desc:		LCD 1602A + DHT22 + DS1307 RTC and buttons for control this stuff.
 Created:	9/23/2017 7:55:34 PM
 Author:	Fox
*/

#include <LiquidCrystal.h>	// Internal lib (Arduino studio)
#include "RTClib.h"		// https://github.com/adafruit/RTClib
#include "DHT.h"		// https://github.com/adafruit/DHT-sensor-library

#define LCD_UPDATE_DELAY 1000
#define DHT_UPDATE_DELAY 5000

#define BTN_LEFT	A0
#define BTN_RIGHT	A1
#define BTN_OK		A2

#define DHTPIN 4
#define DHTTYPE DHT22

RTC_DS1307 rtc;
LiquidCrystal lcd(9, 10, 5, 6, 7, 8);
DHT dht(DHTPIN, DHTTYPE);

unsigned long lcd_next_update;	// Millis() max
unsigned long btn_next_update;
unsigned long dht_next_update;
unsigned char EditPosition = 0;	// uint8_t = 0...255
unsigned char blinkCount = 0;
char lcdBuff[16];

char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };


// the setup function runs once when you press reset or power the board
void setup() {
	//Serial.begin(57600);

	lcd.begin(16, 2);

	if (!rtc.begin()) {
		//Serial.println("Couldn't find RTC");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("RTC Not found");
		while (1);
	}

	if (!rtc.isrunning()) {
		//Serial.println("RTC is NOT running!");
		lcd.clear();
		lcd.setCursor(0, 0);
		lcd.print("RTC Not run");

		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
	}

	pinMode(BTN_LEFT, INPUT_PULLUP);
	pinMode(BTN_RIGHT, INPUT_PULLUP);
	pinMode(BTN_OK, INPUT_PULLUP);

	lcd_next_update = LCD_UPDATE_DELAY;

	// Wait for DHT22 power up and ready
	while (millis() < 3000) {
		delay(100);
	}
}

void PrintTime() {
	DateTime now = rtc.now();

	//lcd.clear();	Do not clear, only ovewrite. (For DHT sensor)

	//sprintf(lcdBuff, "%02d/%02d/%04d", now.day(), now.month(), now.year());
	lcd.setCursor(0, 0);
	sprintf(lcdBuff, "%02d %s %04d", now.day(), months[now.month() - 1], now.year());
	lcd.print(lcdBuff);

	lcd.setCursor(2, 1);
	sprintf(lcdBuff, "%02d:%02d:%02d", now.hour(), now.minute(), now.second());
	lcd.print(lcdBuff);

	/*
	lcd.print(now.day());
	lcd.setCursor(5, 0);
	lcd.print("/");
	lcd.setCursor(6, 0);
	lcd.print(now.month());
	lcd.setCursor(8, 0);
	lcd.print("/");
	lcd.setCursor(9, 0);
	lcd.print(now.year());
	
	lcd.setCursor(4, 1);
	lcd.print(now.hour());
	lcd.setCursor(6, 1);
	lcd.print(":");
	lcd.setCursor(7, 1);
	lcd.print(now.minute());
	lcd.setCursor(9, 1);
	lcd.print(":");
	lcd.setCursor(10, 1);
	lcd.print(now.second());
	*/
	
	if (blinkCount > 10)
		EditPosition = 0;	// Stop cursor blink

	switch (EditPosition)
	{
		case 1: lcd.setCursor(1, 0); lcd.blink(); blinkCount++; break;
		case 2: lcd.setCursor(5, 0); lcd.blink(); blinkCount++; break;
		case 3: lcd.setCursor(10, 0); lcd.blink(); blinkCount++; break;
		case 4: lcd.setCursor(3, 1); lcd.blink(); blinkCount++; break;
		case 5: lcd.setCursor(6, 1); lcd.blink(); blinkCount++; break;
		case 6: lcd.setCursor(9, 1); lcd.blink(); blinkCount++; break;
		default: lcd.noBlink(); blinkCount = 0; break;
	}
}

void AdjustTime(int8_t state) {
	DateTime now = rtc.now();
	int adjust = 0;
	switch (EditPosition)
	{
		case 1: rtc.adjust(now + TimeSpan(state, 0, 0, 0)); break;
		case 2: 
			adjust = now.month() + state;
			if (adjust > 12)
				adjust = 1;
			if (adjust < 1)
				adjust = 12;
			rtc.adjust(DateTime(now.year(), adjust, now.day(), now.hour(), now.minute(), now.second()));
			break;
		case 3: adjust = now.year() + state;
			if (adjust > 2165)
				adjust = 2000;	// 2165 = max year for rtc
			if (adjust < 2000)
				adjust = 2165;
			rtc.adjust(DateTime(adjust, now.month(), now.day(), now.hour(), now.minute(), now.second()));
			break;
		case 4: rtc.adjust(now + TimeSpan(0, state, 0, 0)); break;
		case 5: rtc.adjust(now + TimeSpan(0, 0, state, 0)); break;
		case 6: rtc.adjust(now + TimeSpan(0, 0, 0, state)); break;
		default: break;
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (digitalRead(BTN_LEFT) == LOW)
	{
		delay(25); // debounce
		if (digitalRead(BTN_LEFT) == LOW)
		{
			AdjustTime(-1);
			blinkCount = 0;
			PrintTime();
			delay(500);
		}
	}

	if (digitalRead(BTN_RIGHT) == LOW)
	{
		delay(25); // debounce
		if (digitalRead(BTN_RIGHT) == LOW) {
			AdjustTime(+1);
			blinkCount = 0;
			PrintTime();
			delay(500);
		}
	}

	if (digitalRead(BTN_OK) == LOW)
	{
		delay(25); // debounce
		if (digitalRead(BTN_OK) == LOW) {
			EditPosition++;
			if (EditPosition > 6)
				EditPosition = 1;
			blinkCount = 0;
			PrintTime();
			delay(500);
		}
	}

	if (millis() > lcd_next_update) {
		PrintTime();
		lcd_next_update = millis() + LCD_UPDATE_DELAY;
	}

	if (millis() > dht_next_update) {
		lcd.setCursor(12, 0);
		lcd.print(dht.readTemperature());

		lcd.setCursor(12, 1);
		lcd.print(dht.readHumidity());

		dht_next_update = millis() + DHT_UPDATE_DELAY;
	}
}
