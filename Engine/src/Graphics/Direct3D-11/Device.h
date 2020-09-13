#pragma once
#include "Direct3dHeaders.h"
#include "Log/Log.h"

class Device
{
	ID3D11RenderTargetView* m_pRenderTargetView = NULL;
	ID3D11DepthStencilView* m_pDepthStencilView = NULL;
	IDXGISwapChain* m_pSwapChain = NULL;
	ID3D11Device* m_pDevice = NULL;
	ID3D11DeviceContext* m_pDeviceContext = NULL;

public:
	Device() = default;
	~Device()
	{
		if (m_pRenderTargetView) m_pRenderTargetView->Release();
		if (m_pDepthStencilView) m_pDepthStencilView->Release();
		if (m_pSwapChain) m_pSwapChain->Release();
		if (m_pDevice) m_pDevice->Release();
		if (m_pDeviceContext) m_pDeviceContext->Release();
	}

	HRESULT Create(HWND hwnd);
	HRESULT ResizeBuffers();

	IDXGISwapChain* swapChain() const { return m_pSwapChain; }
	ID3D11Device* device() const { return m_pDevice; }
	ID3D11DeviceContext* context() const { return m_pDeviceContext; }

	HRESULT Present()
	{ 
		HRESULT hr = m_pSwapChain->Present(1, 0);
		if(hr == DXGI_ERROR_DEVICE_REMOVED)
		{
			hr = m_pDevice->GetDeviceRemovedReason();
			LOG_RESULT(hr);
			return hr;
		}
		else if (FAILED(hr))
		{
			LOG_RESULT(hr);
		}
		return hr;
	}

	void SetTargets() { m_pDeviceContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDepthStencilView); }
	void SetLayout(ID3D11InputLayout* layout) { m_pDeviceContext->IASetInputLayout(layout); }

	void ClearBuffers()
	{
		constexpr float fillColour[] = { 0.0f, 0.0f, 0.0f, 1.0f };
		m_pDeviceContext->ClearRenderTargetView(m_pRenderTargetView, fillColour);
		m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}

	ID3D11Device* operator->() const
	{
		return m_pDevice;
	}
};