#pragma once
#include "Events\EventWindow.h"
#include "Pipeline.h"

class Direct3DWindow : public EventWindow
{
	friend class EventWindow;
	bool m_Quit = false;
	Pipeline m_Pipeline;

	void Quit() { m_Quit = true; }

	LRESULT OnSize([[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam) override
	{
		const int width = LOWORD(lParam);
		const int height = HIWORD(lParam);
		m_Pipeline.Resize(width, height);
		return DefWindowProc(m_hwnd, WM_SIZE, wParam, lParam);
	}

	LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam) override 
	{
		switch (wParam)
		{
		case VK_LEFT:  m_Pipeline.MoveCamera(-0.1f,  0.0f, 0.0f); return 0;
		case VK_RIGHT: m_Pipeline.MoveCamera( 0.1f,  0.0f, 0.0f); return 0;
		case VK_UP:    m_Pipeline.MoveCamera( 0.0f,  0.1f, 0.0f); return 0;
		case VK_DOWN:  m_Pipeline.MoveCamera( 0.0f, -0.1f, 0.0f); return 0;
		
		default:
			return DefWindowProc(m_hwnd, WM_SIZE, wParam, lParam);
		}
	}

	void UpdateWindowSize()
	{
		RECT rect{};
		GetClientRect(m_hwnd, &rect);

		const float Width = static_cast<float>(rect.right - rect.left);
		const float Height = static_cast<float>(rect.bottom - rect.top);

		m_Pipeline.SetWindowSize(Width, Height);
	}

public:
	Direct3DWindow(LPWSTR vsCSO, LPWSTR psCSO) : m_Pipeline(vsCSO, psCSO)
	{
	}

	HRESULT Render()
	{
		m_Pipeline.Begin();
		HRESULT hr = m_Pipeline.DrawSquare();
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
			return hr;
		}
		hr =  m_Pipeline.End();
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
		}
		return hr;
	}

	HRESULT InitPipeline()
	{ 
		UpdateWindowSize();

		HRESULT hr = m_Pipeline.CreateDevice(m_hwnd);
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
			return hr;
		}
		hr =  m_Pipeline.CreatePipeline();
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
		}
		return hr;
	}

	PCWSTR ClassName() const override { return L"Direct2D Window"; };
	bool ShouldQuit() const override { return m_Quit; }
};