#include "pch.h"

#pragma region Function Detour Templates

template <typename T>
DetouredFunction<T>::DetouredFunction(T pFunc, std::string Pattern, std::string Module, std::string Ident)
	: DetouredFunction_Container(Pattern, Module), m_pFunction(pFunc), m_Identifier(Ident)
{
	HMODULE hMod = GetModuleHandleA(Module.c_str());

	// Get the original func.
	m_poFunction = Utils::PatternScan(hMod, Pattern.c_str());

	if (!m_poFunction)
	{
		throw std::runtime_error{ "m_poFunction == 0" };
	}
}

template <typename T>
void DetouredFunction<T>::Attach()
{
	if (m_bDetoured) return;

	if (!m_poFunction || !m_pFunction)
	{
		throw std::runtime_error{ "poFunction or pFunction was null" };
	}

	Utils::ConsolePrint("Detour %s attached\n", m_Identifier.c_str());
	DetourAttach(&(PVOID&)m_poFunction, m_pFunction);

	m_bDetoured = true;
}

template <typename T>
void DetouredFunction<T>::Detach()
{
	if (!m_bDetoured) return;

	if (!m_poFunction || !m_pFunction)
	{
		throw std::runtime_error{ "poFunction or pFunction was null" };
	}
	
	Utils::ConsolePrint("Detour %s detached\n", m_Identifier.c_str());
	DetourDetach(&(PVOID&)m_poFunction, m_pFunction);
}

template <typename T>
T DetouredFunction<T>::GetOriginal()
{
	return (T)m_poFunction;
}

template <typename T>
std::string DetouredFunction<T>::GetIdentifier()
{
	return m_Identifier;
}

#pragma endregion

#pragma region Function Hooking

DetouredFunction_Container* g_RunStringHook, * g_CreateMoveHook;

bool __fastcall hkRunStringEx(void* thisptr, int edx, const char* filename, const char* path, const char* buf, bool b1, bool b2, bool b3, bool b4)
{
	auto oRunStringEx = DETOUR_ORIGINAL(hkRunStringEx, g_RunStringHook);

	// Do your script dumping here.

	return oRunStringEx(thisptr, edx, filename, path, buf, b1, b2, b3, b4);
}

void __fastcall hkCreateMove(CInput* pThis, int edx, int sequence_number, float input_sample_frametime, bool active)
{
	auto oCreateMove = DETOUR_ORIGINAL(hkCreateMove, g_CreateMoveHook);
	
	CUserCmd* pCmd = pThis->GetUserCmd(sequence_number);
	CVerifiedUserCmd* pVerified = pThis->GetVerifiedCmd(sequence_number);

	// Modifiy pCmd here
	{
		
	}

	pVerified->m_pCmd = *pCmd;
	pVerified->m_CRC32 = pCmd->Checksum();

	oCreateMove(pThis, edx, sequence_number, input_sample_frametime, active);
}

void Hooks::Initialise()
{
	// All you need to hook is the sig of the function and it's dll. these are generally easy to find if you're modding a source game.

	g_RunStringHook = DETOUR_INIT(hkRunStringEx, "55 8B EC 8B 55 10 81 EC 4C 01 00 00 56 8B F1 57 85 D2 0F 84 ? ? ? ? 8B CA", "lua_shared.dll");
	g_CreateMoveHook = DETOUR_INIT(hkCreateMove, "55 8B EC 83 EC 58 53 8B 5D 08 B8 ? ? ? ?", "client.dll");
	
	// detours takes care of the heavy lifting.
	DetourRestoreAfterWith();
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DETOUR_ATTACH(g_RunStringHook);
	DETOUR_ATTACH(g_CreateMoveHook);

	DetourTransactionCommit();
}

void Hooks::Destroy()
{
	DetourTransactionBegin();
	DetourUpdateThread(GetCurrentThread());

	DETOUR_DETACH(g_RunStringHook);
	DETOUR_DETACH(g_CreateMoveHook);

	DetourTransactionCommit();
}

#pragma endregion