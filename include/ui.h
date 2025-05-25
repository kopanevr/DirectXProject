#pragma once

#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class UI final
{
private:
	void body()																						const noexcept;
public:
#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL Init(HWND hWnd, ID3D11Device* pD3DDevice, ID3D11DeviceContext* pID3D11DeviceContext)		noexcept;
	void DeInit()																					noexcept;

	void Run()																						const noexcept;
	void Setup()																					const noexcept;
	void Render()																					const noexcept;
};
