
struct VSInput
{
    float3 position:POSITIONT;
    float3 normal:NORMAL;
    float2 uv0:TEXCOORD0;
};
struct VSOutput
{
    float4 position:SV_POSITION;
    float3 normal:NORMAL;
    float2 uv0:TEXCOORD0;
};

VSOutput VSMain(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1.0f);
    output.normal = input.normal;
    output.uv0 = input.uv0;
    return output; 
}