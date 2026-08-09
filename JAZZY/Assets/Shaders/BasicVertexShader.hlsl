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

void VSMain(inout MaterialVSOut output);

VSOutput _VSMain(VSInput input)
{
	/*
    VSOutput output;
    output.position = float4(input.position.x, input.position.y, input.position.z, 1);

	// World Space
    output.position = mul(output.position, m_world);
    // View Space
    output.position = mul(output.position, m_view);
    // Screen Space 
    output.position = mul(output.position, m_projection);
    
    output.color = input.color;
    return output;
    */

    VSOutput output;
    output.position = mul(float4(input.position, 1), affineWorld);

    output.position = mul(output.position, cameraData.view);
    output.position = mul(output.position, cameraData.proj);

    output.color = input.color;
    return output;
}