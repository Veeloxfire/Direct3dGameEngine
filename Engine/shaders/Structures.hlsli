struct VertexIn
{
	float2 tex : TEXCOORD;
	float4 position : POSITION;
	unsigned int texIndex : TEXINDEX;
	float4 colour : COLOUR;
};

struct PixelIn
{
	float2 tex : TEXCOORD;
	float4 position : SV_POSITION;
	unsigned int texIndex : TEXINDEX;
	float4 colour : COLOUR;
};