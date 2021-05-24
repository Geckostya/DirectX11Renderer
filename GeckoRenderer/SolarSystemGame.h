#pragma once
#include "Engine/Game.h"
#include "Engine/GameComponents/GeometryGameComponent.h"

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
	void Update(float deltaTime) override;
private:
	GeometryGameComponent* cube1;
	GeometryGameComponent* cube2;
};

