Texture2D wPosTex : register(t0);
Texture2D colorTex : register(t1);
Texture2D normalTex : register(t2);
Texture2D metalTex : register(t3);
Texture2D AOTex : register(t4);

SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);

cbuffer worldMatrix : register(b0)
{
	matrix worldMatrix;
}

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


struct HC_LIGHTS
{
	float4 lightPos;
	float4 camPos;
	float4 lightDir;
	float4 lightColor;
	float4 lightAmbient;
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
	HC_LIGHTS lights = (HC_LIGHTS) 0;
	lights.lightPos = float4(5.0f, 5.0f, 5.0f, 1.0f);
	lights.camPos = float4(0.0f, 0.0f, -3.0f, 1.0f);
	lights.lightDir = float4(0.1f, 0.1f, 0.1f, 1.0f);
	lights.lightColor = float4(0.8f, 0.8f, 0.8f, 1.0f);
	lights.lightAmbient = float4(0.2f, 0.2f, 0.2f, 1.0f);

	lights.camPos = mul(lights.camPos, worldMatrix);
	lights.lightPos = mul(lights.lightPos, worldMatrix);
	lights.lightDir = mul(lights.lightDir, worldMatrix);

    float4 wPosSamp = wPosTex.Sample(pointSampler, input.UV);
    float4 colorSamp = colorTex.Sample(linearSampler, input.UV);
    float4 normSamp = normalTex.Sample(linearSampler, input.UV);
    float4 metalSamp = metalTex.Sample(linearSampler, input.UV);



    //LIGHT CALC
	float3 lightDirection = (-lights.lightDir - wPosSamp);

	float lightIntensity = saturate(dot((normSamp.xyz), lightDirection));
	float4 norColor = saturate(lights.lightColor * lightIntensity);

    //float4 AOsamp = AOTex.Sample(linearSampler, input.UV);

	return colorSamp * norColor + colorSamp * lights.lightAmbient;
};