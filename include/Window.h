#pragma once

#include <Windows.h>

class Window final
{
private:
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// Оконная процедура.

#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL RegisterWindowClass()						const noexcept;							// Регистрация класса.
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL UnregisterWindowClass()					const noexcept;
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL CreateWindowInstance()						noexcept;								// Создание окна.
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL DestroyWindowInstance()					const noexcept;
private:
	HINSTANCE hInstance								= {};
	HWND hWnd										= {};

	LPCSTR lpClassName								= nullptr;
	LPCSTR lpWindowName								= nullptr;

	BOOL startUpFlag								= FALSE;
public:
	Window(LPCSTR lpClassName, LPCSTR lpWindowName);
	~Window();
};
