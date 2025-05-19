#pragma once

#include <Windows.h>

class Window
{
private:
	static LRESULT CALLBACK WinProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);	// Оконная процедура.

#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL RegisterWindowClass();																// Регистрация класса.
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL CreateWindowInstance();															// Созданик окна.
private:
	HINSTANCE hIstance								= {};
	HWND hWnd										= {};

	LPCSTR lpClassName								= nullptr;
	LPCSTR lpWindowName								= nullptr;

	BOOL startUpFlag								= FALSE;
public:
	Window(LPCSTR lpClassName, LPCSTR lpWindowName);
	~Window();
};
