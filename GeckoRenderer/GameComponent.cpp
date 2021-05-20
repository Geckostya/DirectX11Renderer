#include "GameComponent.h"
#include "Game.h"

GameComponent::GameComponent(Game* game)
{
	this->game = game;
}

void GameComponent::Initialize()
{
}

void GameComponent::Reload()
{
}

void GameComponent::Update(float deltaTime)
{
}

void GameComponent::Draw(float deltaTime)
{
}

void GameComponent::DestroyResources()
{
}
