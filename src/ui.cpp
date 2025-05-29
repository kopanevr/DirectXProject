#include "ui.h"

using namespace NUI;
using namespace NDATA;

/**
 * @brief
 */
void UI::Body() noexcept
{
	ImGui::Begin(
		"1",
		nullptr,
		ImGuiWindowFlags_NoTitleBar			|
		ImGuiWindowFlags_AlwaysAutoResize		|
		ImGuiWindowFlags_NoMove
	);

	//

	static bool flag_0 = true;
	static bool flag_1 = false;

	if (ImGui::CollapsingHeader("General") == true)
	{
		ImGui::Checkbox("TEXTURE 1", &flag_0);
		ImGui::Checkbox("TEXTURE 2", &flag_1);
	}

	flags.b_0 = flag_0;
	flags.b_1 = flag_1;

	if (flags.b_2 != flags.b_0 || flags.b_3 != flags.b_1)
	{
		if (flags.b_0 == 1 && flags.b_1 != 1)
		{
			d->payload.texture	= TEXTURES::TEXTURE_0;
		}
		
		if (flags.b_0 != 1 && flags.b_1 == 1)
		{
			d->payload.texture	= TEXTURES::TEXTURE_1;
		}

		flags.b_2 = flags.b_0;
		flags.b_3 = flags.b_1;
	}

	ImGui::Text("FPS:      %d", d->payload.fps);
	ImGui::Text("TEXTURE:  %s", d->PrintTextureName());
	ImGui::Text("WINDOW:");
	ImGui::Text("WIDTH:    %d", d->payload.width);
	ImGui::Text("HEIGHT:   %d", d->payload.height);

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

	d = nullptr;
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
void UI::Run() noexcept
{
	Setup();

	//

	Body();

	//

	Render();
}
