struct PSInput
{
    float3 position : POSITION0; //SEMANTIC
    float4 color : COLOR0; //SEMANTIC
};

struct MaterialPSOut
{
    float4 diffuse;
    float4 specular;
    float shininess;
};

void PSMain(inout MaterialPSOut output);

float4 _PSMain(PSInput input) : SV_Target
{
    float4 newColor = input.color;
    return newColor;
}