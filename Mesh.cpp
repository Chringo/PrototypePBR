#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::Mesh(meshDesc &mDesc)
{
	this->vertexData = mDesc.vertexData;
	this->indexData = mDesc.indexData;
	this->vertexCount = mDesc.vertexCount;
	this->indexCount = mDesc.indexCount;
	this->transMat = mDesc.transMat;

	//for (int i = 0; i < this->vertexCount; i++)
	//{
	////	float temp = vertexData[i].position.y;

	////	vertexData[i].position.y = vertexData[i].position.z;
	////	vertexData[i].position.z = temp;
	
	////	//float tempN = vertexData[i].normal.y;

	////	//vertexData[i].normal.y = vertexData[i].normal.z;
	////	//vertexData[i].normal.z = temp;

	////	vertexData[i].UV.y = 1.0 - vertexData[i].UV.y;
	//	Vertex bajs = this->vertexData[i];
	//}




	this->gDevice = mDesc.FLDesc->gDevice;
	this->gDeviceContext = mDesc.FLDesc->gDeviceContext;
	this->vertexBuffer = nullptr;

	this->initBuffers();
}

void Mesh::initBuffers()
{
	HRESULT result;

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(Vertex) * this->vertexCount;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = false;
	desc.StructureByteStride = 0;


	D3D11_SUBRESOURCE_DATA vData;
	vData.pSysMem = this->vertexData;

	//create vertexbuffer
	result = gDevice->CreateBuffer(&desc, &vData, &this->vertexBuffer);

	D3D11_BUFFER_DESC desc1;
	ZeroMemory(&desc1, sizeof(desc1));
	desc1.Usage = D3D11_USAGE_IMMUTABLE;
	desc1.ByteWidth = sizeof(Index) * indexCount;
	desc1.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc1.CPUAccessFlags = 0;
	desc1.MiscFlags = 0;
	desc1.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA iData;
	iData.pSysMem = indexData;

	//create indexBuffer
	result = gDevice->CreateBuffer(&desc1, &iData, &this->indexBuffer);

}

Mesh::~Mesh()
{
	//if (indexBuffer != nullptr && vertexBuffer != nullptr)
	//{
	//	indexBuffer->Release();
	//	vertexBuffer->Release();
	//}
	//delete vertexData;
	//delete indexData;
}

unsigned int Mesh::getVertexCount() const
{
	return this->indexCount;
}

ID3D11Buffer * Mesh::getVertexBuffer() const
{
	return this->vertexBuffer;
}

ID3D11Buffer * Mesh::getIndexBuffer() const
{
	return this->indexBuffer;
}
