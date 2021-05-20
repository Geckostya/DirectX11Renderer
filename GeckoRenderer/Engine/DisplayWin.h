#pragma once

#include "pch.h"

class DisplayWin
{
public:
	DisplayWin(std::wstring& windowName, int width, int height, WNDPROC wndProc);
	~DisplayWin();
	bool ProcessMessage() const;
	void ProcessAllMessages() const;

public:
	int ClientWidth = 0;
	int ClientHeight = 0;

public:
	HINSTANCE   hInstance = nullptr;
	HWND        handle = nullptr;
	WNDCLASSEX  wc = {};

	static HMODULE Module;
};

