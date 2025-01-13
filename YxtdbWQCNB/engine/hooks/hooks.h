#pragma once
#include "../../kiero/minhook/include/MinHook.h"

#include "../utils/utils.h"

#include "../interfaces/manager.h"



#include "../renderer/renderer.h"

#include "../features/features.h"

#include "../../sdk/globals.h"
#include <dxgi.h>
#include <d3d11.h>

class CHooksManager {
public:
	bool Init();
	void Destroy();
private:

	bool CreateHook(uint8_t* targetAddress, void* hookFunction, void** originalFunction, const char* hookName);
	uint8_t* FindAddress(const char* moduleName, const char* pattern, const char* addressName);


	class CreateMove {
	public:
		typedef void(__fastcall* oCreateMoveFn)(CGameInput*, int, char);
		static oCreateMoveFn oCreateMove;
		static void __fastcall Hook(CGameInput* input, int edx, char a2);
	};
	CreateMove m_CreateMove;

	class PresentScene {
	public:
		typedef HRESULT(__stdcall* oPresentSceneFn) (IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags);
		static oPresentSceneFn oPresentScene;
		HWND outputWindow = nullptr;

		bool init{ };
		ID3D11DeviceContext* context{ };
		ID3D11RenderTargetView* renderTargetView;
	};
	PresentScene m_PresentScene;
	class WindowProc {
	public:
		WNDPROC WndProc;

	};
	WindowProc m_WindowProc;

	class GlowObjects {
	public:
		typedef void(__fastcall* oGlowObjectsFn)(C_GlowProperty*, float*);
		typedef bool(__fastcall* oIsGlowingFn)(C_GlowProperty*);
		static oIsGlowingFn oIsGlowing;
		static oGlowObjectsFn oGlowObjects;

	};
	GlowObjects m_GlowObjects;



	class NoSmokeHook {
	public:
		typedef bool(__fastcall* oNoSmokeFn)(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6);
		static oNoSmokeFn oNoSmoke;
		static bool __fastcall Hook(void* a1, void* a2, void* a3, void* a4, void* a5, void* a6);
	};
	NoSmokeHook m_NoSmoke;

	class RemoveLegs {
	public:
		typedef bool(__fastcall* oRemoveLegsFn)(void*, void*, void*, void*, void*);
		static oRemoveLegsFn oRemoveLegs;
		static bool __fastcall Hook(void* a1, void* a2, void* a3, void* a4, void* a5);
	};
	RemoveLegs m_RemoveLegs;


};
inline CHooksManager* g_pHooksManager = new CHooksManager();