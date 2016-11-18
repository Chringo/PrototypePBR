#pragma once
#include "GlobalData.h"
#include <vector>
#include "Mesh.h"

/*
The model class has everything in local space
*/

class Model
{
private:
	struct worldMat
	{
		DirectX::XMFLOAT4X4 world;
	};

	worldMat wm;

	DirectX::XMMATRIX worldMatrix;
	std::vector<Vertex1>vertexData1;

	std::vector<Mesh> Meshes;
	ID3D11Buffer* cbW;
	ID3D11Buffer* vertexBuffer;

	void map(ID3D11DeviceContext* gDeviceContext);
	Model();
public:
	Model(ID3D11Device* gDevice);
	virtual ~Model();

	void appendMesh(meshDesc & mDesc);
	void update(ID3D11DeviceContext* gDeviceContext);

	unsigned int getMeshVertexCount(int meshIndex) const;
	ID3D11Buffer* getVertexBuffer() const;
	ID3D11Buffer* getConstantBufferW() const;

	void shutdown();

	ID3D11Buffer * getMIndexBuffer() const;
	ID3D11Buffer * getMVertexBuffer() const;
	//load from file
	//initialize
};

