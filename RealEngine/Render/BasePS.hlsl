
Texture2D<float4> baseColorTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);

SamplerState baseSampler : register(s0);

cbuffer MaterialConstants : register(b0)
{
    float4 param1;
}

struct VSOutput
{
    float4 position:SV_POSITION;
    float3 normal:NORMAL;
    float2 uv0:TEXCOORD0;
};

float4 PSMain(VSOutput input) :SV_Target0
{
    int3 Location;
    Location.x = input.uv0.x * 1280;
    Location.y = input.uv0.y * 720;
    Location.z = 0;
    //return float4(input.uv0, 0,1);
    
    //float3 baseColor = baseColorTexture.Load(Location);
    float3 baseColor = baseColorTexture.Sample(baseSampler, float2(input.uv0.r, 1-input.uv0.g));
    
    float gamma = 2.2f;
    baseColor = pow(baseColor.rgb, float3(gamma, gamma, gamma));
    return float4(baseColor, 1.f);
    ////return float4(1.f, 0.f, 0.f, 1.f);
    //return float4(input.uv0.g, input.uv0.g, input.uv0.g, 1.f);
}