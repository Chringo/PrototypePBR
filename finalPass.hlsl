Texture2D wPosTex : register(t0);
Texture2D colorTex : register(t1);
Texture2D normalTex : register(t2);
Texture2D metalTex : register(t3);

SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);

struct VS_IN
{
    float3 Pos : POSITION;
    float2 UV : TEXCOORD0;
};

struct VS_OUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
};


//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
    VS_OUT output = (VS_OUT) 0;

    output.Pos = float4(input.Pos, 1);
    output.UV = input.UV;

    return output;
}

float4 PS_main(VS_OUT input) : SV_Target
{
    float4 wPosSamp = wPosTex.Sample(pointSampler, input.UV);
    float4 colorSamp = colorTex.Sample(linearSampler, input.UV);
    float4 normSamp = normalTex.Sample(linearSampler, input.UV);
    float4 metalSamp = metalTex.Sample(linearSampler, input.UV);
    
    return colorSamp;
};