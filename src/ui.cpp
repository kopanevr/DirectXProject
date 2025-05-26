#include "ui.h"

/**
 * @brief
 */
void UI::Body() const noexcept
{
	ImGui::Begin(
		"1",
		nullptr,
		ImGuiWindowFlags_NoTitleBar			|
		ImGuiWindowFlags_AlwaysAutoResize		|
		ImGuiWindowFlags_NoMove
	);

	//

	if (ImGui::CollapsingHeader("General") == true)
	{
		//
	}

	ImGui::Text("");

	//

	ImGui::End();
}

/**
 * @brief
 */
void UI::Setup() const noexcept
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

/**
 * @brief
 */
void UI::Render() const noexcept
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

//

/**
 * @brief
 */
BOOL UI::Init(HWND hWnd, ID3D11Device* pD3DDevice, ID3D11DeviceContext* pID3D11DeviceContext) noexcept
{
	IMGUI_CHECKVERSION();

	if (ImGui::CreateContext() == nullptr) { return FALSE; }

	if (ImGui_ImplDX11_Init(pD3DDevice, pID3D11DeviceContext) == false) { return FALSE; }

	if (ImGui_ImplWin32_Init(hWnd) == false) { return FALSE; }

	return TRUE;
}

/**
 * @brief
 */
void UI::DeInit() noexcept
{
	ImGui_ImplWin32_Shutdown();
	ImGui_ImplDX11_Shutdown();

	ImGui::DestroyContext();
}

/**
 * @brief
 */
LRESULT UI::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	return ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);
}

/**
 * @brief
 */
void UI::Run() const noexcept
{
	Setup();

	//

	Body();

	//

	Render();
}
