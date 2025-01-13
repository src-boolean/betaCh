#include "hooks.h"
#include "../../imgui/imgui.h"
#include "../../imgui/imgui_impl_dx11.h"
#include "../../imgui/imgui_impl_win32.h"
#include "../../settings.hpp"

CHooksManager::CreateMove::oCreateMoveFn CHooksManager::CreateMove::oCreateMove = nullptr;
CHooksManager::PresentScene::oPresentSceneFn CHooksManager::PresentScene::oPresentScene = nullptr;
CHooksManager::GlowObjects::oGlowObjectsFn CHooksManager::GlowObjects::oGlowObjects = nullptr;
CHooksManager::GlowObjects::oIsGlowingFn CHooksManager::GlowObjects::oIsGlowing = nullptr;
//CHooksManager::LightingModulation::oLightingModulationFn CHooksManager::LightingModulation::oLightingModulation = nullptr;
//CHooksManager::WorldModulation::oModulateWorldColorFn CHooksManager::WorldModulation::oModulateWorldColor = nullptr;
//CHooksManager::BombCode::oBombCodeFn CHooksManager::BombCode::oBombCode = nullptr;
//CHooksManager::DrawObject::oDrawObjectFn CHooksManager::DrawObject::oDrawObject = nullptr;
//CHooksManager::RenderStart::oRenderStartFn CHooksManager::RenderStart::oRenderStart = nullptr;
//CHooksManager::FrameStage::oFrameStageFn CHooksManager::FrameStage::oFrameStage = nullptr;
//CHooksManager::ForceCrosshair::oForceCrosshairFn CHooksManager::ForceCrosshair::oForceCrosshair = nullptr;
//CHooksManager::RemoveZoomOverlay::oRemoveZoomOverlayFn CHooksManager::RemoveZoomOverlay::oRemoveZoomOverlay = nullptr;
CHooksManager::NoSmokeHook::oNoSmokeFn CHooksManager::NoSmokeHook::oNoSmoke = nullptr;
CHooksManager::RemoveLegs::oRemoveLegsFn CHooksManager::RemoveLegs::oRemoveLegs = nullptr;
//CHooksManager::OverrideView::oOverrideViewFn CHooksManager::OverrideView::oOverrideView = nullptr;
//CHooksManager::FovObject::oFovObjectFn CHooksManager::FovObject::oFovObject = nullptr;
//CHooksManager::ViewModel::oViewModelFn CHooksManager::ViewModel::oViewModel = nullptr;
//CHooksManager::CalcViewModelAngles::oViewModelCalcFn CHooksManager::CalcViewModelAngles::oViewModelCalc = nullptr;
//CHooksManager::NoFlashbangEffect::oNoFlashbangEffectFn CHooksManager::NoFlashbangEffect::oNoFlashbangEffect = nullptr;
//CHooksManager::IsRelativeMouseMode::oIsRelativeMouseModeFn CHooksManager::IsRelativeMouseMode::oIsRelativeMouseMode = nullptr;

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

uint8_t* CHooksManager::FindAddress(const char* moduleName, const char* pattern, const char* addressName) {
	uint8_t* address = g_pUtils->m_Memory.PatternScan(moduleName, pattern);
	if (!address) {
		g_pUtils->m_Console.Error("Failed to find address for", addressName);
	}
	else {
		g_pUtils->m_Console.Log("Found address for", addressName);
	}
	return address;
}

bool CHooksManager::CreateHook(uint8_t* targetAddress, void* hookFunction, void** originalFunction, const char* hookName)
{
	if (MH_CreateHook(targetAddress, hookFunction, originalFunction) != MH_OK) {
		g_pUtils->m_Console.Error("Failed to create Hook", hookName);
		return false;
	}
	else {
		g_pUtils->m_Console.Log("Created Hook", hookName);
		return true;
	}
}

typedef bool(__fastcall* oReduceAimPunchFn)(__int64 a1, float* a2, float* a3);
oReduceAimPunchFn oReduceAimPunch;

bool __fastcall ReduceAimPunch(__int64 a1, float* a2, float* a3)
{
	return false;
}

