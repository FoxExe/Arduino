#define PIN_CLK			9
#define PIN_LATCH		10
#define PIN_OE			11

#define PIN_DATA_A		2
#define PIN_DATA_B		3
//#define PIN_DATA_C		4	// Not present
// #define PIN_DATA_D		5	// Not present

#define PIN_LED_R		6
#define PIN_LED_G		7
#define PIN_LED_B		8

void setup()
{
	pinMode(PIN_CLK, OUTPUT);
	pinMode(PIN_LATCH, OUTPUT);
	pinMode(PIN_OE, OUTPUT);
	pinMode(PIN_DATA_A, OUTPUT);
	pinMode(PIN_DATA_B, OUTPUT);
	//pinMode(PIN_DATA_C, OUTPUT);	// Not present
	//pinMode(PIN_DATA_D, OUTPUT);	// Not present
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
	for (uint8_t row = 0; row < 3; row++)
	{
		digitalWrite(PIN_DATA_A, !!(row & B00000001));
		digitalWrite(PIN_DATA_B, !!(row & B00000010));
		//digitalWrite(PIN_DATA_C, !!(row & B00000100));	// Not present
		//digitalWrite(PIN_DATA_D, !!(row & B00001000));	// Not present

		digitalWrite(PIN_OE, LOW);
		digitalWrite(PIN_LATCH, LOW);

		for (uint8_t col = 0; col < 96; col++)	// LED: 24x12 (x:y), 3 scans (lines), so: 24 * (12 / 3) = 96
		{
			// Set pixels
			digitalWrite(PIN_LED_R, HIGH);
			digitalWrite(PIN_LED_G, LOW);
			digitalWrite(PIN_LED_B, LOW);

			digitalWrite(PIN_CLK, HIGH);
			digitalWrite(PIN_CLK, LOW);
		}

		digitalWrite(PIN_LATCH, HIGH);
		digitalWrite(PIN_OE, HIGH);
	}
	delay(3);	// Brightness (inverted)
}
