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
	DirectX::XMMATRIX worldMatrix;
	std::vector<Vertex1>vertexData1;

	std::vector<Mesh> Meshes;
	ID3D11Buffer* vertexBuffer;

public:
	Model();
	virtual ~Model();

	void generateTriangle();
	void initializeTriangle(ID3D11Device* gDevice, ID3D11DeviceContext* gDeviceContext, const DirectX::XMFLOAT3& pos);

	void appendMesh(meshDesc & mDesc);

	ID3D11Buffer* getVertexBuffer() const;
	DirectX::XMMATRIX getWorldModel() const;

	void shutdown();

	ID3D11Buffer * getMIndexBuffer() const;
	ID3D11Buffer * getMVertexBuffer() const;
	//load from file
	//initialize
};

