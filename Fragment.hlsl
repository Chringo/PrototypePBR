struct VS_OUT
{
	float4 Pos : SV_POSITION;
	float3 Normal : Normal;
	float2 UV : TEXCOORD0;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	return float4(1.0,1.0,1.0, 1.0f);
};