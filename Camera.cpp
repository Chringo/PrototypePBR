#include "Camera.h"

Camera::Camera()
{
	this->position	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, -3.0f)));
	this->target	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f)));
	this->upVector	= DirectX::XMLoadFloat3(&(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)));
}

Camera::~Camera()
{
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
	toReturn = DirectX::XMMatrixPerspectiveFovLH(3.14f*0.45f, float(CLIENT_WIDTH) / float(CLIENT_HEIGHT), 0.5f, 20.0f);
	return toReturn;
}

DirectX::XMMATRIX Camera::getOrthogMatrix() const
{
	DirectX::XMMATRIX toReturn;
	toReturn = DirectX::XMMatrixOrthographicLH(CLIENT_WIDTH, CLIENT_HEIGHT, 0.5f, 20.0f);
	return toReturn;
}
