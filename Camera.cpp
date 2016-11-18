#include "Camera.h"

Camera::Camera(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext)
{
	this->position	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, -4.5f)));
	this->target	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)));
	this->upVector	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)));

	DirectX::XMMATRIX w = DirectX::XMMatrixIdentity();

	DirectX::XMStoreFloat4x4(&vp.view, DirectX::XMMatrixTranspose(this->getViewMatrix()));
	DirectX::XMStoreFloat4x4(&vp.projection, DirectX::XMMatrixTranspose(this->getProjectionMatrix()));

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(viewProjection);
	gDevice->CreateBuffer(&bufferDesc, NULL, &cbVP);

	D3D11_MAPPED_SUBRESOURCE gMappedResource;
	viewProjection* dataPtr;

	gDeviceContext->Map(cbVP, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &gMappedResource);
	dataPtr = (viewProjection*)gMappedResource.pData;

	dataPtr->view = vp.view;
	dataPtr->projection = vp.projection;

	gDeviceContext->Unmap(cbVP, NULL);
}

Camera::~Camera()
{
	if (cbVP != nullptr)
	{
		cbVP->Release();
	}
}

DirectX::XMMATRIX Camera::getViewMatrix() const
{
	DirectX::XMMATRIX toReturn;
	toReturn = DirectX::XMMatrixLookAtLH(this->position, this->target, this->upVector);

	return toReturn;
}

DirectX::XMMATRIX Camera::getProjectionMatrix() const
{
	DirectX::XMMATRIX toReturn;
	toReturn = DirectX::XMMatrixPerspectiveFovLH(3.14f*0.45f, float(CLIENT_WIDTH) / float(CLIENT_HEIGHT), 0.005f, 20.0f);
	return toReturn;
}

DirectX::XMMATRIX Camera::getOrthogMatrix() const
{
	DirectX::XMMATRIX toReturn;
	toReturn = DirectX::XMMatrixOrthographicLH(CLIENT_WIDTH, CLIENT_HEIGHT, 0.5f, 20.0f);
	return toReturn;
}

ID3D11Buffer * Camera::getConstantBufferVP() const
{
	return cbVP;
}
