
cbuffer ConstantData : register(b0) {
	matrix viewMat;
	matrix projMat;
	float time;
};

cbuffer MaterialData : register(b1) {
	matrix worldMat;
	float4 color;
	float specular;
};

struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position
	//float3 normal		: NORMAL;       // Norm color
	//float2 uv			: UV;			// UV color
	//float4 color		: COLOR;        // RGBA color
};

struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR0;
	float time			: TEXCOORD0;
	//float3 uvw			: TEXCOORD;
};


VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	matrix worldViewProj = mul(mul(worldMat, viewMat), projMat);

	output.position = mul(float4(input.position, 1.0f), worldViewProj);
	output.color = color;
	output.time = time;

	return output;
}