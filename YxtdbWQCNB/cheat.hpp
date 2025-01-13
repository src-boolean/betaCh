#include"cheat.h"
#include <cstdint>
static bool noflash = false;

DWORD64 client = (DWORD64)GetModuleHandle("client.dll");

void NoFlash()
{
	

	uintptr_t localplayer = *(uintptr_t*)(client + 0x1868CF8);

	if (localplayer)
	{
		*(uintptr_t*)(localplayer + 0x140C) = 0; // m_flFlashDuration

	}

}