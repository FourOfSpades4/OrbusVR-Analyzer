#include "Hooks.h"

uintptr_t gameAssembly;
uintptr_t offsets[1][2];

void Hooks::Init(int version) {
	MH_Initialize();

	gameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");

	// Scoundrel
	offsets[0][0] = 19423920;
	offsets[0][1] = 17191120;

	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + offsets[0][version]), &Classes::ScoundrelHook, (LPVOID*)&Classes::Scoundrel);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + offsets[0][version]));
	printf("Enabled Scoundrel Hook\n");
}