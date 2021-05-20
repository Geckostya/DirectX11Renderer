#pragma once

#include "pch.h"

class DisplayWin
{
public:
	DisplayWin(std::wstring& windowName, int width, int height, WNDPROC wndProc);
	~DisplayWin();

public:
	int ClientWidth = 0;
	int ClientHeight = 0;

public:
	HINSTANCE   hInstance = nullptr;
	HWND        hWnd = nullptr;
	WNDCLASSEX  wc = {};

	static HMODULE Module;
};

