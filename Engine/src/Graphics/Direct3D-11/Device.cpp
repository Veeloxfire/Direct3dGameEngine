#include "Device.h"

#pragma comment(lib, "D3D11.lib")


HRESULT Device::Create(HWND hwnd)
{
	//Temp error
	if (m_pRenderTargetView || m_pSwapChain || m_pDevice || m_pDeviceContext) return HRESULT_FROM_WIN32(ERROR_ACCESS_DENIED);

	UINT Flags = 0;

#ifdef DEBUG
	Flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL FeatureLevel = (D3D_FEATURE_LEVEL)NULL;//Could be a member variable if needed
	HRESULT hr = D3D11CreateDevice(NULL,
								   D3D_DRIVER_TYPE_HARDWARE,//Driver mode
								   NULL,
								   Flags,
								   NULL,//Feature levels array pointer
								   0,//Feature levels array size
								   D3D11_SDK_VERSION,
								   &m_pDevice,
								   &FeatureLevel,
								   &m_pDeviceContext);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	IDXGIDevice* dxgiDevice = NULL;
	hr = m_pDevice->QueryInterface<IDXGIDevice>(&dxgiDevice);
	if (FAILED(hr))
	{
		if(dxgiDevice) dxgiDevice->Release();
		LOG_RESULT(hr);
		return hr;
	}

	IDXGIAdapter* dxgiAdapter = NULL;
	hr = dxgiDevice->GetAdapter(&dxgiAdapter);
	if (FAILED(hr))
	{
		dxgiDevice->Release();
		if (dxgiAdapter) dxgiAdapter->Release();
		LOG_RESULT(hr);
		return hr;
	}

	IDXGIFactory* dxgiFactory = NULL;
	hr = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(hr))
	{
		if (dxgiFactory) dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();
		LOG_RESULT(hr);
		return hr;
	}

	DXGI_SWAP_CHAIN_DESC sd = {};
	ZeroMemory(&sd, sizeof(sd));

	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.RefreshRate.Numerator = 0;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 3;
	sd.OutputWindow = hwnd; //window handle
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	hr = dxgiFactory->CreateSwapChain(m_pDevice, &sd, &m_pSwapChain);
	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	return ResizeBuffers();
}


HRESULT Device::ResizeBuffers()
{
	m_pDeviceContext->ClearState();
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pDepthStencilView) m_pDepthStencilView->Release();

	HRESULT hr = m_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	ID3D11Texture2D* buffer = NULL;
	hr = m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&buffer));
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	hr = m_pDevice->CreateRenderTargetView(buffer, 0, &m_pRenderTargetView);
	if (FAILED(hr))
	{
		buffer->Release();
		LOG_RESULT(hr);
		return hr;
	}

	D3D11_TEXTURE2D_DESC td = {};
	buffer->GetDesc(&td);
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	buffer->Release();

	hr = m_pDevice->CreateTexture2D(&td, NULL, &buffer);
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return hr;
	}

	hr = m_pDevice->CreateDepthStencilView(buffer, NULL, &m_pDepthStencilView);
	buffer->Release();

	SetTargets();

	D3D11_VIEWPORT vp = {};
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = (FLOAT)td.Width;
	vp.Height = (FLOAT)td.Height;
	vp.MinDepth = 0;
	vp.MaxDepth = 0;

	m_pDeviceContext->RSSetViewports(1, &vp);

	if (FAILED(hr)) LOG_RESULT(hr);
	return hr;
}