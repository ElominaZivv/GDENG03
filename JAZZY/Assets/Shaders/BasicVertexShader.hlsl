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
    unsigned int m_time;
}

VSOutput main(VSInput input)
{
    VSOutput output;
    output.position = float4(input.position, 1);
    output.position = lerp(float4(input.position, 1), float4(0.0f, 0.0f, 0.0f, 1.0f), (sin(m_time/1000.0f) + 1.0f) / 2.0f);
    //float time = m_time/1000.0f;
    //output.position = float4(input.position.x + time, input.position.y + time, input.position.z + time, 1);
    output.color = input.color;
    return output;
}