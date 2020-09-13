#include "CoInitialize.h"

HRESULT CallCoInitialize()
{
	static bool called = false;

	if (!called)
	{
		HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		if (FAILED(hr))
		{
			LOG_RESULT(hr);
		}
		called = true;
		return hr;
	}

	return S_OK;
}