Texture2D wPosTex : register(t0);
Texture2D colorTex : register(t1);
Texture2D normalTex : register(t2);
Texture2D metalTex : register(t3);
Texture2D roughTex : register(t4);
Texture2D AOTex : register(t5);

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


struct LIGHT
{
	float3 lightPos;
	float3 camPos;
	float3 lightDir;
	float3 lightColor;
	float3 lightAmbient;
};


LIGHT initLight()
{
    LIGHT light;
    light.lightPos = float3(0.0f, 0.0f, -1.5f);
	//lights.lightDir = float4(0.0f, 0.5f, 1.0f, 1.0f);
    light.lightColor = float3(1.0f, 1.0f, 1.0f);
    light.lightAmbient = float3(0.2f, 0.2f, 0.2f);

    return light;
}

LIGHT initCustomLight(float3 pos, float3 color)
{
    LIGHT light;
    light.lightPos = pos;
	//lights.lightDir = float4(0.0f, 0.5f, 1.0f, 1.0f);
    light.lightColor = color;
    light.lightAmbient = float3(0.2f, 0.2f, 0.2f);

    return light;
}

//PBR FUNTIONS
//sRGB n Linear
float3 linearToSRGB( in float3 linearCol)
{
    float3 sRGBLow = linearCol * 12.92;
    float3 sRGBHigh = (pow(abs(linearCol), 1.0 / 2.4) * 1.055) - 0.055;
    float3 sRGB = (linearCol <= 0.0031308) ? sRGBLow : sRGBHigh;
    return sRGB;
}

float3 sRGBtoLinear(in float3 sRGBCol)
{
    float3 sRGBLow = sRGBCol / 12.92;
    float3 sRGBHigh = (pow(abs(sRGBCol), 1.0 / 2.4) / 1.055) - 2.4;
    float3 linearRGB = (sRGBCol <= 0.04045) ? sRGBLow : sRGBHigh;
    return linearRGB;
}

float3 schlick(float3 f0, float f90, float u) //f0 is spec/metalness, f90 is spec/metal factor
{
    return f0 + (f90 - f0) * pow(1.0f - u, 5.0f);
}

float DisneyDiffuse(float NdotV, float NdotL, float LdotH, float linearRoughness)
{
    float energyBias = lerp(0, 0.05, linearRoughness);
    float energyFactor = lerp(1.0, 1.0 / 1.51, linearRoughness);
    float fd90 = energyBias + 2.0 * LdotH * LdotH * linearRoughness;
    float3 f0 = float3(1.0, 1.0, 1.0);
    float lightScatter = schlick(f0, fd90, NdotL).r;
    float viewScatter = schlick(f0, fd90, NdotV).r;
    
    return lightScatter * viewScatter * energyFactor;
}

float V_SmithGGXCorrelated(float NdotL, float NdotV, float alphaG)
{
    // Original formulation of G_SmithGGX Correlated
    // lambda_v = (-1 + sqrt ( alphaG2 * (1 - NdotL2 ) / NdotL2 + 1)) * 0.5 f;
    // lambda_l = (-1 + sqrt ( alphaG2 * (1 - NdotV2 ) / NdotV2 + 1)) * 0.5 f;
    // G_SmithGGXCorrelated = 1 / (1 + lambda_v + lambda_l );
    // V_SmithGGXCorrelated = G_SmithGGXCorrelated / (4.0 f * NdotL * NdotV );

    // This is the optimize version
    float alphaG2 = alphaG * alphaG;
    // Caution : the " NdotL *" and " NdotV *" are explicitely inversed , this is not a mistake .
    float Lambda_GGXV = NdotL * sqrt((-NdotV * alphaG2 + NdotV) * NdotV + alphaG2);
    float Lambda_GGXL = NdotV * sqrt((-NdotL * alphaG2 + NdotL) * NdotL + alphaG2);

    return 0.5f / (Lambda_GGXV + Lambda_GGXL);
}


