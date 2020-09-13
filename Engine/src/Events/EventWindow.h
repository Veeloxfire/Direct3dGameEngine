#pragma once
#include "BaseWindow.h"

class EventWindow : public BaseWindow<EventWindow>
{
public:
	friend class BaseWindow<EventWindow>;

	EventWindow() = default;
	~EventWindow() = default;

private:
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override final;
protected:
	virtual void Quit() = 0;
	PCWSTR ClassName() const override { return L"EventWindow"; }

	virtual LRESULT OnCreate(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_CREATE, wParam, lParam); }
	virtual LRESULT OnDestroy([[maybe_unused]] WPARAM wParam, [[maybe_unused]] LPARAM lParam) { return 0; }
	virtual LRESULT OnPaint(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_PAINT, wParam, lParam); }
	virtual LRESULT OnSize(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_SIZE, wParam, lParam); }

	virtual LRESULT OnLeftButtonDown(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_LBUTTONDOWN, wParam, lParam); }
	virtual LRESULT OnLeftButtonUp(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_LBUTTONUP, wParam, lParam); }
	virtual LRESULT OnMiddleButtonDown(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_MBUTTONDOWN, wParam, lParam); }
	virtual LRESULT OnMiddleButtonUp(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_MBUTTONUP, wParam, lParam); }
	virtual LRESULT OnRightButtonDown(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_RBUTTONDOWN, wParam, lParam); }
	virtual LRESULT OnRightButtonUp(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_RBUTTONUP, wParam, lParam); }

	virtual LRESULT OnKeyDown(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_KEYDOWN, wParam, lParam); }
	virtual LRESULT OnKeyUp(WPARAM wParam, LPARAM lParam) { return DefWindowProc(m_hwnd, WM_KEYUP, wParam, lParam); }
};