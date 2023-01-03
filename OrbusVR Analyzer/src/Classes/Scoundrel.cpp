#include "Scoundrel.h"

void(__fastcall* Classes::Scoundrel)(DWORD*, DWORD*);
void __stdcall Classes::ScoundrelHook(DWORD* __this, DWORD* method) {
    uintptr_t scoundrelPtr = reinterpret_cast<uintptr_t>(__this);

    // bool chargingBullet = *reinterpret_cast<bool*>(scoundrelPtr + 0x10 + 213);
    // float timeSinceLastShot = *reinterpret_cast<bool*>(scoundrelPtr + 0x10 + 0x14 + 216));
    // 
    // float chargeProgress = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 552);
    // float necessaryChargeProgress = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 556);
    // 
    // uint8_t nextCard = *reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 672);
    // uint8_t activeCard = *reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 674);
    // uint8_t empowerment = *reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 675);
    // 
    // bool oneBasket = *reinterpret_card<bool*>(scoundrelPtr + 0x10 + 676);
    // 
    // int32_t bullets = *reinterpret_cast<int32_t*>(scoundrelPtr + 0x10 + 384 + 0xC);
    // int32_t chargedBullets = *reinterpret_cast<int32_t*>(scoundrelPtr + 0x10 + 580 + 0xC);
    //
    // float bulletRegenClock = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 456 + 0x14);
    // float cardRegenClock = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 520 + 0x14);
    

    return Scoundrel(__this, method);
}