bool CHooksManager::Init()
{
	uint8_t* IsRelativeMouseModeVtable =  reinterpret_cast<uint8_t*>(g_pUtils->m_VMT.GetVMT(g_pInterfaces->m_Interfaces.pSystemInput, 76));
	uint8_t* CreateMoveAddress = FindAddress("client.dll", "85 D2 0F 85 ? ? ? ? 48 8B C4 44 88 40", "CreateMove");
	uint8_t* GameOverlayAddress = FindAddress("GameOverlayRenderer64.dll", "48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 41 8B E8", "PresentScene");
	uint8_t* GlowObjectAddress = g_pUtils->m_Memory.ResolveRip(FindAddress("client.dll", "E8 ? ? ? ? F3 0F 10 BE ? ? ? ? 48 8B CF", "GlowObject"), 1, 5);
	uint8_t* IsGlowingAddress = g_pUtils->m_Memory.ResolveRip(FindAddress("client.dll", "E8 ? ? ? ? 33 DB 84 C0 0F 84 ? ? ? ? 48 8B 4F", "IsGlowing"),1,5);
	uint8_t* LightingOverrideAddress = FindAddress("scenesystem.dll","48 89 54 24 ? 53 41 56 41 57", "LightingOverride");
	uint8_t* WorldOverrideAddress = FindAddress("scenesystem.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 54 41 56 41 57 48 83 EC ? 4C 8B F9", "ModulateWorldColor");
	uint8_t* DrawObjectAddress = FindAddress("scenesystem.dll", "48 8B C4 48 89 50 ? 53", "DrawObject");
	uint8_t* OnRenderStart = FindAddress("client.dll", "48 89 5C 24 ? 48 89 6C 24 ? 56 57 41 56 48 81 EC ? ? ? ? 4C 8B F1 48 8D 94 24", "OnRenderStart");
	uint8_t* FrameStageAddress = FindAddress("client.dll", "48 89 5C 24 ? 56 48 83 EC ? 8B 05 ? ? ? ? 8D 5A", "FrameStageNotify");
	uint8_t* ForceCrosshairAddress = FindAddress("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0", "ForceCrosshair");
	uint8_t* RemoveScopeOverlayAddress = FindAddress("client.dll", "4C 8B DC 53 56 57 48 83 EC", "RemoveScopeOverlay");
	uint8_t* NoSmokeAddress = FindAddress("client.dll","48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B 9C 24 ? ? ? ? 4D 8B F8", "NoSmoke");
	uint8_t* RemoveLegsAddress = FindAddress("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 48 89 7C 24 ? 55 41 54 41 55 41 56 41 57 48 8D AC 24 ? ? ? ? 48 81 EC ? ? ? ? 48 8B 01", "RemoveLegs");
	uint8_t* OverrideViewAddress = FindAddress("client.dll","48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 41 56 41 57 48 83 EC ? 48 8B FA E8", "OverrideView");
	uint8_t* ChangeFovAddress = FindAddress("client.dll","40 53 48 83 EC ? 48 8B D9 E8 ? ? ? ? 48 85 C0 74 ? 48 8B C8 48 83 C4", "FovObject");
	uint8_t* ChangeViewmodelAddress = FindAddress("client.dll","48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC ? 49 8B E8", "ChangeViewmodel");
	uint8_t* NoShootingPunchAddress = FindAddress("client.dll", "48 89 5C 24 ? 55 56 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? F2 41 0F 10 01", "NoShootingPunch");
	uint8_t* NoFlashbangEffectAddress = FindAddress("client.dll","48 89 5C 24 ? 57 48 83 EC ? 49 8B D8 48 8B F9 E8", "NoFlashbangEffect");
	//uint8_t* TestSig2 = FindAddress("client.dll", "48 89 5C 24 ? 48 89 74 24 ? 57 48 81 EC ? ? ? ? 48 8B FA 48 8B D9", "TestSig");
	MH_Initialize();

	CreateHook(CreateMoveAddress, reinterpret_cast<void*>(&CHooksManager::CreateMove::Hook), reinterpret_cast<void**>(&CHooksManager::CreateMove::oCreateMove), "CreateMove");

	//CreateHook(ForceCrosshairAddress, reinterpret_cast<void*>(&CHooksManager::ForceCrosshair::Hook), reinterpret_cast<void**>(&CHooksManager::ForceCrosshair::oForceCrosshair), "ForceCrosshair");

	//CreateHook(GameOverlayAddress, reinterpret_cast<void*>(&CHooksManager::PresentScene::Hook), reinterpret_cast<void**>(&CHooksManager::PresentScene::oPresentScene), "PresentScene");

	//CreateHook(IsGlowingAddress, reinterpret_cast<void*>(&CHooksManager::GlowObjects::HookIsGlowing), reinterpret_cast<void**>(&CHooksManager::GlowObjects::oIsGlowing), "IsGlowing");

	//CreateHook(GlowObjectAddress, reinterpret_cast<void*>(&CHooksManager::GlowObjects::Hook), reinterpret_cast<void**>(&CHooksManager::GlowObjects::oGlowObjects), "GlowObject");

	//CreateHook(LightingOverrideAddress, reinterpret_cast<void*>(&CHooksManager::LightingModulation::Hook), reinterpret_cast<void**>(&CHooksManager::LightingModulation::oLightingModulation), "LightingOverride");

	//CreateHook(WorldOverrideAddress, reinterpret_cast<void*>(&CHooksManager::WorldModulation::Hook), reinterpret_cast<void**>(&CHooksManager::WorldModulation::oModulateWorldColor), "ModulateWorldColor");

	//CreateHook(DrawObjectAddress, reinterpret_cast<void*>(&CHooksManager::DrawObject::Hook), reinterpret_cast<void**>(&CHooksManager::DrawObject::oDrawObject), "DrawObject");

	//CreateHook(OnRenderStart, reinterpret_cast<void*>(&CHooksManager::RenderStart::Hook), reinterpret_cast<void**>(&CHooksManager::RenderStart::oRenderStart), "OnRenderStart");

	//CreateHook(FrameStageAddress, reinterpret_cast<void*>(&CHooksManager::FrameStage::Hook), reinterpret_cast<void**>(&CHooksManager::FrameStage::oFrameStage), "FrameStageNotify");

	//CreateHook(RemoveScopeOverlayAddress, reinterpret_cast<void*>(&CHooksManager::RemoveZoomOverlay::Hook), reinterpret_cast<void**>(&CHooksManager::RemoveZoomOverlay::oRemoveZoomOverlay), "RemoveScopeOverlay");

	CreateHook(NoSmokeAddress, reinterpret_cast<void*>(&CHooksManager::NoSmokeHook::Hook), reinterpret_cast<void**>(&CHooksManager::NoSmokeHook::oNoSmoke), "NoSmoke");

	CreateHook(RemoveLegsAddress, reinterpret_cast<void*>(&CHooksManager::RemoveLegs::Hook), reinterpret_cast<void**>(&CHooksManager::RemoveLegs::oRemoveLegs), "RemoveLegs");

	//CreateHook(OverrideViewAddress, reinterpret_cast<void*>(&CHooksManager::OverrideView::Hook), reinterpret_cast<void**>(&CHooksManager::OverrideView::oOverrideView), "OverrideView");

	//CreateHook(ChangeFovAddress, reinterpret_cast<void*>(&CHooksManager::FovObject::Hook), reinterpret_cast<void**>(&CHooksManager::FovObject::oFovObject), "FovObject");

	//CreateHook(ChangeViewmodelAddress, reinterpret_cast<void*>(&CHooksManager::ViewModel::Hook), reinterpret_cast<void**>(&CHooksManager::ViewModel::oViewModel), "ViewModel");

	//CreateHook(NoShootingPunchAddress, reinterpret_cast<void*>(&CHooksManager::CalcViewModelAngles::Hook), reinterpret_cast<void**>(&CHooksManager::CalcViewModelAngles::oViewModelCalc), "ViewModelCalc");

	//CreateHook(NoFlashbangEffectAddress, reinterpret_cast<void*>(&CHooksManager::NoFlashbangEffect::Hook), reinterpret_cast<void**>(&CHooksManager::NoFlashbangEffect::oNoFlashbangEffect), "NoFlashbangEffect");

	//CreateHook(IsRelativeMouseModeVtable, reinterpret_cast<void*>(&CHooksManager::IsRelativeMouseMode::Hook), reinterpret_cast<void**>(&CHooksManager::IsRelativeMouseMode::oIsRelativeMouseMode), "IsRelativeMouseMode");

	//CreateHook(TestSig2, reinterpret_cast<void*>(&ReduceAimPunch), reinterpret_cast<void**>(&oReduceAimPunch), "TestSig2");


	MH_EnableHook(MH_ALL_HOOKS);

	return true;
}

