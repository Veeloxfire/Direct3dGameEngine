#pragma once
#include "Device.h"

class Buffer
{
protected:
	ID3D11Buffer* m_pBuffer;

	Buffer() : m_pBuffer(NULL) {}
	virtual ~Buffer()
	{
		if (m_pBuffer) m_pBuffer->Release();
	}

	virtual D3D11_USAGE Usage() const { return D3D11_USAGE_DEFAULT; }
	virtual UINT BindFlags() const = 0;
	virtual UINT CPUAccessFlags() const { return 0; }

public:
	template<typename BufferStruct, unsigned int Num>
	HRESULT Create(const Device& d, const BufferStruct* const data)
	{
		D3D11_BUFFER_DESC bufferDesc = {};
		bufferDesc.Usage = Usage();
		bufferDesc.ByteWidth = sizeof(BufferStruct) * Num;
		bufferDesc.BindFlags = BindFlags();
		bufferDesc.CPUAccessFlags = CPUAccessFlags();
		bufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA InitData = {};
		InitData.pSysMem = data;
		InitData.SysMemPitch = 0;
		InitData.SysMemSlicePitch = 0;

		return d->CreateBuffer(&bufferDesc, &InitData, &m_pBuffer);
	}

	ID3D11Buffer* get() const { return m_pBuffer; }
	ID3D11Buffer* operator->() const { return m_pBuffer; }
};

class DynamicBuffer : public Buffer
{
public:
	DynamicBuffer() = default;
	D3D11_USAGE Usage() const override final { return D3D11_USAGE_DYNAMIC; }
	UINT CPUAccessFlags() const override final { return D3D11_CPU_ACCESS_WRITE; }
};

template <typename BType = Buffer>
class VBuffer : public BType
{
public:
	VBuffer() = default;
	UINT BindFlags() const override final { return D3D11_BIND_VERTEX_BUFFER; }

};

template <typename BType = Buffer>
class IBuffer : public BType
{
public:
	IBuffer() = default;
	UINT BindFlags() const override final { return D3D11_BIND_INDEX_BUFFER; }
};

template <typename BType = Buffer>
class CBuffer : public BType
{
public: 
	CBuffer() = default;
	UINT BindFlags() const override final { return D3D11_BIND_CONSTANT_BUFFER; }
};

using VertexBuffer = VBuffer<>;
using InstanceBuffer = VBuffer<DynamicBuffer>;

using IndexBuffer = IBuffer<>;
using ConstantBuffer = CBuffer<DynamicBuffer>;