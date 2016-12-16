#include <DHT.h>

#define DHT_COUNT 7
DHT* sensors[DHT_COUNT] = {
	new DHT(2, DHT22),
	new DHT(3, DHT22),
	new DHT(4, DHT22),
	new DHT(5, DHT22),
	new DHT(6, DHT22),
	new DHT(7, DHT22),
	new DHT(8, DHT22),
};

void setup()
{
	Serial.begin(9600);
	for (int i = 0; i < DHT_COUNT; i++)
	{
		sensors[i]->begin();	// Start sensors.
	}
	delay(2500);
}

void loop()
{
	for (int i = 0; i < DHT_COUNT; i++)
	{
		Serial.print("#");
		Serial.print(i + 1);
		Serial.print(" T=");
		Serial.print(sensors[i]->readTemperature());
		Serial.print(", H=");
		Serial.println(sensors[i]->readHumidity());
	}
	delay(5000);
	Serial.println("== == ==");
}
