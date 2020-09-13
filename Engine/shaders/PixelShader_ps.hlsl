#include "Structures.hlsli"

Texture2D Textures[16] : register(t0);
SamplerState SamplerType : register(s0);

#define Case_Sample(index) case index: return Textures[index].Sample(SamplerType, input.tex)


float4 main(PixelIn input) : SV_TARGET
{
	switch (input.texIndex)
	{
		Case_Sample(0);
		Case_Sample(1);
		Case_Sample(2);
		Case_Sample(3);
		Case_Sample(4);
		Case_Sample(5);
		Case_Sample(6);
		Case_Sample(7);
		Case_Sample(8);
		Case_Sample(9);
		Case_Sample(10);
		Case_Sample(11);
		Case_Sample(12);
		Case_Sample(13);
		Case_Sample(14);
		Case_Sample(15);
	}

	return float4( 0.2f, 0.3f, 0.8f, 1.0f );
}