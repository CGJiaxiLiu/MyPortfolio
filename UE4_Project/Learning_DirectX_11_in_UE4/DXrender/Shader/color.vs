cbuffer VSConstBuffer
{
    matrix worldMatrix;
	matrix inversedWorldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
	float4 viewPosition;
};

struct VertexInputType
{
    float4 pos : POSITION;
    float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 pos : SV_POSITION;
    float3 normal : NORMAL;
	float3 binormal : BINORMAL;
	float3 tangent : TANGENT;
	float2 tex : TEXCOORD;
	float3 viewDirection : DIRECTION;
	float fogFactor : FOG;
	float clip : SV_ClipDistance0;
};

PixelInputType ColorVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    input.pos.w = 1.0f;
    output.pos = mul(input.pos, worldMatrix);
	output.viewDirection = normalize(viewPosition.xyz - output.pos);
	
    output.pos = mul(output.pos, viewMatrix);
	output.fogFactor = output.pos.z >10 ? pow(1.0f / 2.71828f, ((output.pos.z-10.0f) * 0.1f)) : 1.0f;
	
    output.pos = mul(output.pos, projectionMatrix);
   
	
	output.normal = normalize(mul(input.normal, inversedWorldMatrix));
	output.binormal = normalize(mul(input.binormal, worldMatrix));
	output.tangent = normalize(mul(input.tangent, worldMatrix));
	
	output.tex = input.tex;
	output.clip = 1.0f;

    return output;
}