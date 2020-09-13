#include "EventWindow.h"
#include <utility>

LRESULT EventWindow::HandleMessage(const UINT uMsg, const WPARAM wParam, const LPARAM lParam)
{
	//All handled events
	switch (uMsg)
	{
	case WM_CREATE:	return OnCreate(wParam, lParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		Quit();
		return OnDestroy(wParam, lParam);
	case WM_PAINT: return OnPaint(wParam, lParam);
	case WM_SIZE: return OnSize(wParam, lParam);
	case WM_LBUTTONDOWN: return OnLeftButtonDown(wParam, lParam);
	case WM_LBUTTONUP: return OnLeftButtonUp(wParam, lParam);
	case WM_MBUTTONDOWN: return OnMiddleButtonDown(wParam, lParam);
	case WM_MBUTTONUP: return OnMiddleButtonUp(wParam, lParam);
	case WM_RBUTTONDOWN: return OnRightButtonDown(wParam, lParam);
	case WM_RBUTTONUP: return OnRightButtonUp(wParam, lParam);
	case WM_KEYDOWN: return OnKeyDown(wParam, lParam);
	case WM_KEYUP: return OnKeyUp(wParam, lParam);
	}

	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}