Texture2D albedoTex : register(t0);
Texture2D metalTex : register(t1);
Texture2D normalTex : register(t2);
Texture2D roughTex : register(t3);

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
    float3 Pos : POSITION;
    float3 Normal : Normal;
    float3 Tangent : TANGENT0;
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
PS_OUT PS_main(VS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    output.wPosition = input.wPos;
    output.color = albedoTex.Sample(linearSampler, input.UV);
    output.metalness = metalTex.Sample(linearSampler, input.UV);
    output.normal = normalTex.Sample(linearSampler, input.UV);

    return output;
};

