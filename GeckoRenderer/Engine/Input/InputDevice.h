#pragma once
#include "windows.h"
#include <iostream>
#include "winuser.h"
#include <wrl.h>
#include "Keyboard.h"
#include "Mouse.h"

class InputDevice
{
	DirectX::Keyboard keyboard;
	DirectX::Mouse mouse;

public:
	bool IsKeyDown(DirectX::Keyboard::Keys key);

	DirectX::Keyboard& getKeyboard();
	DirectX::Mouse& getMouse();
	
	virtual LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);
};

