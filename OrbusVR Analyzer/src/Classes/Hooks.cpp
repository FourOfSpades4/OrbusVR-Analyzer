#include "Hooks.h"

uintptr_t gameAssembly;
uintptr_t offsets[6][2];

void MagicalActual_Loot$$EKILKKOCNIM(DWORD* __this, DWORD* encoded, DWORD* method) {
	/*
	encoded = 1l2cpp object encoding a string. 0x1A is likely offset
	something to do with what type of bag? 
	*/
}

void MagicalActual_Loot$$EKPAPCPHNPK(DWORD* __this, DWORD* encoded, DWORD* method) {
	/*
	encoded = 1l2cpp object encoding a string. 0x1A is likely offset
	likely string representation of loot
	*/
}

/*
agicalActual.Monster$$ICFNBHPOIOE
*/

/*
void(__fastcall* Test)(DWORD*, DWORD*, DWORD*);
void __stdcall TestHook(DWORD* __this, DWORD* vec, DWORD* method) {
	printf("Arrow Hit!\n");

	Test(__this, vec, method);
}
*/


void Hooks::Init(int version) {
	MH_Initialize();

	gameAssembly = (uintptr_t)GetModuleHandleA("GameAssembly.dll");

	// Scoundrel
	offsets[0][0] = 19424592;
	offsets[0][1] = 0;

	// Text Set
	offsets[1][0] = 18140208;
	offsets[1][1] = 0;

	// Scoundrel Bullet Hit
	offsets[2][0] = 19419744;
	offsets[2][1] = 0;

	// Scoundrel Bullet
	offsets[3][0] = 21787456;
	offsets[3][1] = 0;

	// Card Draw
	offsets[4][0] = 19417568;
	offsets[4][1] = 0;

	// Card Use
	offsets[5][0] = 19443824;
	offsets[5][1] = 0;

	// Card Draw
	offsets[6][0] = 19434640;
	offsets[6][1] = 0;


	
	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19424592), &Hooks::ScoundrelHook, (LPVOID*)&Hooks::Scoundrel);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19424592));

	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19419744), &Hooks::ScoundrelBulletHitHook, (LPVOID*)&Hooks::ScoundrelBulletHit);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19419744));
	
	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 18140032), &Hooks::TextHook, (LPVOID*)&Hooks::Text);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 18140032));

	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 21787456), &Hooks::ScoundrelBulletHook, (LPVOID*)&Hooks::ScoundrelBullet);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 21787456));

	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19417568), &Hooks::CardDrawHook, (LPVOID*)&Hooks::CardDraw);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19417568));

	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19443824), &Hooks::CardUseHook, (LPVOID*)&Hooks::CardUse);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19443824));

	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19434640), &Hooks::EmpowerHook, (LPVOID*)&Hooks::Empower);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19434640));
	/*
	MH_CreateHook(reinterpret_cast<LPVOID*>(gameAssembly + 19444448), &TestHook, (LPVOID*)&Test);
	MH_EnableHook(reinterpret_cast<LPVOID*>(gameAssembly + 19444448));
	*/
	
	
	printf("Created Hook!");
}