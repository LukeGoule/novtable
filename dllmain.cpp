// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"

/*
inline size_t GetMaxClients()
{
    using get_max_clients_t = size_t(__thiscall*)(void*);
    return method<get_max_clients_t>(21, this)(this);
}
*/

unsigned long hkGetMaxClients(void* _this)
{
    Utils::ConsolePrint("GetMaxClients called by: %x\n", (DWORD)_this);

    return 1; // lol
}

DWORD WINAPI OnDllAttach(PVOID pBase)
{
    Utils::AttachConsole();
    Utils::ConsolePrint("Attached.\nPress VK_END to exit\n");

    try
    {
        Hooks::Initialise();
    }
    catch (const std::runtime_error& Error)
    {
        
        auto Message = ("Error during hooking: " + std::string(Error.what())).c_str();

        Utils::ConsolePrint("%s\n", Message);

        MessageBoxA(0, Message, "ERROR", MB_ICONERROR);

        goto exit;
    }

    while (!GetAsyncKeyState(VK_END))
    {}

    exit:
    
    Utils::ConsolePrint("Detaching..\n");
    Hooks::Destroy();

    FreeLibraryAndExitThread(static_cast<HMODULE>(pBase), 1);

    return TRUE;
}

DWORD WINAPI OnDllDetach()
{
    Utils::DetachConsole();

    return TRUE;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  dwReason,
                       LPVOID lpReserved
                     )
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, OnDllAttach, hModule, 0, nullptr);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
    {
        if (lpReserved == nullptr)
            return OnDllDetach();
    }

    return TRUE;
}

