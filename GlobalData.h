#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <vector>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

UINT const CLIENT_WIDTH = 1280;
UINT const CLIENT_HEIGHT = 720;



struct fileLoaderDesc
{
	ID3D11DeviceContext* gDeviceContext;
	ID3D11Device* gDevice;
};

struct Vertex1
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

struct Vertex2
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT2 UVs;
};

//file structs

struct MainHeader
{
	unsigned int meshes;
	//unsigned int numOfMats;
};

struct MeshHeader
{
	unsigned int vertices;
	unsigned int indexLength;
	//double transformationMatrix[16];
	unsigned int jointCount;
	bool skeleton;
};

struct Index
{
	unsigned int i;
};

struct Vertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT2 UV;
};

struct SkelVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 normal;
	DirectX::XMFLOAT3 tangent;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT4 weights;
	DirectX::XMINT4 influences;
};

struct BoneMatrixPalette
{
	DirectX::XMFLOAT4X4 boneMatrix;
};

struct KeyframeHeader
{
	float timeValue;
	float translation[3];
	float quaternion[4];
	float scale[3];
};

struct AnimationStateHeader
{
	unsigned int keyFrameCount;
};

struct JointHeader
{
	float bindPose[16];
	int jointIndex;
	int parentIndex;

	unsigned int animStateCount;
};

struct meshDesc
{
	fileLoaderDesc * FLDesc;
	Vertex * vertexData;
	Index * indexData;
	DirectX::XMFLOAT4X4 transMat;
	unsigned int vertexCount;
	unsigned int indexCount;
};

struct quadVertex
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT2 UV;
};
//#pragma once
//#pragma region utilStructs
//struct Vector2
//{
//	float u, v;
//};
//struct Vector3
//{
//	float x, y, z;
//	/*put overload function here*/
//};
//struct Vector4
//{
//	float x, y, z, w;
//};
//#pragma endregion
//#pragma region header structs




//#pragma endregion