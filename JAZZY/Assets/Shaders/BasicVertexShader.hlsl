struct VSInput
{
    float3 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

struct VSOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;    
    float3 worldPosition : TEXCOORD1;
    float3 worldNormal : TEXCOORD2;
};

struct CameraData
{
    row_major float4x4 view;
    row_major float4x4 proj;
    float3 position;
};

cbuffer ObjectData : register(b0)
{
    row_major float4x4 affineWorld;
    row_major float4x4 rigidWorld;
};

cbuffer CameraData : register(b1)
{
    CameraData cameraData;
};

struct MaterialVSOut
{
};

static float2 TextureCoordinate = float2(0, 0);
void VSMain(inout MaterialVSOut output);


VSOutput _VSMain(VSInput input)
{
	VSOutput output;
    output.position = mul(float4(input.position, 1), affineWorld);
    output.worldPosition = output.position.xyz;    
    output.worldNormal = normalize(mul(input.normal, (float3x3) rigidWorld));

    output.position = mul(output.position, cameraData.view);
    output.position = mul(output.position, cameraData.proj);
    output.texcoord = input.texcoord;

    TextureCoordinate = input.texcoord;  
    MaterialVSOut vsOut;
    VSMain(vsOut);
    
    return output;
}