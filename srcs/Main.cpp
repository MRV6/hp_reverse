#include <windows.h>
#include <iostream>

#include "../includes/Debug.h"
#include "../includes/Studs.h"
#include "../includes/Memory.h"
#include "../includes/Player.h"
#include "../includes/NoClip.h"

static void InitModules()
{
    Studs::Init();
    NoClip::Init();
}

static void LoopModules()
{
    Studs::Loop();
    NoClip::Loop();
}

static void MainThread(HMODULE hModule)
{
    FILE* f;
    AllocConsole();
    freopen_s(&f, "CONOUT$", "w", stdout);
    SetConsoleTitleA("HP Reverse");

    Memory::Init();
    InitModules();

    Debug::Print("HP reverse loaded !");

    std::cout << std::endl << "Press END to close." << std::endl;

    while (!GetAsyncKeyState(VK_END) & 0x1)
    {
        LoopModules();
    }

    if (f != 0) fclose(f);
    FreeConsole();

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
    }
    }

    return TRUE;
}