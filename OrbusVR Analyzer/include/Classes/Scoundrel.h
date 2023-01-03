#pragma once

#include <stdint.h>
#include <MinHook.h>

namespace Classes {
	extern void(__fastcall* Scoundrel)(DWORD*, DWORD*);
	void __stdcall ScoundrelHook(DWORD* __this, DWORD* method);
}