#include "Structures.hlsli"

cbuffer VS_CONSTANT_BUFFER : register(b0)
{
	matrix ViewProjMat;
}

PixelIn main(VertexIn inV)
{
	PixelIn output;
	output.tex = inV.tex;
	output.position = mul(ViewProjMat, inV.position);
	output.texIndex = inV.texIndex;
	output.colour = inV.colour;
	return output;
}