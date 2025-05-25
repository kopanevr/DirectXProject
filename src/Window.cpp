#include "Window.h"

#include <assert.h>

//

/**
 * @brief
 */
LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		
		return (LRESULT)0;
	case WM_SIZE:

		return (LRESULT)0;
	default:
		break;
	}

	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

/**
 * @brief Зарегестрировать класс окна.
 */
BOOL Window::RegisterWindowClass() const noexcept
{
	if (lpClassName == nullptr) { return FALSE; }

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

	if (RegisterWindowClass() == FALSE) { return FALSE; }

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

//

/**
 * @brief
 */
Window::Window(LPCSTR lpClassName, LPCSTR lpWindowName)
	:	lpClassName(lpClassName),
		lpWindowName(lpWindowName)
{}

/**
 * @brief
 */
Window::~Window()
{
	hInstance		= nullptr;
	hWnd			= nullptr;

	lpClassName		= nullptr;
	lpWindowName		= nullptr;
}
