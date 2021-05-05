#pragma once

#ifndef INTERFACE_HPP
#define INTERFACE_HPP

struct interface_iterator_t
{
	void* (*create_interface)();
	char* name;
	interface_iterator_t* next;
};

template<typename out, class type>
inline out GetVFunc1(size_t index, type* self)
{
	return reinterpret_cast<out>((*reinterpret_cast<void***>(self))[index]);
}

template<typename Function> Function GetVFunc2(PVOID Base, DWORD Index)
{
	PDWORD* VTablePointer = (PDWORD*)Base;
	PDWORD VTableFunctionBase = *VTablePointer;
	DWORD dwAddress = VTableFunctionBase[Index];
	return (Function)(dwAddress);
}

inline void* GetVFunc3(void* Instance, int Index)
{
	DWORD VirtualFunction = (*(DWORD*)Instance) + sizeof(DWORD) * Index;
	return (void*)*((DWORD*)VirtualFunction);
}

inline uintptr_t FollowJMP(uintptr_t ptr)
{
	return ptr + 5 + *reinterpret_cast<int*>(ptr + 1);
}

inline auto GetInterfaceList(uintptr_t ptr)
{
	return **reinterpret_cast<interface_iterator_t***>(ptr + 0x6);
}

// Fasterified
inline size_t GetInterfaceVer(const char* name)
{
	size_t version = 0;

	for (; *name; ++name)
	{
		if (*name <= '9')
			version = version * 10 + (*name - '0');
	}

	return version;
}

template<class type>
type* GetInterface(HMODULE dll, const char* name)
{
	assert(dll);

	auto ptr = reinterpret_cast<uintptr_t>(GetProcAddress(dll, "CreateInterface")) + 0x4;
	assert(ptr - 0x4);

	ptr = FollowJMP(ptr);
	type* interface = nullptr;
	size_t version = 0;

	for (auto iter = GetInterfaceList(ptr); iter; iter = iter->next)
	{
		if (std::strstr(iter->name, name))
		{
			if (auto current_version = GetInterfaceVer(iter->name); current_version > version)
			{
				version = current_version;
				interface = reinterpret_cast<type*>(iter->create_interface());
			}
		}
	}

	return interface;
}


#endif