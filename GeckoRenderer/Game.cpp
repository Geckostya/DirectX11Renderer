#include "Game.h"

#include "DisplayWin.h"
#include "DebugRectangleGameComponent.h"


Game::Game(std::wstring& name)
{
	Name = name;
	Instance = this;
}

Game::~Game()
{
	if (Display) delete Display;
	if (TotalTime) delete TotalTime;

	if (Debug)
	{
		Debug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
	}

	if (Device)
	{
		Device->Release();
	}

	for (auto* component : GameComponents)
	{
		delete component;
	}
	GameComponents.clear();
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	return Game::Instance->MessageHandler(hwnd, umessage, wparam, lparam);
}

void Game::Run(int windowWidth, int windowHeight)
{
	Display = new DisplayWin(Name, windowWidth, windowHeight, WndProc);

	PrepareResources();

	Initialize();
	for (auto* component : GameComponents)
	{
		component->Initialize();
	}

	StartTime = new std::chrono::time_point<std::chrono::steady_clock>();
	PrevTime = new std::chrono::time_point<std::chrono::steady_clock>();

	*StartTime = std::chrono::steady_clock::now();
	*PrevTime = *StartTime;

	TotalTime = new std::chrono::duration<long>;

	MSG msg = {};

	// Loop until there is a quit message from the window or the user.
	while (!isExitRequested) {
		Display->ProcessAllMessages();
		UpdateInternal();
	}

	delete StartTime;
	delete PrevTime;

	DestroyResources();
}


void Game::Exit()
{
	isExitRequested = true;
}

void Game::RestoreTargets()
{
}

LRESULT Game::MessageHandler(HWND hwnd, UINT umessage, WPARAM wparam, LPARAM lparam)
{
	switch (umessage)
	{
		// Check if the window is being destroyed.
	case WM_DESTROY:
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		Exit();
		return 0;
	}

	case WM_SIZE:
	{
		std::cout << "Width " << LOWORD(lparam) << " Height " << HIWORD(lparam) << std::endl;

		if (Device)
		{
			int newWidth = LOWORD(lparam);
			int newHeight = HIWORD(lparam);

			if (newHeight == 0 || newWidth == 0 || (Display->ClientHeight == newHeight && Display->ClientWidth == newWidth))
			{
				return 0;
			}

			Display->ClientHeight = newHeight;
			Display->ClientWidth = newWidth;

			if (backBuffer != nullptr)
			{
				backBuffer->Release(); backBuffer = nullptr;
			}
			if (RenderView != nullptr)
			{
				RenderView->Release(); RenderView = nullptr;
			}

			SwapChain1->ResizeBuffers(2, newWidth, newHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			CreateBackBuffer();
		}

		return 0;
	}

	// Check if a key has been pressed on the keyboard.
	case WM_KEYDOWN:
	{
		// If a key is pressed send it to the input object so it can record that state.
		std::cout << "Key: " << static_cast<unsigned int>(wparam) << std::endl;

		if (static_cast<unsigned int>(wparam) == 27)
		{
			PostQuitMessage(0);
			Exit();
		}
		return 0;
	}

	// Check if a key has been released on the keyboard.
	case WM_KEYUP:
	{
		// If a key is released then send it to the input object so it can unset the state for that key.
		return 0;
	}

	// All other messages pass to the message handler in the system class.
	default:
	{
		return DefWindowProc(hwnd, umessage, wparam, lparam);
	}
	}
}

void Game::Initialize()
{
	GameComponents.push_back(new DebugRectangleGameComponent(this));
}

void Game::Update(float deltaTime)
{
	for (auto* component : GameComponents)
	{
		component->Update(deltaTime);
	}
}

void Game::Draw(float deltaTime)
{
	for (auto* component : GameComponents)
	{
		component->Draw(deltaTime);
	}
}

void Game::PostDraw(float deltaTime)
{
}

void Game::PrepareResources()
{
	DXGI_SWAP_CHAIN_DESC swapDesc = {};
	swapDesc.BufferCount = 2;
	swapDesc.BufferDesc.Width = Display->ClientWidth;
	swapDesc.BufferDesc.Height = Display->ClientHeight;
	swapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.OutputWindow = Display->handle;
	swapDesc.Windowed = true;
	swapDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_11_1 };
	HRESULT res = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevel,
		1,
		D3D11_SDK_VERSION,
		&swapDesc,
		&SwapChain,
		&Device,
		nullptr,
		&Context);
	ZCHECK(res);

	CreateBackBuffer();

	SwapChain->QueryInterface<IDXGISwapChain1>(&SwapChain1);
	Context->QueryInterface(IID_ID3DUserDefinedAnnotation, (void**)&DebugAnnotation);

	Device->QueryInterface(IID_ID3D11Debug, (void**)&Debug);
}

void Game::UpdateInternal()
{
	auto curTime = std::chrono::steady_clock::now();
	float deltaTime = std::chrono::duration_cast<std::chrono::microseconds>(curTime - *PrevTime).count() / 1000000.0f;
	*PrevTime = curTime;
	*TotalTime = std::chrono::duration_cast<std::chrono::seconds>(curTime - *StartTime);

	PrepareFrame();

	Update(deltaTime);

	Draw(deltaTime);

	PostDraw(deltaTime);

	EndFrame();
}

void Game::PrepareFrame()
{
	Context->ClearState();
	
	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(Display->ClientWidth);
	viewport.Height = static_cast<float>(Display->ClientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	Context->OMSetRenderTargets(1, &RenderView, nullptr);

	Context->RSSetViewports(1, &viewport);
	Context->RSSetState(RastState);
	
	Context->ClearRenderTargetView(RenderView, DirectX::SimpleMath::Color(0, 0, 0, 1));
}

void Game::EndFrame()
{
	SwapChain1->Present(1, 0);
}

void Game::DestroyResources()
{
	backBuffer->Release();
	for (auto* component : GameComponents)
	{
		component->DestroyResources();
	}
}

void Game::CreateBackBuffer()
{
	HRESULT res = SwapChain->GetBuffer(0, IID_ID3D11Texture2D, (void**)&backBuffer);
	ZCHECK(res);
	
	res = Device->CreateRenderTargetView(backBuffer, nullptr, &RenderView);
	ZCHECK(res);
}