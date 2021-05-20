
#include "Game.h"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE prevInstance, _In_  PSTR cmdLine, _In_  int showCmd)
{
	std::wstring name(L"Game");
	Game game(name);
	game.Run(800, 800);
	return 0;
}
