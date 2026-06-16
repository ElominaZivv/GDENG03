struct PSInput
{
    float3 position : POSITION0; //SEMANTIC
    float4 color : COLOR0; //SEMANTIC
};

float4 main(PSInput input) : SV_Target
{
    float4 newColor = input.color;
    return newColor;
}