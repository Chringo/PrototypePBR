#include "Model.h"



Model::Model()
{
	this->vertexBuffer	= nullptr;
	this->worldMatrix = DirectX::XMMatrixIdentity();
}


Model::~Model()
{

}

void Model::generateTriangle()
{
	Vertex1 v0;
	Vertex1 v1;
	Vertex1 v2;



	//Vertex data
	v0.Pos = DirectX::XMFLOAT3(0, 1, 0);
	v0.Color = DirectX::XMFLOAT4(0, 1, 0, 1); //green

	v1.Pos = DirectX::XMFLOAT3(1, 0, 0);
	v1.Color = DirectX::XMFLOAT4(1, 0, 0, 1); //red

	v2.Pos = DirectX::XMFLOAT3(-1, 0, 0);
	v2.Color = DirectX::XMFLOAT4(0, 0, 1, 1); //blue
	
	this->vertexData1.push_back(v0);
	this->vertexData1.push_back(v1);
	this->vertexData1.push_back(v2);
}



void Model::initializeTriangle(ID3D11Device * gDevice, ID3D11DeviceContext * gDeviceContext, const DirectX::XMFLOAT3& pos)
{
	HRESULT result;

	//raw vertex data
	this->generateTriangle();

	DirectX::XMFLOAT3X3 worldCoord;
	for (int i = 0; i < 3; i++)
	{
		Vertex1 temp;
		temp = this->vertexData1.at(i);

		float x = 0.0f;
		float y = 0.0f;
		float z = 0.0f;

		x = temp.Pos.x;
		y = temp.Pos.y;
		z = temp.Pos.z;

		worldCoord.m[i][0] = x;
		worldCoord.m[i][1] = y;
		worldCoord.m[i][2] = z;
	}

	//convert to xmmatrix
	this->worldMatrix = DirectX::XMLoadFloat3x3(&worldCoord);
	//place it in the world (origin now relative to world instead of local origin)
	this->worldMatrix = DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.ByteWidth = sizeof(Vertex1) * this->vertexData1.size();
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = false;
	desc.StructureByteStride = 0;

	//dynamic buffers does not need initial data
	result = gDevice->CreateBuffer(&desc, nullptr, &this->vertexBuffer);

	D3D11_MAPPED_SUBRESOURCE mappedData;
	
	//map the memory, so that it cant be used in gpu while we change it
	result = gDeviceContext->Map(this->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
	//read the data and save them in a variable
	Vertex1* v = reinterpret_cast<Vertex1*>(mappedData.pData);
	UINT size = this->vertexData1.size();

	//finally write the data into the vertex buffer
	for (UINT i = 0; i < size; i++)
	{
		v[i] = this->vertexData1.at(i);
	}

	gDeviceContext->Unmap(this->vertexBuffer, 0);

}

void Model::appendMesh(meshDesc & mDesc)
{
	//here total model vertices etc can be stored.
	//but i totally shouldnt... its just a test program....
	//this->vertexCou..
	//STHAP IT! DONT JOHN! 
	Mesh mesh(mDesc);
	Meshes.push_back(mesh);
}

ID3D11Buffer* Model::getVertexBuffer() const
{
	return this->vertexBuffer;
}

DirectX::XMMATRIX Model::getWorldModel() const
{
	return this->worldMatrix;
}

void Model::shutdown()
{
	if (this->vertexBuffer != nullptr)
	{
		this->vertexBuffer->Release();
		this->vertexBuffer = nullptr;
	}
}

ID3D11Buffer * Model::getMIndexBuffer() const
{
	return Meshes.at(0).getindexBuffer();
}

ID3D11Buffer * Model::getMVertexBuffer() const
{
	return Meshes.at(0).getVertexBuffer();
}
