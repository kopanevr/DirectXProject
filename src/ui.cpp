#include "ui.h"

#include "timer.h"

using namespace NUI;
using namespace NDATA;

#define DELAY_UPDATE					(long long)	( 30 )
#define INCREMENT									( 10 )

/**
 * @brief
 */
void UI::Body() noexcept
{
	static bool flag_0 = true;
	static bool flag_1 = false;
	static bool flag_2 = false;
	static bool flag_3 = false;

	static float	value_0				= 0.0f;
	static float	value_1				= 0.0f;
	static float	value_2				= 0.0f;
	static int		value_3				= 0U;
	static float	valuePrevious_0		= value_0;
	static float	valuePrevious_1		= value_1;
	static float	valuePrevious_2		= value_2;
	static int		valuePrevious_3		= value_3;

	static Timer timer = {};

	if (timer.isRunning() == false)
	{
		timer.Start(DELAY_UPDATE);
	}

	if (value_3 >= 360)
	{
		flag_2 = false;
	}
	else if (value_3 <= 0)
	{
		flag_2 = true;
	}

	if (timer.isFinished() == true)
	{
		if (flag_2 == true)
		{
			value_3 += INCREMENT;

			flag_0 = true;
			flag_1 = false;
		}
		else
		{
			value_3 -= INCREMENT;

			flag_0 = false;
			flag_1 = true;
		}

		timer.Reset();
	}

	ImGui::Begin(
		"1",
		nullptr,
		ImGuiWindowFlags_NoTitleBar			|
		ImGuiWindowFlags_AlwaysAutoResize		|
		ImGuiWindowFlags_NoMove
	);

	if (ImGui::CollapsingHeader("General") == true)
	{
		ImGui::Checkbox("TEXTURE 1", &flag_0);
		ImGui::Checkbox("TEXTURE 2", &flag_1);
		ImGui::SliderFloat("POSITION X", &value_0, -0.5f, 0.5f, "%.1f");
		ImGui::SliderFloat("POSITION Y", &value_1, -0.5f, 0.5f, "%.1f");
		ImGui::SliderFloat("POSITION Z", &value_2, 0.0f, 1.0f, "%.1f");
		ImGui::SliderInt("ANGLE", &value_3, 0, 360);
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

	if (valuePrevious_0 != value_0)
	{
		valuePrevious_0 = value_0;

		d->payload.x = value_0;
	}

	if (valuePrevious_1 != value_1)
	{
		valuePrevious_1 = value_1;

		d->payload.y = value_1;
	}

	if (valuePrevious_2 != value_2)
	{
		valuePrevious_2 = value_2;

		d->payload.z = value_2;
	}

	if (valuePrevious_3 != value_3)
	{
		valuePrevious_3 = value_3;

		d->payload.a = value_3;
	}

	//ImGui::Text("FPS:        %d", d->payload.fps);
	ImGui::Text("TEXTURE:    %s", d->PrintTextureName());
	ImGui::Text("POSITION X: %.1f", d->payload.x);
	ImGui::Text("POSITION Y: %.1f", d->payload.y);
	ImGui::Text("POSITION Z: %.1f", d->payload.z);
	ImGui::Text("ANGLE:      %d", d->payload.a);
	ImGui::Text("WINDOW:");
	ImGui::Text("WIDTH:      %d", d->payload.width);
	ImGui::Text("HEIGHT:     %d", d->payload.height);

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
