// Note: Not all arduino pins can work in PWM mode!

#define PIN_LED 9

uint8_t brightness = 0;

void setup() {
	pinMode(PIN_LED, OUTPUT);
}

void loop() {
	for (brightness = 0; brightness < 255; brightness++) {
		analogWrite(PIN_LED, brightness);
		delay(5);
	}

	for (brightness = 255; brightness > 0; brightness--) {
		analogWrite(PIN_LED, brightness);
		delay(5);
	}
}