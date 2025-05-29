 /**
 * @file d3d.h
 * @brief
 */

#pragma once

#include <Windows.h>

#include "ui.h"
#include "d3d.h"

#include "fps_counter.h"

using namespace NUI;
using namespace ND3D;
using namespace NDATA;

class Window final
{
private:
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);								// Оконная процедура.

#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL RegisterWindowClass()							noexcept;							// Регистрация класса.
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL UnregisterWindowClass()							const noexcept;
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL CreateWindowInstance()							noexcept;							// Создание окна.
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL DestroyWindowInstance()							const noexcept;
private:
	HINSTANCE hInstance;
	HWND hWnd									= {};

	LPCSTR lpClassName								= nullptr;
	LPCSTR lpWindowName								= nullptr;

	BOOL startUpFlag								= TRUE;

	UI ui										= {};
	D3D d3D										= {};

	static Window* pThis;

	FPS_Counter fpsCounter;

	Data* d										= &data;
public:
	Window(LPCSTR lpClassName = (LPCSTR)"DefaultClassName", LPCSTR lpWindowName = (LPCSTR)"DefaultWindowName");
	~Window();

	void Loop();
};
