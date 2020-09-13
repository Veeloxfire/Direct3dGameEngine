#include "Texture2D.h"
#include "TextureLoader/TextureLoader.h"


HRESULT Texture2D::CreateFromFile(const Device& d, const wchar_t* filename)
{
	HRESULT hr = TextureLoader::CreateWICTextureFromFileEx(
		d.device(),
		filename,
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		WIC_LOADER_FLAGS::WIC_LOADER_DEFAULT,
		getResourceDoublePointer(),
		getMutableViewDoublePointer()
	);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
	}
	return hr;
}