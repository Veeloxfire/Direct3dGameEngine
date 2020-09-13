#include "Graphics/Direct3D-11/Direct3DWindow.h"
#include "Log/Log.h"
#include "Util/CoInitialize.h"

#include <fstream>

#define PATH L"./../bin/Win32/Debug/"

int WINAPI wWinMain(_In_ HINSTANCE hInstance, [[maybe_unused]] _In_opt_ HINSTANCE hPrevInstance, [[maybe_unused]] _In_ PWSTR pCmdLine, _In_ int nCmdShow)
{
	CallCoInitialize();
	Direct3DWindow win(PATH L"VertexShader_vs.cso", PATH L"PixelShader_ps.cso");

	if (!win.Create(hInstance, L"Direct2D", WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}

	HRESULT hr = win.InitPipeline();
	if (FAILED(hr))
	{
		LOG_RESULT(hr);
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	MSG msg = {};
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			hr = win.Render();
			if (FAILED(hr))
			{
				LOG_RESULT(hr);
				PostQuitMessage(0);
				return 0;
			}
		}

		if(win.ShouldQuit())
		{
			return 0;
		}
	}
}

