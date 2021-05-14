#include <Windows.h>
#include <wincon.h>
#include <iostream>

using namespace std;

#include "Color.h"

int GetCurrentColor(HANDLE cHandle) {
	if (cHandle == NULL) throw new exception("Console handle is null.");
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(cHandle, &info);
	return info.wAttributes;
}

void SetColor(int color, HANDLE cHandle) {
	if (cHandle == NULL) throw new exception("Console handle is null.");
	SetConsoleTextAttribute(cHandle, color);
}

HANDLE GetConsoleHandle() {
	return GetStdHandle(STD_OUTPUT_HANDLE);
}