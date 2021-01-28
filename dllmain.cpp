#include "includes.h"

unsigned long __stdcall onAttach(LPVOID lpParam) {
#ifdef _DEBUG
    AllocConsole();
    FILE* file = nullptr;
    freopen_s(&file, "CONIN$", "r", stdin);
    freopen_s(&file, "CONOUT$", "w", stdout);
#endif

    std::cout << "Attached successfully" << std::endl;

    WINDOWINFO info;
    ZeroMemory(&info, sizeof(info));
    info.cbSize = sizeof(info);
    GetWindowInfo(globals::mainWindow, &info);
    globals::wRect = info.rcClient;

    hooks::init();

    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        globals::mainModule = hModule;
        globals::mainWindow = (HWND)FindWindowA(0, "STAR WARS Battlefront II");
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)onAttach, hModule, 0, 0);

    case DLL_PROCESS_DETACH:
        kiero::shutdown();
        break;
    }
    return TRUE;
}

