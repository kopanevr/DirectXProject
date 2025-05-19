#include "Window.h"

//

/**
 * @brief
 */
LRESULT CALLBACK Window::WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

/**
 * @brief
 */
BOOL Window::RegisterWindowClass()
{

	return TRUE;
}

/**
 * @brief
 */
BOOL Window::CreateWindowInstance()
{
	if (lpClassName == nullptr || lpWindowName == nullptr) { return FALSE; }

	return TRUE;
}

//

/**
 * @brief
 */
Window::Window(LPCSTR lpClassName, LPCSTR lpWindowName)
{
}

/**
 * @brief
 */
Window::~Window()
{
}
