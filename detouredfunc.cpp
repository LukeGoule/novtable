#include "pch.h"

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