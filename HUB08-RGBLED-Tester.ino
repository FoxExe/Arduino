#define LED_PIXELS_X	8
#define LED_PIXELS_Y	6
#define LED_BLOCKS_X	3
#define LED_BLOCKS_Y	2

#define PIN_CLK			9
#define PIN_LATCH		10
#define PIN_OE			11

#define PIN_DATA_A		2
#define PIN_DATA_B		3

#define PIN_LED_R		6
#define PIN_LED_G		7
#define PIN_LED_B		8


static unsigned long timeCounter = 0;
uint8_t colors = 1;
uint8_t blocks = 1;
uint8_t r, g, b = 0;

void setup()
{
	Serial.begin(9600);

	pinMode(PIN_CLK, OUTPUT);
	pinMode(PIN_LATCH, OUTPUT);
	pinMode(PIN_OE, OUTPUT);
	pinMode(PIN_DATA_A, OUTPUT);
	pinMode(PIN_DATA_B, OUTPUT);
	pinMode(PIN_LED_R, OUTPUT);
	pinMode(PIN_LED_G, OUTPUT);
	pinMode(PIN_LED_B, OUTPUT);

	digitalWrite(PIN_LED_R, LOW);
	digitalWrite(PIN_LED_G, LOW);
	digitalWrite(PIN_LED_B, LOW);

	digitalWrite(PIN_OE, LOW);
	digitalWrite(PIN_OE, HIGH);
	digitalWrite(PIN_LATCH, HIGH);
}

void loop()
{
	if ((millis() - timeCounter) > 1000) {
		timeCounter = millis();

		Serial.println(colors);

		switch (colors)
		{
		case 1:
			r = HIGH;
			g = LOW;
			b = LOW;
			break;
		case 2:
			r = LOW;
			g = HIGH;
			b = LOW;
			break;
		case 3:
			r = LOW;
			g = LOW;
			b = HIGH;
			break;
		case 4:
			r = LOW;
			g = HIGH;
			b = HIGH;
			break;
		case 5:
			r = HIGH;
			g = LOW;
			b = HIGH;
			break;
		case 6:
			r = HIGH;
			g = HIGH;
			b = LOW;
			break;
		case 7:
			r = HIGH;
			g = HIGH;
			b = HIGH;
			break;
		case 8:
			r = LOW;
			g = LOW;
			b = LOW;
			break;
		}

		colors++;
		if (colors == 9)
			colors = 1;
	}

	for (uint8_t row = 0; row < 3; row++)
	{
		digitalWrite(PIN_DATA_A, !!(row & B00000001));
		digitalWrite(PIN_DATA_B, !!(row & B00000010));

		for (uint8_t col = 0; col < 96; col++)
		{
			digitalWrite(PIN_LED_R, r);
			digitalWrite(PIN_LED_G, g);
			digitalWrite(PIN_LED_B, b);

			digitalWrite(PIN_CLK, HIGH);
			digitalWrite(PIN_CLK, LOW);
		}

		digitalWrite(PIN_LATCH, HIGH);
		digitalWrite(PIN_LATCH, LOW);
		
		// Brightness. Less = bright (PWM-Like)
		digitalWrite(PIN_OE, LOW);
		delayMicroseconds(100);
		digitalWrite(PIN_OE, HIGH);
	}

	blocks++;
	if (blocks == 6) {
		blocks = 1;
		delay(10);
	}
}
