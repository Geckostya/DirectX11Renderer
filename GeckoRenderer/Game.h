#pragma once

#include <chrono>

#include "DisplayWin.h"
#include "pch.h"

class GameComponent;

class Game
{
public:
	inline static Game* Instance;

	std::wstring Name;
	std::vector<GameComponent*> GameComponents;
	bool isExitRequested;

public:
	DisplayWin* Display;
	std::chrono::time_point<std::chrono::steady_clock>* StartTime;
	std::chrono::time_point<std::chrono::steady_clock>* PrevTime;
	std::chrono::duration<long>* TotalTime;

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context;
	IDXGISwapChain* SwapChain;
	IDXGISwapChain1* SwapChain1;
	ID3D11RenderTargetView* RenderView;
	ID3DUserDefinedAnnotation* DebugAnnotation;
	ID3D11Debug* Debug;
	ID3D11RasterizerState* RastState;
	ID3D11Texture2D* backBuffer;


public:
	Game(std::wstring& name);
	virtual ~Game();

	void Run(int windowWidth, int windowHeight);
	void Exit();

	void RestoreTargets();

	virtual LRESULT CALLBACK MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam);

protected:
	
	virtual void Initialize();
	virtual void Update(float deltaTime);
	virtual void Draw(float deltaTime);
	virtual void PostDraw(float deltaTime);
	
	void PrepareResources();
	void UpdateInternal();
	void PrepareFrame();
	void EndFrame();
	void DestroyResources();

private:
	void CreateBackBuffer();
};

