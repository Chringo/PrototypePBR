#pragma once
#include "GlobalData.h"
#define RTV_COUNT 5
class Deferr
{
public:
	Deferr(ID3D11DeviceContext* gDeviceContext,	ID3D11Device* gDevice);
	~Deferr();

	void firstPass(ID3D11Buffer * vertexBuffer, 
		ID3D11Buffer * indexBuffer,
		ID3D11Buffer * modelWorldCb,
		ID3D11Buffer * cameraVpCb
	);
	void finalPass(ID3D11RenderTargetView * RTV, ID3D11DepthStencilView * DSV);

private:
	Deferr();
	ID3D11ShaderResourceView* nullsrvs[5] =
	{
		NULL,
		NULL,
		NULL,
		NULL
	};

	const UINT WIN_WIDTH = 640;
	const UINT WIN_HEIGHT = 480;

	ID3D11DeviceContext* gDeviceContext;
	ID3D11Device* gDevice;
	ID3D11DepthStencilView * DSV;
	ID3D11ShaderResourceView * SRVs[RTV_COUNT];
	ID3D11Texture2D * T2Ds[RTV_COUNT];
	ID3D11RenderTargetView * RTVs[RTV_COUNT];
	ID3D11Buffer * vertexBufferQuad;
	ID3D11SamplerState * pointSamplerState;
	ID3D11SamplerState * linearSamplerState;

	ID3D11VertexShader* deferrVertexShader = nullptr;
	ID3D11PixelShader* deferrPixelShader = nullptr;

	ID3D11VertexShader* finalVertexShader = nullptr;
	ID3D11PixelShader* finalPixelShader = nullptr;

	ID3D11InputLayout* deferrVertexLayout;
	ID3D11InputLayout* finalVertexLayout;

	ID3D11Resource* albedoResource = nullptr;
	ID3D11ShaderResourceView* albedoResourceView = nullptr;
	ID3D11Resource* metalnessResource = nullptr;
	ID3D11ShaderResourceView* metalnessResourceView = nullptr;
	ID3D11Resource* normalResource = nullptr;
	ID3D11ShaderResourceView* normalResourceView = nullptr;
	ID3D11Resource* roughnessResource = nullptr;
	ID3D11ShaderResourceView* roughnessResourceView = nullptr;
	ID3D11Resource* aoResource = nullptr;
	ID3D11ShaderResourceView* aoResourceView = nullptr;


	const quadVertex triangleVertices[6] = 
	{
		//T1
		DirectX::XMFLOAT3 (-1.0f, -1.0f, 0.0f),	//v0 pos
		DirectX::XMFLOAT2(0.0f, 1.0f),			//v0 UV

		DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),	//v1
		DirectX::XMFLOAT2(0.0f, 0.0f),			//v1 UV

		DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), //v2
		DirectX::XMFLOAT2(1.0f, 1.0f),			//v2 UV

		//T2
		DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f),	//v3 pos
		DirectX::XMFLOAT2(0.0f, 0.0f),			//v3 UV

		DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f),	//v4
		DirectX::XMFLOAT2(1.0f, 0.0f),			//v4 UV

		DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f),	//v5
		DirectX::XMFLOAT2(1.0f, 1.0f)			//v5 UV
	};

	void initDefQuad();
	void initShaders();
	void initBuffers();
	void initTexViews();

public:
	void InitDDS();
};

