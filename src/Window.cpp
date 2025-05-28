#include "window.h"

#include <assert.h>

/**
 * @brief
 */
LRESULT CALLBACK window::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const LRESULT rs = pThis->ui.HandleMessage(hWnd, uMsg, wParam, lParam);

	if (rs != (LRESULT)0) { return rs; }

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

		return (LRESULT)0;
	case WM_SIZE:
		pThis->d3D.DestroyTargetView();

		if (pThis->d3D.ResizeSwapChaninBuffers(hWnd) == TRUE) {  }

		if (pThis->d3D.CreateTargetView() == TRUE) {  }

		pThis->d3D.SetViewport(hWnd);

		return (LRESULT)0;
	default:
		break;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

/**
 * @brief Зарегестрировать класс окна.
 */
BOOL window::RegisterWindowClass() noexcept
{
	if (lpClassName == nullptr) { return FALSE; }

	hInstance = (HINSTANCE)GetModuleHandleA(nullptr);

	assert(hInstance != nullptr);

	if (hInstance == nullptr) { return FALSE; }

	WNDCLASSA w = {};

	w.lpfnWndProc		= WinProc;
	w.hInstance			= hInstance;
	w.lpszClassName		= lpClassName;

	const ATOM a = RegisterClassA(&w);

	assert(a != (ATOM)0U);

	if (a == (ATOM)0U) { return FALSE; }

	return TRUE;
}

BOOL window::UnregisterWindowClass() const noexcept
{
	assert(lpClassName != nullptr);
	assert(hInstance != nullptr);

	if (lpClassName == nullptr) { return FALSE; }
	if (hInstance == nullptr) { return FALSE; }

#ifndef NDEBUG
	BOOL r = UnregisterClassA(lpClassName, hInstance);

	assert(r == TRUE);

	return r;
#else
	return UnregisterClassA(lpClassName, hInstance);
#endif
}

/**
 * @brief Создать класс окна.
 */
BOOL window::CreateWindowInstance() noexcept
{
	assert(lpClassName != nullptr);
	assert(lpWindowName != nullptr);

	if (lpClassName == nullptr || lpWindowName == nullptr) { return FALSE; }

	hWnd = CreateWindowA(
		lpClassName,
		lpWindowName,
		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,

		nullptr,
		nullptr,
		hInstance,
		nullptr
	);

	assert(hWnd != nullptr);

	if (hWnd == nullptr) { return FALSE; }

	return TRUE;
}

/**
 * @brief
 */
BOOL window::DestroyWindowInstance() const noexcept
{
	if (hWnd == nullptr) { return FALSE; }

#ifndef NDEBUG
	BOOL r = DestroyWindow(hWnd);

	assert(r == TRUE);

	return r;
#else
	return DestroyWindow(hWnd);
#endif
}

window* window::pThis = nullptr;

//

/**
 * @brief
 */
window::window(LPCSTR lpClassName, LPCSTR lpWindowName)
	:	lpClassName(lpClassName),
		lpWindowName(lpWindowName)
{
	pThis = this;

	//

	if (RegisterWindowClass() == FALSE) { startUpFlag = FALSE; return; }

	if (CreateWindowInstance() == FALSE) { startUpFlag = FALSE; return; }

	if (d3D.Init(hWnd) == FALSE) { startUpFlag = FALSE; return; };

	if (ui.Init(hWnd, d3D.GetD3DContext()->pD3DDevice, d3D.GetD3DContext()->pD3DDeviceContext) == FALSE) { startUpFlag = FALSE; };
}

/**
 * @brief
 */
window::~window()
{
	ui.DeInit();

	d3D.DeInit();

	if (UnregisterWindowClass() == FALSE) {  }

	hInstance		= nullptr;
	hWnd			= nullptr;

	lpClassName		= nullptr;
	lpWindowName		= nullptr;
}

/**
 * @brief
 */
void window::Loop()
{
	if (startUpFlag == FALSE) { return; }

	//

	ShowWindow(hWnd, SW_SHOWNORMAL);
	UpdateWindow(hWnd);

	MSG msg = {};

	RECT rect = {};

	while (msg.message != WM_QUIT)
	{
		fpsCounter.Start();

		if (PeekMessageA(&msg, nullptr, (UINT)0U, (UINT)0U, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			d3D.Render();

			fpsCounter.End();

			ui.data.payload.fps		= fpsCounter.GetFps();

			if (GetWindowRect(hWnd, &rect) != TRUE) {};

			LONG width = rect.right - rect.left;
			LONG height = rect.bottom - rect.top;

			ui.data.payload.width	= (uint16_t)width;
			ui.data.payload.height	= (uint16_t)height;

			ui.data.IsChangedFlag	= true;

			//

			ui.Run();
			d3D.data				= ui.data;
			ui.data.IsChangedFlag	= false;

			//

			d3D.Present();
		}
	}
}
