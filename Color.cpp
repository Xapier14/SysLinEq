#include <Windows.h>
#include <wincon.h>
#include <iostream>

using namespace std;

#include "Color.h"

// gets the current color (screen buffer info/text attribute)
int GetCurrentColor(HANDLE cHandle) {
	if (cHandle == NULL) throw new exception("Console handle is null.");
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(cHandle, &info);
	return info.wAttributes;
}

// sets the color (background&foreground).
// the color codes are defined in the msdn page on win32's console text attributes.
void SetColor(int color, HANDLE cHandle) {
	if (cHandle == NULL) throw new exception("Console handle is null.");
	SetConsoleTextAttribute(cHandle, color);
}

// returns the current std out handle
HANDLE GetConsoleHandle() {
	return GetStdHandle(STD_OUTPUT_HANDLE);
}