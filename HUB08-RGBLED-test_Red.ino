
// Led panel info
#define LED_SIZE_X	24		// Size in pixels (Width)
#define LED_SIZE_Y	12		// Size in pixels (Height)
#define LED_CHIPS	6		// 6 chips per color
#define LED_ROWS	3		// "1/3 scan". Its just 3 different lines, selected by pins A and B.
#define LED_BLOCKS	3		// 3 "blocks" of pixels (8x12, vertical). 2 chips per block.
#define LED_PPR		(LED_SIZE_X * LED_SIZE_Y) / LED_ROWS	// Pixels per row

// Arduino pins
#define PIN_CLK			10
#define PIN_LATCH		9
#define PIN_OE			11	// OE = Output enable

#define PIN_DATA_A		2
#define PIN_DATA_B		3

#define PIN_LED_R		6
#define PIN_LED_G		7
#define PIN_LED_B		8

/*
unsigned long timerPrevious = 0;
unsigned long timerInterval = 5000;
uint8_t imageNum = 0;
*/

uint8_t imageR[] = {
	1, 0, 1, 0, 1, 0, 1, 0,   1, 0, 1, 0, 1, 0, 1, 0,   1, 0, 1, 0, 1, 0, 1, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,

	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 1, 0, 1, 0, 1, 0, 1,   0, 1, 0, 1, 0, 1, 0, 1,   0, 1, 0, 1, 0, 1, 0, 1,

};

uint8_t imageG[] = {
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 1, 1, 1, 0, 0,   0, 1, 1, 0, 0, 0, 1, 1,   0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 1, 1, 1, 1, 1, 0,   1, 1, 1, 1, 0, 0, 0, 1,   1, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 0, 1,   1, 0, 0, 1, 1, 0, 0, 0,   1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 1, 1, 1, 1, 0, 1,   1, 0, 0, 1, 1, 0, 0, 0,   0, 1, 1, 0, 0, 0, 0, 0,

	0, 0, 1, 1, 1, 0, 0, 1,   1, 0, 0, 1, 1, 0, 0, 0,   0, 1, 1, 0, 0, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 0, 1,   1, 0, 0, 1, 1, 0, 0, 0,   1, 1, 1, 1, 0, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 0, 0,   1, 1, 1, 1, 0, 0, 0, 1,   1, 0, 0, 1, 1, 0, 0, 0,
	0, 0, 1, 1, 0, 0, 0, 0,   0, 1, 1, 0, 0, 0, 1, 1,   0, 0, 0, 0, 1, 1, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
};

uint8_t imageB[] = {
	0, 1, 0, 1, 0, 1, 0, 1,   0, 1, 0, 1, 0, 1, 0, 1,   0, 1, 0, 1, 0, 1, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,

	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 0,   0, 0, 0, 0, 0, 0, 0, 1,
	1, 0, 1, 0, 1, 0, 1, 0,   1, 0, 1, 0, 1, 0, 1, 0,   1, 0, 1, 0, 1, 0, 1, 0,
};

//static unsigned long timeCounter = 0;

void setup()
{
	Serial.begin(115200);

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

	digitalWrite(PIN_OE, HIGH);
	digitalWrite(PIN_LATCH, LOW);
	digitalWrite(PIN_CLK, LOW);

	delay(1000);
}

void loop()
{
	/*
	unsigned long timerCurrent = millis();
	if (timerCurrent - timerPrevious >= timerInterval) {
		timerPrevious = timerCurrent;
		switch (imageNum)
		{
		default:
			imageNum = 0;
			return;
			break;
		}
		
		imageNum++;
	}
	*/

	for (uint8_t row = 0; row < LED_ROWS; row++)
	{
		// Row select
		digitalWrite(PIN_DATA_A, !!(row & B00000001));
		digitalWrite(PIN_DATA_B, !!(row & B00000010));

		for (uint8_t block = 0; block < LED_BLOCKS; block++)	// 3
		{
			for (uint8_t rowInBlock = 0; rowInBlock < (LED_SIZE_Y / LED_ROWS); rowInBlock++)	// 2
			{
				for (uint8_t colInBlock = 0; colInBlock < (LED_SIZE_X / LED_BLOCKS); colInBlock++)	// 8
				{
					int pos = (rowInBlock + 1) * LED_SIZE_X * LED_ROWS - row * LED_SIZE_X - LED_SIZE_X / LED_BLOCKS * block - colInBlock - 1;
					digitalWrite(PIN_LED_R, imageR[pos]);
					digitalWrite(PIN_LED_G, imageG[pos]);
					digitalWrite(PIN_LED_B, imageB[pos]);

					digitalWrite(PIN_CLK, HIGH);
					digitalWrite(PIN_CLK, LOW);
				}
			}
		}
		
		digitalWrite(PIN_LATCH, HIGH);
		digitalWrite(PIN_LATCH, LOW);
		
		// Brightness. Less = bright (PWM-Like)
		digitalWrite(PIN_OE, LOW);
		delayMicroseconds(100);
		digitalWrite(PIN_OE, HIGH);
	}
}