float GGX(float NdotH, float m)
{
    //divide by PI happens later
    float m2 = m * m;
    float f = (NdotH * m2 - NdotH) * NdotH + 1;
    return m2 / (f * f);
}



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

   
    float3 camPos = float3(0.0f, 0.0f, -1.5f);
    uint lightCount = 3;
    float Pi = 3.14159265359;
    LIGHT light[3]; 
    light[0] = initCustomLight(float3(0.0, -0.8, -1.3), float3(0.6, 0.6, 0.6));
    light[1] = initCustomLight(float3(0.0, 0.0, -1.5), float3(0.5, 0.5, 0.5));
    light[2] = initCustomLight(float3(0.5, 1.2, -1.0), float3(0.4, 0.4, 0.4));

    //SAMPLING
    float4 wPosSamp = wPosTex.Sample(pointSampler, input.UV);
    float3 metalSamp = (metalTex.Sample(linearSampler, input.UV)).rgb;
    float3 colorSamp = (colorTex.Sample(linearSampler, input.UV)).rgb;
    float3 N = (normalTex.Sample(linearSampler, input.UV)).rgb;
    float3 AOSamp = (AOTex.Sample(linearSampler, input.UV)).rgb;
    float3 roughSamp = (roughTex.Sample(linearSampler, input.UV)).rgb;

    float4 diffuseLight = float4(0, 0, 0, 0);
    float4 specularLight = float4(0, 0, 0, 0);

  

    float sRGBrough = linearToSRGB(roughSamp).r;
    float linearRough = roughSamp.r;
    //FOR EACH LIGHT
    for (uint i = 0; i < lightCount; i++)
    {
        //PBR variables 
        float3 V = normalize(float3(0.0f, 0.0f, 1.0f)); //camDir
        float3 L = normalize((wPosSamp.xyz) - light[i].lightPos);
        float3 H = normalize(V + L);
        float lightPower = 0;

        float LdotH = saturate(dot(L, H));
        float NdotH = saturate(dot(N, H));
        float NdotL = saturate(dot(N, L));
        float VdotH = saturate(dot(V, H));
        float NdotV = saturate(dot(N, V));



        //if (dot(camPos - light.lightPos, normalize(light.lightDir)) > 0) //just for lights with direction.

        //else //lights with no direction


        lightPower = 1.0f; //could add falloff factor
        
        //DO SHADOW STUFF HERE

        //DIFFUSE
        float fd = DisneyDiffuse(NdotV, NdotL, LdotH, roughSamp.r) /*/Pi*/; //roughness should be linear
        diffuseLight += float4(fd.xxx * light[i].lightColor * lightPower * colorSamp.rgb, 1);

        //SPECULAR
        float3 f = schlick(metalSamp.r, 1, LdotH);
        float vis = V_SmithGGXCorrelated(NdotV, NdotL, sRGBrough); //roughness should be sRGB
        float d = GGX(NdotH, sRGBrough); //roughness should be sRGB


        float3 fr = d * f * vis / Pi;

        
        specularLight += float4(fr * metalSamp.r * light[i].lightColor * lightPower, 1);

        //return f.rgbr;
        //return specularLight;
    }

    return diffuseLight;
    //float4 redspec = (specularLight.r, 0.0, 0.0, 0.0);




    ////STANDARD NORMAL/LIGHT CALC N RENDER
    //float3 lightDirection = normalize(light.lightPos - (wPosSamp.xyz));
    //float lightIntensity = saturate(dot(N, lightDirection));

    //float3 norColor = saturate(light.lightColor * lightIntensity);
    
    //return float4(saturate(colorSamp.xyz * norColor).xyz + (colorSamp.xyz * light.lightAmbient), 1.0);
    ////END

    //float4 AOsamp = AOTex.Sample(linearSampler, input.UV);


    //return normSamp;
};
