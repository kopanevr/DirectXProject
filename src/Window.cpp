#include "Window.h"

#include <assert.h>

//

/**
 * @brief
 */
LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	const LRESULT rs = pThis->ui.HandleMessage(hWnd, uMsg, wParam, lParam);

	if (rs != (LRESULT)0) { return rs; }

	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);

		return (LRESULT)0;
	case WM_SIZE:
		if (pThis->d3D.SetViewport(hWnd) == TRUE) { break; };

		return (LRESULT)0;
	default:
		break;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

/**
 * @brief Зарегестрировать класс окна.
 */
BOOL Window::RegisterWindowClass() noexcept
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

BOOL Window::UnregisterWindowClass() const noexcept
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
BOOL Window::CreateWindowInstance() noexcept
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
		0,
		0,

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
BOOL Window::DestroyWindowInstance() const noexcept
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

Window* Window::pThis = nullptr;

//

/**
 * @brief
 */
Window::Window(LPCSTR lpClassName, LPCSTR lpWindowName)
	:	lpClassName(lpClassName),
		lpWindowName(lpWindowName)
{
	pThis = this;

	if (RegisterWindowClass() == FALSE) { startUpFlag = FALSE; return; }

	if (CreateWindowInstance() == FALSE) { startUpFlag = FALSE; return; }

	if (d3D.Init(hWnd) == FALSE) { startUpFlag = FALSE; return; };

	if (ui.Init(hWnd, d3D.GetD3DContext()->pD3DDevice, d3D.GetD3DContext()->pD3DDeviceContext) == FALSE) { startUpFlag = FALSE; };
}

/**
 * @brief
 */
Window::~Window()
{
	ui.DeInit();

	d3D.DeInit();

	if (DestroyWindowInstance() == FALSE) {  }

	if (UnregisterWindowClass() == FALSE) {  }

	hInstance		= nullptr;
	hWnd			= nullptr;

	lpClassName		= nullptr;
	lpWindowName		= nullptr;
}

/**
 * @brief
 */
void Window::Loop()
{
	if (startUpFlag == FALSE) { return; }

	//

	MSG msg = {};

    BOOL state = FALSE;

	while (state == FALSE)
	{
		if (PeekMessageA(&msg, nullptr, (UINT)0U, (UINT)0U, PM_REMOVE) == TRUE)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
			{
				state = TRUE;
			}
		}

		if (state == TRUE) { break; }

		//

		d3D.Render();

		//

		ui.Run();
	}
}
