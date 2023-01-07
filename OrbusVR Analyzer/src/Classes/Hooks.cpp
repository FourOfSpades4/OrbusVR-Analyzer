#include "Hooks.h"

uintptr_t gameAssembly;
uintptr_t offsets[1][2];


void Hooks::Init(int version) {
	MH_Initialize();

	gameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");

	// Scoundrel
	offsets[0][0] = 19423920;
	offsets[0][1] = 17191120;

	// Text Set
	offsets[1][0] = 18139392;
	offsets[1][1] = 0;

	// Scoundrel Bullet Hit
	offsets[2][0] = 19419072;
	offsets[2][1] = 17186272;

	// Scoundrel Bullet
	offsets[3][0] = 21786752;
	offsets[3][1] = 17147280;

	// Card Draw
	offsets[4][0] = 19416896;
	offsets[4][1] = 0;

	
	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19423920), &Hooks::ScoundrelHook, (LPVOID*)&Hooks::Scoundrel);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19423920));
	
	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 18139392), &Hooks::TextHook, (LPVOID*)&Hooks::Text);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 18139392));
	
	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19419072), &Hooks::ScoundrelBulletHitHook, (LPVOID*)&Hooks::ScoundrelBulletHit);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19419072));

	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 21786752), &Hooks::ScoundrelBulletHook, (LPVOID*)&Hooks::ScoundrelBullet);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 21786752));

	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19416896), &Hooks::CardDrawHook, (LPVOID*)&Hooks::CardDraw);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19416896));

	
	printf("Created Hook!");
}