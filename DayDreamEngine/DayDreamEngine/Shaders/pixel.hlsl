struct VertexToPixel
{
	float4 position		: SV_POSITION;
	float4 color		: COLOR0;
	float time			: TEXCOORD0;
};

struct PixelOutput {
	float4 color		: SV_TARGET;
};

PixelOutput main(VertexToPixel input)
{
	PixelOutput output;
	output.color = float4(input.color.x * abs(sin(input.time)), input.color.y * abs(cos(input.time - 120.0f)), input.color.z * abs(sin(input.time - 240.0f)), 1.0f);
	return output;
}