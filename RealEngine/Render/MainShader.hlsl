struct PSInput
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

cbuffer GlobalConstants : register(b0)
{
    float4x4 ViewProjMatrix;
}

PSInput VSMain(float3 position : POSITION, float4 color : COLOR)
{
    PSInput result;
    float4x4 localvp = { { 1.0f, 0.0f, 0.0f, 0.0f },
                       { 0.0f, 1.0f, 0.0f, 0.0f },
                       { 0.0f, 0.0f, 1.0f, 0.0f },
                       { 0.0f, 0.0f, 0.0f, 1.0f },
                };
    result.position = mul(ViewProjMatrix, float4(position, 1.0f));
    //result.position = mul(localvp, float4(position, 1.0f));
    //result.position = float4(position, 1.0f);
    result.color = color;

    return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
    return input.color;
    // return float4(0.7,1.0, 0.1, 1.0);
}