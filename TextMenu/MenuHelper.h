#pragma once

struct MenuItem
{
	const char *Name;
	void(*Function)();
};

struct MenuList
{
	const char *Name;
	MenuItem *Items;
	uint8_t Count;
};