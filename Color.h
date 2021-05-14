#pragma once
#include <Windows.h>
#include <wincon.h>

int GetCurrentColor(HANDLE cHandle);
void SetColor(int color, HANDLE cHandle);
HANDLE GetConsoleHandle();