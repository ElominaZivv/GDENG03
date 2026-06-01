struct VSInput
{
    float3 position : POSITION0; //SEMANTIC
    float4 color : COLOR0; //SEMANTIC
};

struct VSOutput
{
    float4 position : SV_Position; // System Value (SV)
    float4 color : COLOR0;
};

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1);
    output.color = input.color;
    return output;
}