
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
	//float3 uvw			: TEXCOORD;
};


VertexToPixel main(VertexShaderInput input)
{
	VertexToPixel output;

	output.position = float4(input.position.x, input.position.y, input.position.z, 1.0f);

	return output;
}