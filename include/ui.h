 /**
 * @file ui.h
 * @brief
 */

#pragma once

#include <Windows.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

#include "data.h"

using namespace NDATA;

namespace NUI
{
class UI final
{
private:
	void Body()																					noexcept;

	UI(const UI&)																					= delete;
	UI& operator=(const UI&)																			= delete;

	void Setup()																					const noexcept;
	void Render()																					const noexcept;
private:
	struct
	{
		bool b_0	: 1;
		bool b_1	: 1;
		bool b_2	: 1;
		bool b_3	: 1;

		bool		: 4;
	}
	flags = {		  0,
				  0	
	};
public:
	UI()																						= default;
	~UI()																						= default;

#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	BOOL Init(HWND hWnd, ID3D11Device* pD3DDevice, ID3D11DeviceContext* pID3D11DeviceContext)											noexcept;
	void DeInit()																					noexcept;

#if __cplusplus > 201703L
	[[nodiscard]]
#endif
	LRESULT HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	void Run()																					noexcept;
public:
	Data* d																						= &data;
};
}
