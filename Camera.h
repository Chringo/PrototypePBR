#pragma once
#include "GlobalData.h"

class Camera
{
public:
	Camera(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext);
	virtual ~Camera();

	DirectX::XMMATRIX getViewMatrix() const;
	DirectX::XMMATRIX getProjectionMatrix() const;
	DirectX::XMMATRIX getOrthogMatrix() const;

	ID3D11Buffer * getConstantBufferVP() const;
	

private:
	Camera();
	struct viewProjection
	{
		DirectX::XMFLOAT4X4 view;
		DirectX::XMFLOAT4X4 projection;
	};

	viewProjection vp;

	DirectX::XMVECTOR position;
	DirectX::XMVECTOR target;
	DirectX::XMVECTOR upVector;

	ID3D11Buffer * cbVP;


	float perspAngle;
	float nearPlane;
	float farPlane;
};

