#include "Pipeline.h"

void Pipeline::AssignShaders()
{
	m_Device.context()->VSSetShader(m_VertexShader.get(), NULL, 0);
	m_Device.context()->PSSetShader(m_PixelShader.get(), NULL, 0);
}

HRESULT Pipeline::CreateLayout()
{
	if (m_Layout) m_Layout->Release();

	const ShaderData& vsBlob = m_VertexShader.getBlob();

	HRESULT hr = m_Device->CreateInputLayout(Layout::Layout,
		Layout::NumElements,
		vsBlob.buffer,
		vsBlob.length,
		&m_Layout);
	m_Device.SetLayout(m_Layout);
	if (FAILED(hr)) LOG_RESULT(hr);
	return hr;
}

HRESULT Pipeline::CreatePipeline()
{
	m_Camera.UpdateIfNeeded();

	HRESULT hr = m_VertexShader.Create(m_Device);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	hr = m_PixelShader.Create(m_Device);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	hr = CreateLayout();
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	hr = m_VertexData.CreateBuffers(m_Device, m_Camera);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	hr = m_TestTexture.CreateFromFile(m_Device, L"C:/Users/Tom/source/repos/Chess/Chess/textures/chessboard.png");
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}


	hr = m_SamplerState.Create(m_Device);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	return hr;
}

void Pipeline::Begin()
{
	ClearBuffers();
	m_Device.SetTargets();

	AssignShaders();
	m_Device.SetLayout(m_Layout);

	m_VertexData.Reset();
	m_VertexData.SetBuffers(m_Device);

	m_PixelShader.SetMultipleResources(m_Device, m_TestTexture);
	m_PixelShader.SetSampler(m_Device, m_SamplerState);

	if (m_Camera.NeedsUpdate())
	{
		m_Camera.UpdateIfNeeded();
		m_VertexData.UpdateConstant(m_Device, m_Camera);
	}

}

HRESULT Pipeline::DrawSquare()
{
	if (m_VertexData.ReachedEnd())
	{
		HRESULT hr = Flush();
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
			return hr;
		}
	}

	m_VertexData.AddSquare(Colour{ 0.3f, 0.2f, 0.8f, 1.0f });
	return S_OK;
}


HRESULT Pipeline::Flush()
{
	HRESULT hr = m_VertexData.UpdateInstances(m_Device);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	const unsigned int NumInstances = m_VertexData.NumInstances();

	m_Device.context()->DrawIndexedInstanced(6, NumInstances, 0, 0, 0);
	return hr;
}

HRESULT Pipeline::End()
{
	HRESULT hr = Flush();
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	hr = m_Device.Present();
	if (FAILED(hr)) LOG_RESULT(hr);
	return hr;
}