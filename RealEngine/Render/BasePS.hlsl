
Texture2D<float4> baseColorTexture : register(t0);
Texture2D<float3> normalTexture : register(t1);

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
    return baseColorTexture.Load(Location);
    
    //return float4(input.normal, 1.f);
}