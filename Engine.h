#pragma once
#include <windows.h>
#include"GlobalData.h"
#include "FileLoader.h"
#include "Camera.h"
#include "Deferr.h"

class Engine
{
private:
	FileLoader * fileLoader;
	Deferr * deferr;

	//This interface sets renderstates, bind resourses to the graphics pipeline, and issues rendering commands
	ID3D11DeviceContext* gDeviceContext;
	//Checks feature support and allocating resources
	ID3D11Device* gDevice;

	//check 4x MSAA quality support
	bool enable4xMSAA; //false by default
	UINT g4xMsaaQuality;

	//interface hanling the front and backbuffer
	IDXGISwapChain* gSwapChain;

	ID3D11RenderTargetView* gRenderTargetView; //need this here?

	ID3D11Texture2D* mDepthStencilBuffer;
	ID3D11DepthStencilView* mDepthStencilView;

	ID3D11VertexShader* vertexShader = nullptr;
	ID3D11PixelShader* pixelShader;

	ID3D11Buffer* indexBuffer;
	ID3D11InputLayout* inputLayout;

	ID3D11InputLayout* vertexLayout;
	
	Camera * camera;

	void draw();
	void defPass();
	
	
public:
	Engine();
	virtual ~Engine();

	bool initialize(HWND* window);
	void shutdown();

	ID3D11Device* getDevice();
	ID3D11DeviceContext* getDeviceContext();
	void render();
};

