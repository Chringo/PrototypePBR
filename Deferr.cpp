#include "Deferr.h"

Deferr::Deferr()
{
}

inline void Deferr::initDefQuad()
{


}

inline void Deferr::initShaders()
{
}

inline void Deferr::initBuffers()
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
}

Deferr::~Deferr()
{
}