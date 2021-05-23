#include "InputDevice.h"

#include "../DisplayWin.h"


DirectX::Keyboard& InputDevice::getKeyboard()
{
	return DirectX::Keyboard::Get();
}

DirectX::Mouse& InputDevice::getMouse()
{
	return DirectX::Mouse::Get();
}

InputDevice::InputDevice(DisplayWin* display)
{
	getMouse().SetWindow(display->handle);
}

bool InputDevice::IsKeyDown(DirectX::Keyboard::Keys key)
{
	return DirectX::Keyboard::Get().GetState().IsKeyDown(key);
}

LRESULT InputDevice::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	DirectX::Keyboard::ProcessMessage(umessage, wparam, lparam);
	DirectX::Mouse::ProcessMessage(umessage, wparam, lparam);
	return 0;
}
