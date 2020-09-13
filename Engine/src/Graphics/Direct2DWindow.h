#pragma once
#include "Events\EventWindow.h"
#include <d2d1.h>

class Direct2DWindow : public EventWindow
{
	bool m_Quit = false;

	ID2D1Factory* pFactory = NULL;
	ID2D1HwndRenderTarget* pRenderTarget = NULL;
	ID2D1SolidColorBrush* pBrush = NULL;
	D2D1_ELLIPSE ellipse;

	void CalculateLayout();
	HRESULT CreateGraphicsResources();
	void DiscardGraphicsResources();

	LRESULT OnCreate(WPARAM wParam, LPARAM lParam) override final;
	LRESULT OnDestroy(WPARAM wParam, LPARAM lParam) override final;
	LRESULT OnPaint(WPARAM wParam, LPARAM lParam) override final;
	LRESULT OnSize(WPARAM wParam, LPARAM lParam) override final;

	void Quit() { m_Quit = true; }
public:
	Direct2DWindow() : ellipse() {}

	PCWSTR ClassName() const override { return L"Direct2D Window"; };
	bool ShouldQuit() const override { return m_Quit; }
};