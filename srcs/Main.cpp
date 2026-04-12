#include <windows.h>
#include <iostream>
#include <ctime>

#include "../includes/Debug.h"
#include "../includes/Studs.h"
#include "../includes/Memory.h"
#include "../includes/Entity.h"
#include "../includes/NoClip.h"
#include "../includes/Interface.h"
#include "../includes/Menu.h"

static void InitModules()
{
    Memory::Init();
    Studs::Init();
    NoClip::Init();
}

static void LoopModules()
{
    NoClip::Loop();
    Menu::Loop();
}

static void MainThread(HMODULE hModule)
{
    InitInterface();
    InitModules();

    while (!GetAsyncKeyState(VK_END) & 0x1)
    {
        LoopModules();
    }

    ShutdownInterface();

    MessageBeep(MB_OK);
    FreeLibraryAndExitThread(hModule, 0);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(hModule);
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread, hModule, NULL, NULL);

            if (hThread != 0)
            {
                CloseHandle(hThread);
            }

            break;
        }
        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;
}