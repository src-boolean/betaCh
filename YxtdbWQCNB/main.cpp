#include "includes.h"
#include "notification.hxx"
#include "engine/utils/utils.h"
#include "engine/hooks/hooks.h"
#include "engine/interfaces/manager.h"
#include "engine/schema/schema.h"
#include "settings.hpp"
#include <iostream>
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

static bool debugmenu = false;
bool init = false;
HRESULT __stdcall hkPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
	if (!init)
	{
		if (SUCCEEDED(pSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pDevice)))
		{
			pDevice->GetImmediateContext(&pContext);
			DXGI_SWAP_CHAIN_DESC sd;
			pSwapChain->GetDesc(&sd);
			window = sd.OutputWindow;
			ID3D11Texture2D* pBackBuffer;
			pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
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
	if (GetAsyncKeyState(VK_INSERT) & 0x8000) {
		if (!menucheat::isInsertPressed) {
			menucheat::open = !menucheat::open;
			menucheat::isInsertPressed = true;
		}


	}
	else
	{
		menucheat::isInsertPressed = false;
	}

	if (menucheat::open) {
		ImGui::Begin("ImGui Window",0, ImGuiWindowFlags_NoSavedSettings);
		if (ImGui::Button("test")) {

			snotification::AddNotification("1");
		}
		snotification::RenderNotifications();

		ImGui::Checkbox("NoFlash", &noflash);

		ImGui::Checkbox("NoSmoke", &menucheat::settings::misc::nosmoke);

		ImGui::Checkbox("Debug Panel", &debugmenu);

		ImGui::End();

		

	}

	if (debugmenu) {
		ImGui::Begin("Debug Panel", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoSavedSettings);
		ImGui::Text("LP Info", menucheat::settings::misc::distances);
		ImGui::Text("info", menucheat::settings::misc::tp);
		ImGui::End();
	}

	ImGui::Render();

	pContext->OMSetRenderTargets(1, &mainRenderTargetView, NULL);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	return oPresent(pSwapChain, SyncInterval, Flags);
}

DWORD WINAPI MainThread(LPVOID lpReserved)
{
	bool init_hook = false;
	do
	{
		if (kiero::init(kiero::RenderType::D3D11) == kiero::Status::Success)
		{
			kiero::bind(8, (void**)&oPresent, hkPresent);
			init_hook = true;
		}
	} while (!init_hook);
	while (true)
	{
		if(noflash)
		NoFlash();
		Sleep(1);

	}
	return TRUE;
}
unsigned long EntryPoint(void* entryPoint)
{
#ifdef _DEBUG
	g_pUtils->m_Console.Init();
#endif // _DEBUG

	g_pUtils->m_Memory.Init();
	g_pInterfaces->Init();
	g_pSchemaManager->Init();
	g_pHooksManager->Init();

	while (!GetAsyncKeyState(VK_DELETE) & 1)
		Sleep(250);

	g_pHooksManager->Destroy();
	g_pUtils->m_Console.Destroy();

	FreeLibraryAndExitThread((HMODULE)entryPoint, 0);
}

//BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID lpReserved)
//{
//	switch (dwReason)
//	{
//	case DLL_PROCESS_ATTACH:
//		DisableThreadLibraryCalls(hMod);
//		CreateThread(nullptr, 0, MainThread, hMod, 0, nullptr);
//		auto handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryPoint, hMod, 0, NULL);
//
//		break;
//	case DLL_PROCESS_DETACH:
//		kiero::shutdown();
//		break;
//	}
//	return TRUE;
//}

BOOL WINAPI DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserve)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{

		DisableThreadLibraryCalls(hModule);
		CreateThread(nullptr, 0, MainThread, hModule, 0, nullptr);
		auto handle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)EntryPoint, hModule, 0, NULL);
		if (handle)
			CloseHandle(handle);
	}
	return TRUE;
}