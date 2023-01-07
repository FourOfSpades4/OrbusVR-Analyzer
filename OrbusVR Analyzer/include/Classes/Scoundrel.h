#pragma once

#include <winsock2.h>
#include <stdint.h>
#include <MinHook.h>
#include <chrono>

namespace Hooks {
	extern void(__fastcall* Scoundrel)(DWORD*, DWORD*);
	void __stdcall ScoundrelHook(DWORD* __this, DWORD* method);

	extern void(__fastcall* Text)(DWORD*, const char*, DWORD*);
	void __stdcall TextHook(DWORD* __this, const char* str, DWORD* method);

    extern void(__fastcall* CardDraw)(DWORD*, DWORD*, DWORD*);
    void __stdcall CardDrawHook(DWORD* __this, DWORD* card, DWORD* method);

    extern void(__fastcall* ScoundrelBullet)(DWORD*, DWORD*);
    void __stdcall ScoundrelBulletHook(DWORD* __this, DWORD* method);

    extern void(__fastcall* ScoundrelBulletHit)(DWORD*, DWORD*, DWORD*, float, int32_t, bool, DWORD*);
    void __stdcall ScoundrelBulletHitHook(DWORD* __this, DWORD* entity, DWORD* vector, float speed, int32_t bullets_charged, bool fullChamber, DWORD* method);
}

namespace Scoundrel {
    enum Empowerment {
        NO_EMPOWER = 0,
        EMPOWER = 1,
        SPREAD = 2,
        COPY = 3
    };

    enum Card {
        NO_CARD = 0,
        POISON = 1,
        ICE = 2,
        FLAME = 3,
        LIGHT = 4,
        ASH = 5,
        FLINT = 6
    };

    static std::string to_string(Card card) {
        switch (card) {
        case NO_CARD: return "";
        case POISON: return "Posion";
        case ICE: return "Ice";
        case FLAME: return "Flame";
        case LIGHT: return "Light";
        case ASH: return "Ash";
        case FLINT: return "Flint";
        }
        return "";
    }

    static std::string to_string(Empowerment empower) {
        switch (empower) {
        case NO_EMPOWER: return "";
        case EMPOWER: return "Empowered";
        case SPREAD: return "Spread";
        case COPY: return "Duplicate";
        }
        return "";
    }

    struct Bullet {
        int32_t id;
        std::chrono::steady_clock::time_point lastUpdate = std::chrono::steady_clock::now();
        float wastedTime;
        bool hitStatus = false;
        int rank = 0;
        int bulletsCharged;
        Card card;
        Empowerment empowerment;
    };

    extern bool chargingBullet[2];
    extern float timeSinceLastShot[2];

    extern float chargeProgress[2];
    extern float necessaryChargeProgress[2];
    extern std::chrono::steady_clock::time_point chargeFinishTime;

    extern Card nextCard[2];
    extern Card activeCard[2];
    extern Empowerment empowerment[2];

    extern int32_t bullets[2];
    extern int32_t chargedBullets[2];
    extern int32_t maxBullets;

    extern int8_t rankShot;

    extern float bulletRegenClock[2];
    extern float cardRegenClock[2];

    extern float timeSinceRankShot;

    static float rankShotDuration = 10;
    static float breakShotDuration = 8;

    extern float cardUseDelay;
}
