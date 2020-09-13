#pragma once
#include "Device.h"

class ShaderResourceView
{
	ID3D11ShaderResourceView* m_ResourceView = nullptr;

public:
	ShaderResourceView() {}

	~ShaderResourceView()
	{
		if (m_ResourceView) m_ResourceView->Release();
	}

	const ID3D11ShaderResourceView* getViewPtr() const { return m_ResourceView; }
	ID3D11ShaderResourceView* getMutableViewPtr() const { return m_ResourceView; }

	ID3D11ShaderResourceView* const * getViewDoublePointer() const { return &m_ResourceView; }
	ID3D11ShaderResourceView** getMutableViewDoublePointer() { return &m_ResourceView; }
};

class ShaderResourceViewArray
{
public:
	virtual unsigned int GetLength() const = 0;
	virtual ID3D11ShaderResourceView** GetPointer() const = 0;

};

template <unsigned int Length>
class FixedLengthShaderResourceViewArray : public ShaderResourceViewArray
{
	ID3D11ShaderResourceView* Array[Length] = {};
	unsigned int NextIndex = 0;

	~FixedLengthShaderResourceViewArray()
	{
		Clear();
	}
public:
	void AppendResource(const ShaderResourceView& view)
	{
		assert(NextIndex >= Length);

		ID3D11ShaderResourceView* const ResourceView = view.getMutableViewPtr();
		ResourceView->AddRef();

		Array[NextIndex] = ResourceView;
		NextIndex++;
	}

	void Clear()
	{
		for (unsigned int i = 0; i < Length; i++)
		{
			if (Array[i]) Array[i]->Release();
		}
	}

	unsigned int GetLength() const override final { return NextIndex; }
	ID3D11ShaderResourceView** GetPointer() const override final { Array; }
};