#include <windows.h>
#include <iostream>
#include <ctime>

#include "../includes/Studs.h"
#include "../includes/Memory.h"
#include "../includes/Entity.h"
#include "../includes/NoClip.h"
#include "../includes/Interface.h"
#include "../includes/Menu.h"
#include "../includes/Model.h"
#include "../includes/Logs.h"
#include "../includes/Main.h"

#include "../vendor/minhook/include/MinHook.h"

_sub_52BED0 orig_sub_52BED0 = nullptr;

std::queue<std::function<void()>> Main::funcQueue = {};
std::mutex Main::funcQueueMutex;

void Main::InitModules()
{
    Memory::Init();
    Studs::Init();
    NoClip::Init();
    Model::Init();
}

void Main::LoopModules()
{
    NoClip::Loop();
    Menu::Loop();
}

void Main::Shutdown(HMODULE hModule)
{
    ShutdownInterface();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_Uninitialize();

    MessageBeep(MB_OK);
    FreeLibraryAndExitThread(hModule, 0);
}

// Random function running every frame
// We hook it so we can call game functions in the game thread, avoiding deadlocks
float __fastcall sub_52BED0_hook(float* a1, float* a2, float* a3)
{
    Main::funcQueueMutex.lock();

    if (!Main::funcQueue.empty())
    {
        Main::funcQueue.front()();
        Main::funcQueue.pop();
    }

    Main::funcQueueMutex.unlock();

    return orig_sub_52BED0(a1, a2, a3);
};

void Main::Init(HMODULE hModule)
{
    if (MH_Initialize() != MH_OK)
    {
        Shutdown(hModule);
        return;
    }

    Logs::Add("Minhook initialized !");

    InitInterface();
    Main::InitModules();

    if (MH_CreateHook(reinterpret_cast<void*>(Memory::GetBaseAddress() + 0x52BED0), &sub_52BED0_hook, reinterpret_cast<void**>(&orig_sub_52BED0)) != MH_OK)
    {
        Shutdown(hModule);
        return;
    }

    MH_EnableHook(MH_ALL_HOOKS);

    while (!GetAsyncKeyState(VK_END) & 0x1)
    {
        LoopModules();
    }

    Shutdown(hModule);
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DisableThreadLibraryCalls(hModule);
            HANDLE hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Main::Init, hModule, NULL, NULL);

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