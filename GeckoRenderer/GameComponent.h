#pragma once

class Game;

class GameComponent
{
public:
	Game* game;

public:
	explicit GameComponent(Game* game);
	
	virtual void Initialize();
	virtual void Reload();
	virtual void Update(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void DestroyResources();
};

