struct DirectionalLightData
{
    float3 color;
    float3 direction;
    float intensity;
};

cbuffer EnvironmentData : register(b2)
{    
    DirectionalLightData directionLightData;
};

float3 ComputePhongDirectionalLight(
	CameraData camera,
	DirectionalLightData light,
	float3 worldPosition,
	float3 worldNormal,

	float kd,
	float3 id,

	float ks,
	float3 is,

	float shininess
)
{
    float3 N = normalize(worldNormal);
    float3 L = normalize(-light.direction.xyz);
    float3 V = normalize(camera.position.xyz - worldPosition);
    float3 R = reflect(-L, N);
    float NdotL = saturate(dot(N, L));
    float RdotV = saturate(dot(R, V));

    float3 diffuse = kd * id * NdotL;   
    
    float spec = pow(RdotV, shininess);
    float3 specular = ks * spec * is;

    float3 result = (diffuse + specular) * light.color.rgb * light.intensity;
    return result;
}

struct MaterialPSOut
{
    float4 diffuse;
    float4 specular;
    float shininess;
};

void PSMain(inout MaterialPSOut output);

float4 _PSMain(VSOutput input) : SV_TARGET
{
    TextureCoordinate = input.texcoord; 
    MaterialPSOut psOut;
    psOut.diffuse = float4(1, 1, 1, 1);    
    psOut.specular = float4(0,0,0,0);
    psOut.shininess = 0.0; 
    PSMain(psOut);

    
    float3 result = float3(0, 0, 0);

	//ambient light
    float ka = 0.1;
    float3 ia = float3(0.27f, 0.39f, 0.55f) * psOut.diffuse.rgb;
    float3 ambientLight = ka * ia;    
    result = ambientLight;

	//directional light
    result += ComputePhongDirectionalLight(
        cameraData, 
        directionLightData,
        input.worldPosition,
        input.worldNormal,
        1.0, psOut.diffuse.rgb,
        1.0, psOut.specular.rgb,
        psOut.shininess
    );   

    result = psOut.diffuse.rgb;
    return float4(result, 1);
}