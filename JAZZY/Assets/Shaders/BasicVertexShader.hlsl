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

cbuffer constant: register(b0)
{
	// row major says that the rows go from top to bottom
	// column major says that the rows go from left to right instead
    // Basically, the column major is the transpose of row major
    row_major float4x4 m_world;
    row_major float4x4 m_view;
    row_major float4x4 m_projection;
    unsigned int m_time;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1);

	// World Space
    output.position = mul(output.position, m_world);
    // View Space
    output.position = mul(output.position, m_view);
    // Screen Space 
    output.position = mul(output.position, m_projection);

    output.position = lerp(float4(input.position, 1), float4(0.0f, 0.0f, 0.0f, 1.0f), (sin(m_time/1000.0f) + 1.0f) / 2.0f);
    //float time = m_time/1000.0f;
    //output.position = float4(input.position.x + time, input.position.y + time, input.position.z + time, 1);
    output.color = input.color;
    return output;
}