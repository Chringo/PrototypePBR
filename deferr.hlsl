Texture2D albedoTex		 : register(t0);
Texture2D metalTex		 : register(t1);
Texture2D normalTex		 : register(t2);
Texture2D roughTex		 : register(t3);
Texture2D aoTex			 : register(t4);

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

struct GS_OUT
{
    float4 Pos : SV_POSITION;
    float4 Normal : TEXCOORD1;
    float2 UV : TEXCOORD0;
    
    float4 wPos : WORLDPOSITION;

    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;

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
// GEOMETRY SHADER
//-----------------------------------------------------------------------------------------
[maxvertexcount(6)]
void GS_main(triangle VS_OUT input[3],
 inout TriangleStream <GS_OUT> output)
{
    GS_OUT element = (GS_OUT)0;

    float3 faceEdgeA = input[1].wPos.xyz - input[0].wPos.xyz;
    float3 faceEdgeB = input[2].wPos.xyz - input[0].wPos.xyz;
    float2 uvEdge1 = input[1].UV - input[0].UV;
    float2 uvEdge2 = input[2].UV - input[0].UV;

    float3 normal = normalize(-cross(faceEdgeA, faceEdgeB));
    float3 tangent = (uvEdge2[1] * faceEdgeA - uvEdge1[1] * faceEdgeB) * (1 / (uvEdge1[0] * uvEdge2[1] - uvEdge2[0] * uvEdge1[1]));
    tangent = normalize(tangent);

    float3 binormal = normalize(-cross(normal, tangent));

    tangent = mul(float4(tangent, 1), worldMatrix).xyz;
    binormal = mul(float4(binormal, 1), worldMatrix).xyz;

    for (uint vertex = 0; vertex < 3; vertex++)
    {
        element.Pos = input[vertex].Pos;
        element.Normal = mul(float4(input[vertex].Normal, 1.0), worldMatrix);
        element.UV = input[vertex].UV;
       
        element.wPos = input[vertex].wPos;

        element.binormal = binormal;
        element.tangent = tangent;


        output.Append(element);
    }

    //output.RestartStrip();
}


//-----------------------------------------------------------------------------------------
// PIXEL SHADER
//-----------------------------------------------------------------------------------------
PS_OUT PS_main(GS_OUT input)
{
    PS_OUT output = (PS_OUT)0;

    float4 normal;

    output.wPosition = input.wPos;
    output.color = albedoTex.Sample(linearSampler, input.UV);
    output.metalness = metalTex.Sample(linearSampler, input.UV);
    output.normal = normalTex.Sample(linearSampler, input.UV);

    output.normal = (output.normal * 2.0f) - 1.0f;

    output.normal.z = (output.normal.z * -1);


    output.normal = float4((output.normal.x * input.tangent) + (output.normal.y * input.binormal) + (output.normal.z * (input.Normal.xyz)), 1.0);
    output.normal = normalize(output.normal);



    return output;
};

