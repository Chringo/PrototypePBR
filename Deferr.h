#pragma once
#include "GlobalData.h"
#define PASSES 4
class Deferr
{
public:
	Deferr(ID3D11DeviceContext* gDeviceContext,	ID3D11Device* gDevice);
	~Deferr();

	

private:
	Deferr();
	const UINT WIN_WIDTH = 640;
	const UINT WIN_HEIGHT = 480;

	ID3D11DeviceContext* gDeviceContext;
	ID3D11Device* gDevice;
	ID3D11DepthStencilView * DSV;
	ID3D11ShaderResourceView * SRVs[PASSES];
	ID3D11Texture2D * T2Ds[PASSES];
	ID3D11RenderTargetView * RTVs[PASSES];
	ID3D11Buffer * vertexBufferQuad;


	void initDefQuad();
	void initShaders();
	void initBuffers();
	void initTexViews();

	const quadVertex triangleVertices[6] = 
	{
		//T1
		-1.0f, -1.0f, 0.0f,	//v0 pos
		0.0f, 1.0f,			//v0 UV

		-1.0f, 1.0f, 0.0f,	//v1
		0.0f, 0.0f,			//v1 UV

		1.0f, -1.0f, 0.0f, //v2
		1.0f, 1.0f,			//v2 UV

		//T2
		-1.0f, 1.0f, 0.0f,	//v3 pos
		0.0f, 0.0f,			//v3 UV

		1.0f, 1.0f, 0.0f,	//v4
		1.0f, 0.0f,			//v4 UV

		1.0f, -1.0f, 0.0f,	//v5
		1.0f, 1.0f			//v5 UV
	};

};

