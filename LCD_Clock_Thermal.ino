/*
 Name:		LCD_Clock_Thermal.ino
 Created:	06.11.2017 20:58:01
 Author:	Allester Fox
 Descr:		Show date/time and data from thermal sensor (DHT22) on SSD1306 LCD screen
 Usage:		Press OK for edit/select mode (On screen you will see what you select now)
			By pressing Up/Down buttons - change selected value
			In normal mode Up/Down will change thermal data Celceus/Fahrenheit
 Note:		Some code commented out - Its for 16x2 lcd screen
*/

//#include <LiquidCrystal.h>
#include <Adafruit_SSD1306.h>
#include <RTClib.h>		// https://github.com/adafruit/RTClib
#include <DHT.h>		// https://github.com/adafruit/DHT-sensor-library

#define LCD_UPDATE_DELAY	1000
#define DHT_UPDATE_DELAY	5000
bool ShowInFahrenheit = false;	// By default

#define BTN_UP		4
#define BTN_DOWN	3
#define BTN_OK		2

#define DHTPIN		PIN_A0
#define DHTTYPE		DHT22

// rs, enable, d0, d1, d2, d3
//LiquidCrystal lcd(7, 6, 5, 4, 3, 2);
Adafruit_SSD1306 display(11, 12, 9, 10, 8);

RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);

unsigned long lcd_next_update;	// Millis() max
unsigned long btn_next_update;
unsigned long dht_next_update;
uint8_t EditPosition = 0;	// uint8_t = 0...255
uint8_t blinkCount = 0;
char lcdBuff[16];
float temperature, humidity;

const char* months[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
const char* setting[] = { "day", "month", "year", "hour", "min", "sec" };

// the setup function runs once when you press reset or power the board
void setup() {
	display.begin();
	display.clearDisplay();
	display.setCursor(0, 0);
	display.setTextColor(WHITE);
	display.setTextSize(1);
	display.println(F("[LCD] OK"));
	display.display();

	//lcd.begin(16, 2);
	//lcd.clear();
	//lcd.print(F("Loading..."));

	display.print(F("[RTC] "));
	display.display();

	if (!rtc.begin()) {
		//lcd.clear();
		//lcd.setCursor(0, 0);
		//lcd.print(F("RTC Not found"));
		display.println(F("Error!"));
		display.display();
		while (1);
	}

	if (!rtc.isrunning()) {
		//Serial.println("RTC is NOT running!");
		//lcd.clear();
		//lcd.setCursor(0, 0);
		//lcd.print(F("RTC Not run"));
		display.println(F("Not run!"));
		display.display();
		while (1);
	}

	// Only for RTC_DS3231
	/*
	if (rtc.lostPower()) {
		display.println(F("No power"));
		display.println(F("> Adjusting RTC"));
		display.display();
		rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
		// rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
	}
	*/

	display.println(F("OK"));
	display.display();

	display.print(F("[BTN] "));
	display.display();

	pinMode(BTN_UP, INPUT_PULLUP);
	pinMode(BTN_DOWN, INPUT_PULLUP);
	pinMode(BTN_OK, INPUT_PULLUP);

	display.println(F("OK"));
	display.display();

	lcd_next_update = LCD_UPDATE_DELAY;

	display.print(F("[DHT] "));
	display.display();
	// Wait for DHT22 power up and ready
	while (millis() < 3000)
		delay(100);

	temperature = dht.readTemperature(ShowInFahrenheit);
	humidity = dht.readHumidity();

	display.println(F("OK"));
	display.display();
	delay(1000);
}

void PrintTime() {
	DateTime now = rtc.now();

	//lcd.clear();
	display.clearDisplay();

	//sprintf(lcdBuff, "%02d/%02d/%04d", now.day(), now.month(), now.year());
	//lcd.setCursor(0, 0);
	sprintf_P(lcdBuff, PSTR("%.2d %s"), now.day(), months[now.month() - 1]);
	//lcd.print(lcdBuff);
	display.setCursor(0, 0);
	display.setTextSize(2);
	display.print(lcdBuff);

	sprintf_P(lcdBuff, PSTR("%.4d"), now.year());
	display.setCursor(80, 0);
	display.setTextSize(2);
	display.print(lcdBuff);

	//lcd.setCursor(2, 1);
	sprintf_P(lcdBuff, PSTR("%.2d:%.2d"), now.hour(), now.minute());
	//lcd.print(lcdBuff);
	display.setCursor(8, 20);
	display.setTextSize(4);
	display.print(lcdBuff);

	sprintf_P(lcdBuff, PSTR("%.2d"), now.second());
	display.setCursor(61, 42);
	display.setTextSize(1);
	display.print(lcdBuff);

	//sprintf_P(lcdBuff, PSTR("Out: %.2fc, %.2f%"), temperature, humidity);
	display.setCursor(8, 56);
	display.setTextSize(1);
	//display.print(lcdBuff);
	display.print(temperature);
	display.print(ShowInFahrenheit ? F("f") : F("c"));

	//lcd.setCursor(12, 0);
	//lcd.print(temperature);

	//lcd.setCursor(12, 1);
	//lcd.print(humidity);

	if (blinkCount > 10)
		EditPosition = 0;	// Stop cursor blink
	/*
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
	*/
	switch (EditPosition)
	{
	case 1:
	case 2:
	case 3:
	case 4:
	case 5:
	case 6:
		//display.setCursor(54, 56);
		display.setTextSize(1);
		display.print(' ');
		display.print(setting[EditPosition - 1]);
		blinkCount++;
		break;
	default: blinkCount = 0; break;
	}

	display.setCursor(88, 56);
	display.print(humidity);
	display.print(F("%"));

	display.display();
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
	default:
		ShowInFahrenheit = !ShowInFahrenheit;
		dht_next_update = millis();	// Update on screen now!
		break;	// Toggle "Show temperature in fahrenheit"
	}
}

// the loop function runs over and over again until power down or reset
void loop() {
	if (digitalRead(BTN_UP) == LOW)
	{
		delay(25); // debounce
		if (digitalRead(BTN_UP) == LOW)
		{
			AdjustTime(+1);
			blinkCount = 0;
			PrintTime();
			delay(250);
		}
	}

	if (digitalRead(BTN_DOWN) == LOW)
	{
		delay(25); // debounce
		if (digitalRead(BTN_DOWN) == LOW) {
			AdjustTime(-1);
			blinkCount = 0;
			PrintTime();
			delay(250);
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
			delay(250);
		}
	}

	if (millis() > dht_next_update) {
		temperature = dht.readTemperature(ShowInFahrenheit);
		humidity = dht.readHumidity();
		dht_next_update = millis() + DHT_UPDATE_DELAY;
	}

	if (millis() > lcd_next_update) {
		PrintTime();
		lcd_next_update = millis() + LCD_UPDATE_DELAY;
	}
}