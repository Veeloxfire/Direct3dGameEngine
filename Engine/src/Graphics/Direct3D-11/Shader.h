#pragma once

#include "Device.h"
#include "ShaderResourceView.h"
#include "SamplerState.h"
#include <utility>

struct ShaderData
{
	char* buffer = nullptr;
	unsigned int length = 0;
	
	static ShaderData LoadFromFile(LPCWSTR _In_ FileName);
};


template<typename ShaderType>
class Shader
{
protected:
	ShaderType* m_Shader = NULL;
	ShaderData blob;

	Shader(LPCWSTR _In_ FileName)
		: blob(ShaderData::LoadFromFile(FileName))
	{}

	virtual ~Shader()
	{
		if (m_Shader) m_Shader->Release();
		delete[] blob.buffer;
	}

	virtual void SetShaderResources(const Device& d, unsigned int StartSlot, unsigned int NumberOfSlots, ID3D11ShaderResourceView* const* ViewArray) = 0;
public:

	virtual HRESULT Create(const Device& d) = 0;
	virtual void SetSampler(const Device& d, const SamplerState& s) const = 0;

	void SetResource(const Device& d, const ShaderResourceView& r) const
	{
		SetShaderResources(d, 0, 1, r.getViewDoublePointer());
	}

	void SetResourceArray(const Device& d, const ShaderResourceViewArray& r) const
	{
		SetShaderResources(d, 0, r.GetLength(), r.GetPointer());
	}

	template <typename ... Ts>
	void SetMultipleResources(const Device& d, const Ts& ... ResourceViews)
	{
		static_assert((std::is_base_of_v<ShaderResourceView, Ts> && ...), "Can only set ShaderResourceView objects");

		static constexpr unsigned int NumViews = sizeof...(ResourceViews);
		ID3D11ShaderResourceView* const ViewArray[NumViews] = { ResourceViews.getMutableViewPtr()... };
		
		SetShaderResources(d, 0, NumViews, ViewArray);
	}

	inline ShaderType* get() const { return m_Shader; }
	inline const ShaderData& getBlob() const { return blob; }
};

class VertexShader : public Shader<ID3D11VertexShader>
{
public:
	VertexShader(LPCWSTR _In_ FileName) : Shader(FileName) {}
	~VertexShader() override final = default;

	HRESULT Create(const Device& d) override final
	{
		if (m_Shader) m_Shader->Release();
		return d->CreateVertexShader(blob.buffer, blob.length, nullptr, &m_Shader);
	}

	void SetSampler(const Device& d, const SamplerState& s) const override final
	{
		d.context()->VSSetSamplers(0, 1, s.getDoublePointer());
	}

protected:
	void SetShaderResources(const Device& d, unsigned int StartSlot, unsigned int NumberOfSlots, ID3D11ShaderResourceView* const* ViewArray) override final
	{
		d.context()->VSSetShaderResources(StartSlot, NumberOfSlots, ViewArray);
	}
};

class PixelShader : public Shader<ID3D11PixelShader>
{
public:
	PixelShader(LPCWSTR _In_ FileName) : Shader(FileName) {}
	~PixelShader() override final = default;

	HRESULT Create(const Device& d) override final
	{
		if (m_Shader) m_Shader->Release();
		return d->CreatePixelShader(blob.buffer, blob.length, NULL, &m_Shader);
	}

	void SetSampler(const Device& d, const SamplerState& s) const override final
	{
		d.context()->PSSetSamplers(0, 1, s.getDoublePointer());
	}

protected:
	void SetShaderResources(const Device& d, unsigned int StartSlot, unsigned int NumberOfSlots, ID3D11ShaderResourceView* const* ViewArray) override final
	{
		d.context()->PSSetShaderResources(StartSlot, NumberOfSlots, ViewArray);
	}
};