#pragma once
#include "Engine/Game.h"

class SolarSystemGame
	: public Game
{
public:
	explicit SolarSystemGame(std::wstring& name)
		: Game(name)
	{
	}

protected:
	void Initialize() override;
};

