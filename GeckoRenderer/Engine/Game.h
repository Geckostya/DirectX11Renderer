#pragma once

#include <chrono>

#include "DisplayWin.h"
#include "Input/InputDevice.h"
#include "pch.h"
#include "Camera/CameraController.h"
#include "Camera/CameraMatrix.h"

class GameComponent;

class Game
{
public:
	inline static Game* Instance = nullptr;

	std::wstring Name;
	std::vector<GameComponent*> GameComponents;
	bool isExitRequested;

	CameraMatrix* Camera;
	CameraController* CameraController;

public:
	DisplayWin* Display = nullptr;
	std::chrono::time_point<std::chrono::steady_clock>* StartTime = nullptr;
	std::chrono::time_point<std::chrono::steady_clock>* PrevTime = nullptr;
	std::chrono::duration<long, std::ratio<1, 1000000>>* TotalTime = nullptr;
	InputDevice* inputDevice;

public:
	Microsoft::WRL::ComPtr<ID3D11Device> Device;
	ID3D11DeviceContext* Context = nullptr;
	IDXGISwapChain* SwapChain = nullptr;
	IDXGISwapChain1* SwapChain1 = nullptr;
	ID3D11RenderTargetView* RenderView = nullptr;
	ID3D11DepthStencilView* DepthView = nullptr;
	ID3DUserDefinedAnnotation* DebugAnnotation = nullptr;
	ID3D11Debug* Debug = nullptr;
	ID3D11RasterizerState* RastState = nullptr;
	ID3D11Texture2D* backBuffer = nullptr;
	ID3D11Texture2D* depthBuffer = nullptr;


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

	float getTime() const;

private:
	void CreateBackBuffer();
};

