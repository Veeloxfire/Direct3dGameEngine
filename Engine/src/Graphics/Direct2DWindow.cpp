#include "Direct2DWindow.h"
#pragma comment(lib, "d2d1")


template <typename T> static void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

void Direct2DWindow::CalculateLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float x = size.width / 2.0f;
		const float y = size.height / 2.0f;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

HRESULT Direct2DWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;
	if (pRenderTarget == NULL)
	{
		RECT rc{};
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		hr = pFactory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
											  D2D1::HwndRenderTargetProperties(m_hwnd, size),
											  &pRenderTarget);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 0);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);

			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}
		}
	}
	return hr;
}

void Direct2DWindow::DiscardGraphicsResources()
{
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
}

LRESULT Direct2DWindow::OnCreate([[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
	if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
	{
		return -1;
	}
	return 0;
}

LRESULT Direct2DWindow::OnDestroy([[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
	DiscardGraphicsResources();
	SafeRelease(&pFactory);
	PostQuitMessage(0);
	return 0;
}

LRESULT Direct2DWindow::OnPaint([[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps = {};
		BeginPaint(m_hwnd, &ps);

		pRenderTarget->BeginDraw();

		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::SkyBlue));
		pRenderTarget->FillEllipse(ellipse, pBrush);

		hr = pRenderTarget->EndDraw();
		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}

	return 0;
}

LRESULT Direct2DWindow::OnSize([[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam)
{
	if (pRenderTarget != NULL)
	{
		RECT rc = {};
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}

	return 0;
}