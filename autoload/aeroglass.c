/* compile: cl /MD /LD aeroglass.c user32.lib gdi32.lib */

#include <windows.h>
#include <dwmapi.h>

#define EXPORT __declspec(dllexport)

typedef HRESULT (WINAPI *PDwmIsCompositionEnabled)(BOOL *pfEnabled);
typedef HRESULT (WINAPI *PDwmEnableBlurBehindWindow)(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind);



EXPORT int aeroglass_enable(int windowId);
EXPORT int aeroglass_disable(int windowId);


static int aeroglass_apply(HWND hWnd, BOOL enable);
static HMODULE Initialize(HINSTANCE hInstDLL);
static void Finalize(HMODULE handle);



static BOOL _isCompositionEnabled = FALSE;



static HMODULE
Initialize(HINSTANCE hInstDll)
{
	HMODULE handle;
	char dllPath[MAX_PATH];

	HANDLE hDll;
	PDwmIsCompositionEnabled pEnabled;
	BOOL available = FALSE;

	/* load myself to keep instance */
	GetModuleFileName(hInstDll, dllPath, sizeof(dllPath));
	handle = LoadLibrary(dllPath);
	if (!handle)
		return NULL;

	if (!(hDll = LoadLibrary("Dwmapi.dll")))
	{
		FreeLibrary(handle);
		return NULL;
	}

	available = FALSE;
	pEnabled = (PDwmIsCompositionEnabled) GetProcAddress(hDll, "DwmIsCompositionEnabled");
	if (pEnabled)
		pEnabled(&available);
	_isCompositionEnabled = available;

	FreeLibrary(hDll);

	return handle;
}


static void
Finalize(HMODULE handle)
{
	if (handle)
		FreeLibrary(handle);
}


static int
aeroglass_apply(HWND hWnd, BOOL enable)
{
	HANDLE hDll;
	PDwmEnableBlurBehindWindow pBlurWindow;
	DWM_BLURBEHIND bb = { 0 };

	if (!(hDll = LoadLibrary("Dwmapi.dll")))
		return 0;

	if (_isCompositionEnabled)
	{
		pBlurWindow = (PDwmEnableBlurBehindWindow) GetProcAddress(hDll, "DwmEnableBlurBehindWindow");
		if (pBlurWindow)
		{
			bb.fEnable = enable;
			bb.dwFlags = DWM_BB_ENABLE;
			pBlurWindow(hWnd, &bb);
		}
	}
	FreeLibrary(hDll);

	return (int) hWnd;
}


int
aeroglass_enable(int windowId)
{
	return aeroglass_apply((HWND) windowId, TRUE);
}


int
aeroglass_disable(int windowId)
{
	return aeroglass_apply((HWND) windowId, FALSE);
}


BOOL WINAPI
DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	HMODULE handle = NULL;

	switch(fdwReason)
	{
		case DLL_PROCESS_ATTACH:
			handle = Initialize(hinstDLL);
			break;
		case DLL_PROCESS_DETACH:
			Finalize(handle);
			break;
		case DLL_THREAD_ATTACH:
			break;
		case DLL_THREAD_DETACH:
			break;
	}
	return	TRUE;
}
