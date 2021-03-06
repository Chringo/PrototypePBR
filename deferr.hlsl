Texture2D albedoTex		 : register(t0);
Texture2D metalTex		 : register(t1);
Texture2D normalTex		 : register(t2);
Texture2D roughTex		 : register(t3);
Texture2D aoTex			 : register(t4);

SamplerState linearSampler : register(s0);
SamplerState pointSampler : register(s1);

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
    float3 Tangent : TANGENT0;
    float2 UV : TEXCOORD0;

    float4 wPos : WORLDPOSITION;
};

struct GS_OUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : TEXCOORD1;
    float2 UV : TEXCOORD0;
   
    float3 tangent : TANGENT;
    float3 biTangent : BINORMAL;
    
    float4 wPos : WORLDPOSITION;
};

struct PS_OUT
{
    float4 wPosition : SV_Target0;
    float3 color : SV_Target1;
    float3 normal : SV_Target2;
    float3 metalness : SV_Target3;
    float3 roughness : SV_Target4;
    float3 AO : SV_Target5;
};

float3 normalToWorldSpace(float3 normalMapSample, float3 normal, float3 tangent, float3 biTangent) //Function for normal mapping  
{
 
    // here we build the tbn basis. to transform the sampled normal to texture space
    //then we return the normal and use it with our calculations
 
    //Convert from [0,1] to [-1,1]
    float3 normalT = 2.0f * normalMapSample - 1.0f;
 
    //Build basis
    float3 N = normal;
 
    //Make sure tangent is completely orthogonal to normal
    float3 T = normalize(tangent - dot(tangent, N) * N); //Read page 582
    float3 B = biTangent; //cross(N, T); //Bitangent
 
                                                //Create the "Texture Space" matrix
    float3x3 TBN = float3x3(T, B, N);
 
    //Convert normal from normal map to texture space and store in input.normal
 
    float3 bumpedNormal = mul(normalT, TBN);
 
    return normalize(bumpedNormal);
 
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
    output.Tangent = input.Tangent;
    output.wPos = mul(float4(input.Pos, 1), worldMatrix);

    return output;
}
//-----------------------------------------------------------------------------------------
// GEOMETRY SHADER
//-----------------------------------------------------------------------------------------
[maxvertexcount(6)]
void GS_main(triangle VS_OUT input[3],
 inout TriangleStream <GS_OUT> output)
{
    GS_OUT element = (GS_OUT)0;

    for (uint vertex = 0; vertex < 3; vertex++)
    {
        element.Pos = input[vertex].Pos;
        element.Normal = mul(float4(input[vertex].Normal, 0.0), worldMatrix);
        element.UV = input[vertex].UV;
       
        element.wPos = input[vertex].wPos;

        element.tangent = mul(float4(input[vertex].Tangent, 0.0), worldMatrix);

        element.biTangent = -cross(element.Normal, element.tangent);


        output.Append(element);
    }

    output.RestartStrip();
}


//-----------------------------------------------------------------------------------------
// PIXEL SHADER
//-----------------------------------------------------------------------------------------
PS_OUT PS_main(GS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    float3 normalSamp = normalTex.Sample(pointSampler, input.UV);

    output.wPosition = input.wPos;
    output.color = albedoTex.Sample(pointSampler, input.UV);
    output.metalness = metalTex.Sample(pointSampler, input.UV);
    output.normal = normalToWorldSpace(normalSamp, input.Normal, input.tangent, input.biTangent);
    output.roughness = roughTex.Sample(pointSampler, input.UV);
    output.AO = aoTex.Sample(pointSampler, input.UV);
   


    return output;

  
};

