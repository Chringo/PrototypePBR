SamplerState linearSampler : register(s0);

cbuffer worldMatrix : register(b0)
{
    matrix worldMatrix;
}

cbuffer viewProj : register(b1)
{
    matrix viewMatrix;
    matrix projectionMatrix;
}

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

struct PS_OUT
{
    float4 wPosition : SV_Target0;
    float4 color : SV_Target1;
    float4 normal : SV_Target2;
    float4 metalness : SV_Target3;
    //float4 roughness : SV_Target4;
};
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
PS_OUT PS_main(VS_OUT input) : SV_Target
{
    PS_OUT output = (PS_OUT)0;

    output.wPosition = input.wPos;
    output.color = (1.0, 1.0, 1.0, 1.0f);
    output.metalness = (1.0, 1.0, 1.0, 1.0);
    output.normal = (1.0, 1.0, 1.0, 1.0);

    return output;
};

