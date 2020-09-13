#pragma once
#include "Device.h"

class SamplerState
{
	ID3D11SamplerState* m_SamplerState = nullptr;

public:
	HRESULT Create(const Device& d)
	{
		D3D11_SAMPLER_DESC desc = {};
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.MinLOD = -FLT_MAX;
		desc.MaxLOD =  FLT_MAX;
		desc.MipLODBias = 0.0f;
		desc.MaxAnisotropy = 1;
		desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
		desc.BorderColor[0] = 1.0f;
		desc.BorderColor[1] = 1.0f; 
		desc.BorderColor[2] = 1.0f; 
		desc.BorderColor[3] = 1.0f; 

		HRESULT hr = d->CreateSamplerState(&desc, &m_SamplerState);
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
		}
		return hr;
	}

	ID3D11SamplerState* const * getDoublePointer() const { return &m_SamplerState; }
};