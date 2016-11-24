#pragma once
#include "GlobalData.h"
#include <vector>

class Mesh
{
public:
	Mesh(meshDesc&);
	Mesh();
	~Mesh();

	ID3D11Buffer * getVertexBuffer() const;
	ID3D11Buffer * getIndexBuffer() const;
private:
	Vertex * vertexData;
	SkelVertex * skelVertexData;
	Index * indexData;
	unsigned int vertexCount;
	unsigned int indexCount;
	DirectX::XMFLOAT4X4 transMat;

	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	ID3D11DeviceContext* gDeviceContext;
	ID3D11Device* gDevice;

	void initBuffers();
};

