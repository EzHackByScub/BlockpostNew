#include "includes.h"
#include "framework/il2cpp-init.h"
#include "Cheat.h"
#include "Functions.h"
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

Present oPresent;
HWND window = NULL;
WNDPROC oWndProc;
ID3D11Device* pDevice = NULL;
ID3D11DeviceContext* pContext = NULL;
ID3D11RenderTargetView* mainRenderTargetView;

void InitImGui()
{
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags = ImGuiConfigFlags_NoMouseCursorChange;
	ImGui_ImplWin32_Init(window);
	ImGui_ImplDX11_Init(pDevice, pContext);
}

LRESULT __stdcall WndProc(const HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	if (true && ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam))
		return true;

	return CallWindowProc(oWndProc, hWnd, uMsg, wParam, lParam);
}

bool init = false;
bool menushow = true;
int tabb = 0;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)& pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)& pBackBuffer);
			pDevice->CreateRenderTargetView(pBackBuffer, NULL, &mainRenderTargetView);
			pBackBuffer->Release();
			oWndProc = (WNDPROC)SetWindowLongPtr(window, GWLP_WNDPROC, (LONG_PTR)WndProc);
			InitImGui();
			init = true;
		}

		else
			return oPresent(pSwapChain, SyncInterval, Flags);
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Cheat::RenderWallHack();
	if (GetAsyncKeyState(VK_INSERT) & 1)
		menushow = !menushow;
	if (menushow)
	{
		ImGui::Begin("SCUBUS HACK");
		if (ImGui::Button(("WallHack"), ImVec2(100.f, 0.f)))
			tabb = 0;
		ImGui::SameLine(0.f, 2.f);
		if (ImGui::Button(("AimBot"), ImVec2(100.f, 0.f)))
			tabb = 1;
		ImGui::SameLine(0.f, 2.f);
		if (ImGui::Button(("Misc"), ImVec2(100.f, 0.f)))
			tabb = 2;
		ImGui::SameLine(0.f, 2.f);
		if (ImGui::Button(("Colors"), ImVec2(100.f, 0.f)))
			tabb = 3;
		if (tabb == 0)
		{
			ImGui::Checkbox("WallHack ", &Cheat::wallhackactive);
			ImGui::Checkbox("TeamCheck", &Cheat::teamcheck);
			ImGui::Checkbox("Check SpawnProtect", &Cheat::spawnprotectcheck);
		}
		if (tabb == 1)
		{
			ImGui::Checkbox("Aim Bot", &Cheat::AimBotStartactive);
		}
		if (tabb == 2)
		{
			ImGui::Checkbox("Granade Hack ", &Cheat::attackspecialhook);
			ImGui::Checkbox("Free Cam", &Cheat::FreeCamactive);
		}
		if (tabb == 3)
		{
			ImGui::ColorEdit4("WallHack Color", Cheat::colorWh);
			ImGui::ColorEdit4("WallHack Spawn Protect Color", Cheat::colorWhspwp);
		}
		ImGui::End();
	}


	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}
BOOL __stdcall StartThread(HMODULE hModule, LPTHREAD_START_ROUTINE StartAddress)
{
	return CloseHandle(CreateThread(0, 0, (LPTHREAD_START_ROUTINE)StartAddress, hModule, 0, 0));
}
DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)& oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	return TRUE;
}
DWORD WINAPI FunctTread(HMODULE hMod)
{
	while (true)
	{
		Cheat::FunctionsStart();
	}
	FreeLibraryAndExitThread(hMod, 0);
}
DWORD WINAPI AimBotTread(HMODULE hMod)
{
	while (true)
	{
		Cheat::AimBotStart();
	}
	FreeLibraryAndExitThread(hMod, 0);
}
BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		Functions::nopBytes(reinterpret_cast<uintptr_t>(GetModuleHandle("GameAssembly.dll")) + 0x2D9B16, 7); // insert crash bypass
		init_il2cpp();
		Cheat::height = app::Screen_get_height(nullptr);
		Cheat::width = app::Screen_get_width(nullptr);
		DisableThreadLibraryCalls(hMod);
		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
		StartThread(nullptr, (LPTHREAD_START_ROUTINE)FunctTread);
		StartThread(nullptr, (LPTHREAD_START_ROUTINE)AimBotTread);
		break;
	case DLL_PROCESS_DETACH:
		kiero::shutdown();
		break;
	}
	return TRUE;
}