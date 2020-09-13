#pragma once
#include "Graphics\Direct3D-11\Direct3dHeaders.h"
#include <wincodec.h>

enum class WIC_LOADER_FLAGS : uint32_t
{
    WIC_LOADER_DEFAULT = 0,
    WIC_LOADER_FORCE_SRGB = 0x1,
    WIC_LOADER_IGNORE_SRGB = 0x2,
    WIC_LOADER_SRGB_DEFAULT = 0x4,
    WIC_LOADER_FIT_POW2 = 0x20,
    WIC_LOADER_MAKE_SQUARE = 0x40,
    WIC_LOADER_FORCE_RGBA32 = 0x80,
};

class TextureLoader
{
public:
    static HRESULT CreateWICTextureFromFileEx(ID3D11Device* d3dDevice,
        const wchar_t* fileName,
        size_t maxsize,
        D3D11_USAGE usage,
        unsigned int bindFlags,
        unsigned int cpuAccessFlags,
        unsigned int miscFlags,
        WIC_LOADER_FLAGS loadFlags,
        ID3D11Resource** texture,
        ID3D11ShaderResourceView** textureView) noexcept;
};