void CHooksManager::Destroy()
{
	reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_PresentScene.outputWindow, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_WindowProc.WndProc)));

	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
}

void __fastcall CHooksManager::CreateMove::Hook(CGameInput* input, int edx, char a2)
{
	oCreateMove(input, edx, a2);

	Globals::LocalPlayerController = C_PlayerController::GetLocalPlayerController();
	if (!Globals::LocalPlayerController)
		return oCreateMove(input, edx, a2);
	Globals::LocalPlayerPawn = (C_PlayerPawn*)g_pInterfaces->m_Interfaces.pEntityList->GetClientEntityFromHandle(Globals::LocalPlayerController->GetHandlePawn());
	if (!Globals::LocalPlayerPawn)
		return oCreateMove(input, edx, a2);

	 C_UserCmd* cmd = nullptr;
	
	g_pFeatures->m_Misc.Aimbot(cmd);

}


bool __fastcall CHooksManager::NoSmokeHook::Hook(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6)
{
	if (settings::misc::nosmoke)
		return oNoSmoke(a1, a2, a3, a4, a5, a6);

	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected())
		return true;

	if (!Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return true;

	if (Globals::LocalPlayerPawn->IsAlive())
		return false;


	return true;
}

bool __fastcall CHooksManager::RemoveLegs::Hook(void* a1, void* a2, void* a3, void* a4, void* a5)
{
	if (settings::misc::nosmoke)
		return oRemoveLegs(a1, a2, a3, a4, a5);

	if (!g_pInterfaces->m_Interfaces.pEngineClient->IsConnected())
		return true;

	if (!Globals::LocalPlayerPawn || !Globals::LocalPlayerController)
		return true;

	if (Globals::LocalPlayerPawn->IsAlive())
		return false;


	return true;
}






