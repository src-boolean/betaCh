#include"cheat.h"
#include <cstdint>
static bool noflash = false;

DWORD64 client = (DWORD64)GetModuleHandle("client.dll");


static auto heal = NULL;

void NoFlash()
{


    //constexpr std::ptrdiff_t m_iHealth = 0x344; // int32
    while (true)
    {
        uintptr_t localplayer = *(uintptr_t*)(client + 0x1869D88);
        uintptr_t flashDur = *(uintptr_t*)(client + 0x140C);
        if (localplayer)
        {
            heal = *(uintptr_t*)(localplayer + 0x344); // m_iHealth

            if (flashDur > 0)
                *(uintptr_t*)(localplayer + 0x140C) = 0; // m_flFlashDuration
            if (!noflash) {
                *(uintptr_t*)(localplayer + 0x140C) = 15; // m_flFlashDuration
            }
        }
    }
}
