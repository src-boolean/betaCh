#include"cheat.h"
#include <cstdint>
static bool noflash = false;

DWORD64 client = (DWORD64)GetModuleHandle("client.dll");

void NoFlash()
{
	

	
	while (true)
	{
		uintptr_t localplayer = *(uintptr_t*)(client + 0x1869D88);
		uintptr_t flashDur = *(uintptr_t*)(client + 0x140C);
	if (localplayer)
	{
		if (flashDur > 0)
		*(uintptr_t*)(localplayer + 0x140C) = 0; // m_flFlashDuration

	}
	}
}



