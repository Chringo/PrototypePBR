#include "Model.h"



void Model::map(ID3D11DeviceContext* gDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE gMappedResource;
	worldMat* dataPtr;

	gDeviceContext->Map(cbW, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &gMappedResource);
	dataPtr = (worldMat*)gMappedResource.pData;
	dataPtr->world = wm.world;
	gDeviceContext->Unmap(cbW, NULL);
}

Model::Model(ID3D11Device* gDevice)
{
	this->vertexBuffer	= nullptr;
	this->worldMatrix = DirectX::XMMatrixIdentity();

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(worldMat);
	gDevice->CreateBuffer(&bufferDesc, NULL, &cbW);
}

Model::~Model()
{

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

void Model::update(ID3D11DeviceContext* gDeviceContext)
{
	//MAEK TRANSFORMATIONS TO WORLDMATRIX HERE
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMMatrixRotationY(0.0005f));
	worldMatrix = DirectX::XMMatrixMultiply(worldMatrix, DirectX::XMMatrixRotationX(0.0005f));

	//STORE TRANSFORMATIONS HERE
	DirectX::XMStoreFloat4x4(&wm.world, worldMatrix);

	//THEN MAP CBUFFER
	map(gDeviceContext);
}

ID3D11Buffer* Model::getVertexBuffer() const
{
	return this->vertexBuffer;
}

ID3D11Buffer * Model::getConstantBufferW() const
{
	return cbW;
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
	return Meshes.at(0).getIndexBuffer();
}

ID3D11Buffer * Model::getMVertexBuffer() const
{
	return Meshes.at(0).getVertexBuffer();
}
