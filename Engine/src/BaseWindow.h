#pragma once
#include <Windows.h>

template <typename DERIVED>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED* pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		else
		{
			pThis = (DERIVED*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}

		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	BaseWindow() : m_hwnd(NULL)	{ }

	BOOL Create(HINSTANCE hInstance,
				PCWSTR lpWindowName,
				DWORD dwStyle,
				DWORD dwExStype = 0,
				int x = CW_USEDEFAULT,
				int y = CW_USEDEFAULT,
				int nWidth = CW_USEDEFAULT,
				int nHeight = CW_USEDEFAULT,
				HWND hWndParent = 0,
				HMENU hMenu = 0)
	{
		WNDCLASS wc = {};

		wc.lpfnWndProc = DERIVED::WindowProc;
		wc.hInstance = hInstance;
		wc.lpszClassName = ClassName();

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(
			dwExStype,
			ClassName(),
			lpWindowName,
			dwStyle,
			x,
			y,
			nWidth,
			nHeight,
			hWndParent,
			hMenu,
			hInstance,
			this
		);

		return (BOOL)m_hwnd;
	}

	HWND Window() const { return m_hwnd; }

protected:
	HWND m_hwnd;

	virtual bool ShouldQuit() const = 0;
	virtual PCWSTR ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
};
