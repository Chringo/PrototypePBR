struct VS_IN
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
	float4 Weights : WEIGHTS;
	int4 Influences : INFLUENCES;
};

struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : Normal;
	float2 UV : TEXCOORD0;
};

cbuffer worldMatrix:register(b0)
{
	matrix worldMatrix;
}

cbuffer viewProj : register(b1)
{
	matrix viewMatrix;
	matrix projectionMatrix;
}

cbuffer boneMatrixPalette : register(b2)
{
	matrix boneMatrices[36];
}

//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT Skinning_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	/*Initialize 4 weight values to be assigned further in code.*/
	float weights[4] = { 0.f, 0.f, 0.f, 0.f };

	/*To get the 4th weight so that the sum of the weights add up to 1,
	we subtract 1 with the weights 0, 1, 2.*/
	weights[0] = input.Weights.x;
	weights[1] = input.Weights.y;
	weights[2] = input.Weights.z;
	weights[3] = input.Weights.w;

	float3 posSkin = float3(0.f, 0.f, 0.f);
	float3 norSkin = float3(0.f, 0.f, 0.f);

	/*Perform vertex blending.*/
	for (int i = 0; i < 4; i++)
	{
		if (weights[i] > 1e-5f)
		{
			/*The vertex-blended position v´ of any given vertex v, relative to the root frame, is calculated with
			the following format: ((v´ = w0 * v * F0 + w1 * v * F1 + w2 * v * F2 + w3 * v * F3)). Where "w" is the weights
			multiplied with the result of the product that is the vertex position "v" multiplied individually with all
			of the bone transforms that influence it "F".*/
			posSkin += weights[i] * mul(float4(input.Pos, 1.f), boneMatrices[input.Influences[i]]).xyz;

			/*To transform the normals, tangents and bitangents we need to multiply the values with the bone
			transform matrices (as a 3x3 matrix) to exclude translations.*/

			norSkin += weights[i] * mul(input.Normal, (float3x3)boneMatrices[input.Influences[i]]).xyz;
		}
	}

	matrix VP = mul(viewMatrix, projectionMatrix);
	matrix WVP = mul(worldMatrix, VP);

	output.Pos = mul(float4(posSkin, 1), WVP);
	output.Normal = mul(norSkin, worldMatrix);
	output.UV = input.UV;

	return output;
}