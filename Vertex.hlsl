struct VS_IN
{
	float3 Pos : POSITION;
	float3 Normal : NORMAL;
	float2 UV : TEXCOORD0;
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

cbuffer viewProj:register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;
}
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

    matrix VP = mul(viewMatrix, projectionMatrix);
    matrix WVP = mul(worldMatrix, VP);

    output.Pos = mul(float4(input.Pos, 1), WVP);
	output.Normal = input.Normal;
	output.UV = input.UV;

	return output;
}