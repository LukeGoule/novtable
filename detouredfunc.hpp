#pragma once

#ifndef DTDFUNC_HPP
#define DTDFUNC_HPP

#include <string>
#include <cinttypes>

#define DETOUR_INIT(Func, Pat, Mod)		reinterpret_cast<DetouredFunction_Container*>(new DetouredFunction<decltype(&Func)>(Func, Pat, Mod, #Func));
#define DETOUR_ORIGINAL(Func, dt)		reinterpret_cast<DetouredFunction<decltype(&Func)>*>(dt)->GetOriginal();

#define DETOUR_ATTACH(dt) dt->Attach()
#define DETOUR_DETACH(dt) dt->Detach(); delete dt;

class DetouredFunction_Container
{
protected:
	std::string m_Pattern, m_Module;
	bool m_bDetoured = false;
	uint8_t* m_poFunction;

	DetouredFunction_Container(std::string Pattern, std::string Module)
		: m_Pattern(Pattern), m_Module(Module), m_poFunction(nullptr)
	{}

public:
	virtual void Attach() = 0;
	virtual void Detach() = 0;
};

template<typename T>
class DetouredFunction : public DetouredFunction_Container
{

	T m_pFunction;
	std::string m_Identifier;

public:

	DetouredFunction(T pFunc, std::string Pattern, std::string Module = "client.dll", std::string Ident = "(unknown)");

	void Attach();
	void Detach();
	T GetOriginal();

	std::string GetIdentifier();
};

#endif