#include "Deferr.h"
#include "DDSTextureLoader.h"

using namespace DirectX;

void Deferr::InitDDS()
{
	HRESULT hr;
	ID3D11Resource* textureResource;
	ID3D11ShaderResourceView* textureResourceView;
	size_t maxSize = 0;

	wchar_t * testPath = L"./BBF/PBR dds/test_albedom.dds";
	hr = CreateDDSTextureFromFile(
		this->gDevice,
		testPath,
		&textureResource,
		&textureResourceView,
		maxSize,
		(DDS_ALPHA_MODE*)DDS_ALPHA_MODE_UNKNOWN);

}

Deferr::Deferr()
{
}

inline void Deferr::initDefQuad()
{
	HRESULT hr;

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(quadVertex) * 6;

	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = triangleVertices;
	hr = gDevice->CreateBuffer(&bufferDesc, &data, &vertexBufferQuad);
}

void Deferr::initShaders()
{
	HRESULT hr;

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP; //wrap, (repeat) for use of tiling texutures
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = gDevice->CreateSamplerState(&samplerDesc, &linearSamplerState);

	gDeviceContext->PSSetSamplers(0, 1, &linearSamplerState);


	D3D11_SAMPLER_DESC samplerDescPoint;
	samplerDescPoint.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDescPoint.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescPoint.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescPoint.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samplerDescPoint.MipLODBias = 0.0f;  //mipmap offset level
	samplerDescPoint.MaxAnisotropy = 1;
	samplerDescPoint.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDescPoint.MinLOD = 0;
	samplerDescPoint.MaxLOD = D3D11_FLOAT32_MAX;

	hr = gDevice->CreateSamplerState(&samplerDesc, &pointSamplerState);

	gDeviceContext->PSSetSamplers(1, 1, &pointSamplerState);

	//maek finalpass shaders
	ID3DBlob* pfVS = nullptr;
	hr = D3DCompileFromFile(
		L"finalPass.hlsl",
		nullptr,
		nullptr,
		"VS_main",
		"vs_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&pfVS,
		nullptr);

	hr = gDevice->CreateVertexShader(pfVS->GetBufferPointer(), pfVS->GetBufferSize(), nullptr, &this->finalVertexShader);

	D3D11_INPUT_ELEMENT_DESC inputDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	hr = gDevice->CreateInputLayout(inputDesc, ARRAYSIZE(inputDesc), pfVS->GetBufferPointer(), pfVS->GetBufferSize(), &this->finalVertexLayout);
	pfVS->Release();

	ID3DBlob *pfPs = nullptr;
	D3DCompileFromFile(
		L"finalPass.hlsl",
		nullptr,
		nullptr,
		"PS_main",
		"ps_5_0",
		D3DCOMPILE_DEBUG,
		0,
		&pfPs,
		nullptr);

	hr = gDevice->CreatePixelShader(pfPs->GetBufferPointer(), pfPs->GetBufferSize(), nullptr, &this->finalPixelShader);
	pfPs->Release();

	//DEFEEEEEEEEEERRRRRRRRRRRRRRR
	ID3DBlob* pVS = nullptr;
	D3DCompileFromFile(
		L"Vertex.hlsl", // filename
		nullptr,		// optional macros
		nullptr,		// optional include files
		"VS_main",		// entry point
		"vs_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,				// shader compile options
		0,				// effect compile options
		&pVS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	hr = gDevice->CreateVertexShader(pVS->GetBufferPointer(), pVS->GetBufferSize(), nullptr, &this->deferrVertexShader);

	//creating the input layout
	D3D11_INPUT_ELEMENT_DESC desc1[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = gDevice->CreateInputLayout(desc1, ARRAYSIZE(desc1), pVS->GetBufferPointer(), pVS->GetBufferSize(), &this->deferrVertexLayout);
	pVS->Release();

	ID3DBlob* pPS = nullptr;
	D3DCompileFromFile(
		L"Fragment.hlsl", // filename		//L"PixelShader.hlsl"
		nullptr,		// optional macros
		nullptr,		// optional include files
		"PS_main",		// entry point
		"ps_5_0",		// shader model (target)
		D3DCOMPILE_DEBUG,				// shader compile options
		0,				// effect compile options
		&pPS,			// double pointer to ID3DBlob		
		nullptr			// pointer for Error Blob messages.
						// how to use the Error blob, see here
						// https://msdn.microsoft.com/en-us/library/windows/desktop/hh968107(v=vs.85).aspx
	);

	hr = gDevice->CreatePixelShader(pPS->GetBufferPointer(), pPS->GetBufferSize(), nullptr, &this->deferrPixelShader);

}

void Deferr::initBuffers()
{


}

void Deferr::initTexViews()
{
	for (int pass = 0; pass < PASSES; pass++)
	{
		HRESULT hr;
		D3D11_TEXTURE2D_DESC textureDesc;
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
		D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;


		ZeroMemory(&textureDesc, sizeof(textureDesc));
		//Set up the render texture desciption

		textureDesc.Width = (UINT)WIN_WIDTH;
		textureDesc.Height = (UINT)WIN_HEIGHT;
		textureDesc.MipLevels = 1;
		textureDesc.ArraySize = 1;
		textureDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
		textureDesc.SampleDesc.Count = 1;
		textureDesc.Usage = D3D11_USAGE_DEFAULT;
		textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
		textureDesc.CPUAccessFlags = 0;
		textureDesc.MiscFlags = 0;

		//Create the render target Texture

		hr = gDevice->CreateTexture2D(&textureDesc, NULL, &T2Ds[pass]);
		if (FAILED(hr))
			MessageBox(NULL, L"Failed to create deferr Texture2D", L"Error", MB_ICONERROR | MB_OK);

		//set up description for render target view
		ZeroMemory(&renderTargetDesc, sizeof(renderTargetDesc));
		renderTargetDesc.Format = textureDesc.Format;
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetDesc.Texture2D.MipSlice = 0;

		//Create render target

		hr = gDevice->CreateRenderTargetView(T2Ds[pass], &renderTargetDesc, &RTVs[pass]);
		if (FAILED(hr))
			MessageBox(NULL, L"Failed to create deferr RTV", L"Error", MB_ICONERROR | MB_OK);

		//Set up the shader resource view
		ZeroMemory(&resourceViewDesc, sizeof(resourceViewDesc));
		resourceViewDesc.Format = textureDesc.Format;
		resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		resourceViewDesc.Texture2D.MostDetailedMip = 0;
		resourceViewDesc.Texture2D.MipLevels = 1;

		//Create the resourceView;
		hr = gDevice->CreateShaderResourceView(T2Ds[pass], nullptr, &SRVs[pass]);
		if (FAILED(hr))
			MessageBox(NULL, L"Failed to create deferr SRV", L"Error", MB_ICONERROR | MB_OK);
	}

	HRESULT hr;

	ID3D11Texture2D* stencilTest;

	D3D11_TEXTURE2D_DESC textureDesc;
	ZeroMemory(&textureDesc, sizeof(textureDesc));
	//Set up the render texture desciption

	textureDesc.Width = (UINT)WIN_WIDTH;
	textureDesc.Height = (UINT)WIN_HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_D32_FLOAT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;

	hr = gDevice->CreateTexture2D(&textureDesc, NULL, &stencilTest);
	if (FAILED(hr))
		MessageBox(NULL, L"failed creating deferr texure2D stencil", L"Error", MB_ICONERROR | MB_OK);

	hr = gDevice->CreateDepthStencilView(stencilTest, nullptr, &DSV);
	if (FAILED(hr))
		MessageBox(NULL, L"failed creating deferr DSV", L"Error", MB_ICONERROR | MB_OK);
	stencilTest->Release();


	for (int pass = 0; pass < PASSES; pass++)
	{
		T2Ds[pass]->Release();
	}
}

Deferr::Deferr(ID3D11DeviceContext * gDeviceContext, ID3D11Device * gDevice)
{
	this->gDeviceContext = gDeviceContext;
	this->gDevice = gDevice;

	this->initShaders();
	this->initDefQuad();
	this->initBuffers();
	this->initTexViews();
}

Deferr::~Deferr()
{
}

void Deferr::firstPass(ID3D11Buffer * vertexBuffer, ID3D11Buffer * indexBuffer)
{
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (unsigned int i = 0; i < PASSES; i++)
	{
		this->gDeviceContext->ClearRenderTargetView(this->RTVs[i], black);
	}
	
	this->gDeviceContext->ClearDepthStencilView(this->DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	
	ID3D11RenderTargetView* rtvsToSet[] =
	{
		RTVs[0],
		RTVs[1],
		RTVs[2],
		RTVs[3],
	};

	gDeviceContext->OMSetRenderTargets(PASSES, rtvsToSet, DSV);

	UINT vertexSize = sizeof(Vertex);
	UINT vertexCount = 36;
	UINT indexSize = sizeof(Index);
	UINT offset = 0;

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->gDeviceContext->IASetInputLayout(this->deferrVertexLayout);
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &vertexSize, &offset);
	this->gDeviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);


	this->gDeviceContext->VSSetShader(this->deferrVertexShader, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->deferrPixelShader, nullptr, 0);

	this->gDeviceContext->PSSetSamplers(0, 1, &linearSamplerState);
	this->gDeviceContext->PSSetShaderResources(0, 1, &this->textureResourceView);

	this->gDeviceContext->DrawIndexed(36, 0, 0);
}

void Deferr::finalPass(ID3D11RenderTargetView * RTV, ID3D11DepthStencilView * DSV)
{
	float black[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->gDeviceContext->ClearRenderTargetView(RTV, black);

	gDeviceContext->OMSetRenderTargets(1, &RTV, DSV);

	UINT vertexSize = sizeof(quadVertex);
	UINT vertexCount = 6;
	UINT offset = 0;

	this->gDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->gDeviceContext->IASetVertexBuffers(0, 1, &vertexBufferQuad, &vertexSize, &offset);
	this->gDeviceContext->IASetInputLayout(this->finalVertexLayout);

	this->gDeviceContext->VSSetShader(this->finalVertexShader, nullptr, 0);
	this->gDeviceContext->HSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->DSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->GSSetShader(nullptr, nullptr, 0);
	this->gDeviceContext->PSSetShader(this->finalPixelShader, nullptr, 0);

	this->gDeviceContext->PSSetSamplers(0, 1, &linearSamplerState);
	this->gDeviceContext->PSSetSamplers(1, 1, &pointSamplerState);

	this->gDeviceContext->PSSetShaderResources(0, 2, SRVs);

	this->gDeviceContext->Draw(vertexCount, 0);

	this->gDeviceContext->Flush();
}
