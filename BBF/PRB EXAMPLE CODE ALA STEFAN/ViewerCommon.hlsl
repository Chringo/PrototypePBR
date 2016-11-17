//--------------------------------------------------------------------------------------
// DrawTri.ps
// Direct3D 11 Shader Model 4.0 Demo
// Copyright (c) Stefan Petersson, 2011
//--------------------------------------------------------------------------------------
//BRA INFO: http://docs.garagegames.com/torque-3d/official/content/documentation/Artist%20Guide/Formats/TextureCompression.html



#define CB_PER_FRAME_REGISTER b0
#define CB_PER_MESH_REGISTER b1
#define CB_PER_SUBMESH_REGISTER b2
#define CB_SKELETON_REGISTER b3
#define CB_LIGHT_REGISTER b4

cbuffer PerFrame : register(CB_PER_FRAME_REGISTER)
{
	matrix matView;
	matrix matViewProjection;
	matrix matViewProjectionInv;
	float4 ViewVector;
	float4 CamPosW;
	float4 CamPosV;
};

cbuffer SkinData : register(CB_SKELETON_REGISTER)
{
	matrix boneArray[50];
};

cbuffer PerMesh : register(CB_PER_MESH_REGISTER)
{
	matrix World;
};

cbuffer PerSubmesh : register(CB_PER_SUBMESH_REGISTER)
{
	matrix GeometricOffset;
	int SkinMethod;
	int HasNormalMap;
};

struct AreaLight
{
	matrix ViewProjection;
	float4 Type_Radius_Width_Height;
	float3 Position;
	float3 Direction;
	float3 Left;
	float3 Up;
	float3 Color;
	float Intensity;
};

cbuffer AreaLights : register(CB_LIGHT_REGISTER)
{
	uint AreaLightCount;
	AreaLight gAreaLights[50];
}



static const float PI = 3.14159265f;

Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D metalTextureMap : register(t2);
Texture2D roughnessMap : register(t3);
Texture2D specularMap : register(t4);
Texture2D emissiverMap : register(t5);
Texture2DArray<float> shadowMaps : register(t6);

SamplerState ColorMapSampler : register(s0);
SamplerComparisonState ShadowSampler : register(s1);

struct VSIn
{
	float3 PosL : POSITION;
	float2 TexCoord : TEXCOORD;

	float3 NormalL : NORMAL;
	float4 TangentL : TANGENT; //w has winding info

	float4 BoneIndices : BLENDINDICES;
	float4 BoneWeights : BLENDWEIGHT;
};

struct GSIn
{
	float3 PosW : POSITION_W;
	float2 TexCoord : TEXCOORD;

	float3 NormalV : NORMAL_V;
	float3 TangentV : TANGENT_V;
	float3 BitangentV : BITANGENT_V;
};

struct PSIn
{
	float4 PosH  : SV_Position;
	float2 TexCoord : TEXCOORD;
	
	float3 PosW : POSITION_W;
	float3 PosV : POSITION_V;
	
	float3 NormalV : NORMAL_V;
	float3 TangentV : TANGENT_V;
	float3 BitangentV : BITANGENT_V;

	float3 CamToPixelV : CAMTOPIXEL_V;
};

struct PS_Shadow_In
{
	float4 PosH : SV_POSITION;     // Projection coord
	float2 TexCoord : TEXCOORD;       // Texture coord
	uint RTIndex : SV_RenderTargetArrayIndex;
};
