#pragma once
#include "Device.h"
#include "ShaderResourceView.h"

class Texture2D : public ShaderResourceView
{
	ID3D11Texture2D* m_Texture2D = nullptr;
public:
	ID3D11Resource** getResourceDoublePointer() { return reinterpret_cast<ID3D11Resource**>(&m_Texture2D); }

	Texture2D() {}
	~Texture2D()
	{
		if (m_Texture2D) m_Texture2D->Release();
	}

	HRESULT CreateFromFile(const Device& d, const wchar_t* filename);

	ID3D11Texture2D* get() { return m_Texture2D; }
};