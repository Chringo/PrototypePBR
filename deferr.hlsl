struct VS_IN
{
    float3 Pos : SV_POSITION;
    float3 Normal : Normal;
    float2 UV : TEXCOORD0;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : Normal;
    float2 UV : TEXCOORD0;

    float4 wPos : WORLDPOSITION;
};

cbuffer worldMatrix : register(b0)
{
    matrix worldMatrix;
}

cbuffer viewProj : register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;
}
//-----------------------------------------------------------------------------------------
// VERTEX SHADER
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    matrix VP = mul(viewMatrix, projectionMatrix);
    matrix WVP = mul(worldMatrix, VP);

    output.Pos = mul(float4(input.Pos, 1), WVP);
    output.Normal = input.Normal;
    output.UV = input.UV;
    output.wPos = mul(float4(input.Pos, 1), worldMatrix);

    return output;
}
//-----------------------------------------------------------------------------------------
// PIXEL SHADER
//-----------------------------------------------------------------------------------------
float4 PS_main(VS_OUT input) : SV_Target
{
    return float4(1.0, 1.0, 1.0, 1.0f);
};

