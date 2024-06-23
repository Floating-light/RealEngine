
struct VSOutput
{
    float4 position:SV_POSITION;
    float3 normal:NORMAL;
    float2 uv0:TEXCOORD0;
};
float4 PSMain(VSOutput input) :SV_Target0
{
    return float4(input.normal, 1.f);
}