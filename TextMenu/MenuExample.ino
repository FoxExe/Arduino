/*
Name:		MenuExample.ino
Created:	06.01.2018 20:41:17
Author:	Fox

Uses Nokia 5110 LCD as Display and 3 buttons for control (Up, Down, Select/OK)

Program size: 8 324 bytes (used 27% of a 30 720 byte maximum)
Minimum Memory Usage: 944 bytes (46% of a 2048 byte maximum)

TODO:
	* Use PROGMEM for free some ram
	* Make StaticMenu (PROGMEM/Flash) and DynamicMenu (Current/RAM)
*/

#include <Adafruit-GFX-Library-master\Adafruit_GFX.h>
#include <Adafruit-PCD8544-Nokia-5110-LCD-library-master\Adafruit_PCD8544.h>

#include "MenuHelper.h"

#define BTN_UP		2
#define BTN_DOWN	3
#define BTN_OK		4
#define BTN_DEBOUNCE_TIME	10
#define BTN_REPEAT_TIME		250

//Adafruit_PCD8544 display = Adafruit_PCD8544(12, 11, 10, 9, 8);	// Software SPI
Adafruit_PCD8544 display = Adafruit_PCD8544(8, 7, 6);	// Hardware SPI
#define LCD_BL_PIN	5	// Pin for control LCD Backlight

MenuItem MainMenuList[] = {
	{ "Sub menu", PrintSubMenu },
	{ "Backlight", ToggleBacklight },
	{ "Item 3", nullptr },
	{ "Item 4 --", nullptr },
	{ "Item 5", nullptr },
	{ "Item 6 ++", nullptr },
	{ "Item 7", nullptr },
	{ "Item 8 ==", nullptr },
	{ "Item 9", nullptr },
	{ "Item 10", nullptr },
	{ "Item 11 aa", nullptr },
	{ "Item 12 bb", nullptr },
	{ "Item 13 cc", nullptr },
	{ "Item 14 dd", nullptr },
	{ "Item 15 ee", nullptr },
	{ "Item 16 ff", nullptr },
};

MenuList MainMenu = {
	"Main menu",
	MainMenuList,
	sizeof(MainMenuList) / sizeof(MenuItem),
};

MenuItem SubMenuList[] = {
	{ "Sub Item 1", nullptr },
	{ "Sub Item 2", nullptr },
	{ "Sub Item 3", nullptr },
	{ "Sub Item 4", nullptr },
	{ "Sub Item 5", nullptr },
	{ "Sub Item 6", nullptr },
	{ "Sub Item 7", nullptr },
	{ "Main Menu", PrintMainMenu },
};

MenuList SubMenu = {
	"Sub menu",
	SubMenuList,
	sizeof(SubMenuList) / sizeof(MenuItem),
};

MenuList *CurrentMenu = &MainMenu;
uint8_t ActiveItem = 0;
uint8_t MaxMenuItems = 5;
uint8_t SpcMenuItems = 2;

void PrintMainMenu() {
	CurrentMenu = &MainMenu;
	ActiveItem = 0;
	PrintMenu();
}

void PrintSubMenu() {
	CurrentMenu = &SubMenu;
	ActiveItem = 0;
	PrintMenu();
}

void ToggleBacklight() {
	digitalWrite(LCD_BL_PIN, !digitalRead(LCD_BL_PIN));		// Led backlight TURN ON
}

void(*BtnOnPressUp)();
void(*BtnOnPressDown)();
void(*BtnOnPressOk)();

void PrintMenu() {
	uint8_t min = 0;

	if (ActiveItem > SpcMenuItems) {
		min = ActiveItem - SpcMenuItems;

		if (ActiveItem > CurrentMenu->Count - MaxMenuItems + 1)
			min = CurrentMenu->Count - MaxMenuItems;
	}

	uint8_t max = min + MaxMenuItems;
	if (max > CurrentMenu->Count - 1)
		max = CurrentMenu->Count;

	display.clearDisplay();
	for (; min < max; min++)
	{
		(min == ActiveItem) ? display.print(">") : display.print(" ");
		display.println(CurrentMenu->Items[min].Name);
	}

	/*
	// Scroll right side, 5px width with line
	display.drawRect(84 - 5, 0, 5, 48, BLACK);
	display.drawFastVLine(84 - 3, map(ActiveItem, 0, CurrentMenu->Count, 2, 48 - 3), (48 - 2 * 2) / CurrentMenu->Count, BLACK);
	*/
	uint8_t scrollSize = (48 - 2 * 2) / CurrentMenu->Count;
	display.drawFastVLine(84 - 1, map(ActiveItem, 0, CurrentMenu->Count, 0, 48 - scrollSize), scrollSize, BLACK);


	/*
	// Menu items count, bottom-right + horisontal line top of hem
	display.drawFastHLine(0, 40, 84, BLACK);
	display.setCursor(6 * 9, 41);	// Font size * char count (padding/tabulation)
	display.print(ActiveItem + 1);
	display.print("/");
	display.print(CurrentMenu->Count);
	*/

	/*
	// Menu items count, bottom-right, White text on Black background
	display.fillRect(0, 40, 84, 8, BLACK);
	display.setTextColor(WHITE);
	display.setCursor(6 * 9, 41);
	display.print(ActiveItem + 1);
	display.print("/");
	display.print(CurrentMenu->Count);
	display.setTextColor(BLACK);
	*/

	display.display();

}

void MenuUp() {
	if (ActiveItem == 0)
		ActiveItem = CurrentMenu->Count - 1;
	else
		ActiveItem--;
	PrintMenu();
}

void MenuDown() {
	if (ActiveItem == CurrentMenu->Count - 1)
		ActiveItem = 0;
	else
		ActiveItem++;
	PrintMenu();
}

void MenuSelect() {
	if (CurrentMenu->Items[ActiveItem].Function != nullptr)
		CurrentMenu->Items[ActiveItem].Function();	// Need run in separated thread or similar
}

void setup() {
	// Buttons init
	pinMode(BTN_UP, INPUT_PULLUP);
	pinMode(BTN_DOWN, INPUT_PULLUP);
	pinMode(BTN_OK, INPUT_PULLUP);

	BtnOnPressDown = MenuDown;
	BtnOnPressUp = MenuUp;
	BtnOnPressOk = MenuSelect;

	// LCD init
	display.begin();
	display.setContrast(52);
	display.setTextColor(BLACK);
	display.setTextSize(1);
	display.clearDisplay();
	display.display();

	pinMode(LCD_BL_PIN, OUTPUT);		// Led backlight init
	digitalWrite(LCD_BL_PIN, HIGH);		// Led backlight TURN ON

	// Print main menu on screen
	PrintMenu();
}

void CheckButton(uint8_t buttonPin, void(*function)()) {
	if (digitalRead(buttonPin) == LOW)
	{
		delay(BTN_DEBOUNCE_TIME); // debounce
		while (digitalRead(buttonPin) == LOW)
		{
			if (function != nullptr)
				function();
			delay(BTN_REPEAT_TIME);
		}
	}
}

void loop() {
	CheckButton(BTN_UP, BtnOnPressUp);
	CheckButton(BTN_DOWN, BtnOnPressDown);
	CheckButton(BTN_OK, BtnOnPressOk);
}
