#include "Scoundrel.h"
#include <iostream>
#include <chrono>
#include <map>
#include "AnalyzerFrame.h"

uintptr_t player = NULL;

auto currentTime = std::chrono::steady_clock::now();
auto previousRankTime = std::chrono::steady_clock::time_point();
auto previousHitTime = std::chrono::steady_clock::time_point();
auto elapsedRankTime = currentTime - previousRankTime;

bool Scoundrel::chargingBullet[2];
float Scoundrel::timeSinceLastShot[2];

float Scoundrel::chargeProgress[2];
float Scoundrel::necessaryChargeProgress[2];
std::chrono::steady_clock::time_point Scoundrel::chargeFinishTime;
bool finishedCharge = false;

Scoundrel::Card Scoundrel::nextCard[2];
Scoundrel::Card Scoundrel::activeCard[2];
Scoundrel::Empowerment Scoundrel::empowerment[2];

int32_t Scoundrel::bullets[2];
int32_t Scoundrel::chargedBullets[2];

int32_t Scoundrel::maxBullets = 3;

float Scoundrel::bulletRegenClock[2];
float Scoundrel::cardRegenClock[2];

float Scoundrel::timeSinceRankShot;

float Scoundrel::cardUseDelay;
auto lastCardRegen = currentTime;

int8_t Scoundrel::rankShot;

int current = 0;

bool inCombat = false;

int32_t currentBulletID = 0;
float wastedTime;
bool hit = false;
Scoundrel::Card card;
Scoundrel::Empowerment empowerment;
int8_t rankShot;
int32_t bulletsCharged;

std::map<uintptr_t, Scoundrel::Bullet> bullets;

void shotBullet() {
    currentBulletID++;
    wastedTime = Scoundrel::chargeProgress[current];
    if (finishedCharge) {
        wastedTime += (currentTime - Scoundrel::chargeFinishTime).count() / pow(10, 9);
    }
    finishedCharge = false;
}

void evaluateScoundrel() {
    if (Scoundrel::chargingBullet[0] != Scoundrel::chargingBullet[1] && !Scoundrel::chargingBullet[current]) {
        shotBullet();
    }

    for (auto const& [key, val] : bullets)
    {
        if ((currentTime - val.lastUpdate).count() / pow(10, 9) > 0.2) {
            bullets[key].hitStatus = hit;
            bullets[key].card = card;
            bullets[key].empowerment = empowerment;
            bullets[key].rank = rankShot;
            bullets[key].bulletsCharged = bulletsCharged;

            UI::m_Scoundrel->PrintShot(bullets[key]);
            bullets.erase(key);
            hit = false;
        }
    }

    if (!finishedCharge && Scoundrel::chargedBullets[current] == Scoundrel::maxBullets) {
        finishedCharge = true;
        Scoundrel::chargeFinishTime = currentTime;
    }

    if (Scoundrel::bulletRegenClock[current] < Scoundrel::bulletRegenClock[1 - current] && Scoundrel::bullets[1 - current] == 6) {
        UI::m_Scoundrel->WastedBullet();
    }

    if (Scoundrel::timeSinceRankShot > Scoundrel::rankShotDuration) {
        Scoundrel::rankShot = 0;
    }
}


void(__fastcall* Hooks::Scoundrel)(DWORD*, DWORD*);
void __stdcall Hooks::ScoundrelHook(DWORD* __this, DWORD* method) {
    uintptr_t scoundrelPtr = reinterpret_cast<uintptr_t>(__this);

    if (player != NULL && scoundrelPtr == player && inCombat) {
        currentTime = std::chrono::steady_clock::now();

        Scoundrel::chargingBullet[current] = *reinterpret_cast<bool*>(scoundrelPtr + 0x10 + 213);
        Scoundrel::timeSinceLastShot[current] = *reinterpret_cast<bool*>(scoundrelPtr + 0x10 + 0x14 + 216);

        Scoundrel::chargeProgress[current] = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 552);
        Scoundrel::necessaryChargeProgress[current] = 0.5;

        Scoundrel::nextCard[current] = (Scoundrel::Card) * reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 672);
        Scoundrel::activeCard[current] = (Scoundrel::Card) * reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 674);
        Scoundrel::empowerment[current] = (Scoundrel::Empowerment) * reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 675);

        Scoundrel::bullets[current] = *reinterpret_cast<int32_t*>(scoundrelPtr + 0x10 + 384 + 0xC);
        Scoundrel::chargedBullets[current] = *reinterpret_cast<int32_t*>(scoundrelPtr + 0x10 + 580 + 0xC);

        Scoundrel::bulletRegenClock[current] = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 456 + 0x14);
        Scoundrel::cardRegenClock[current] = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 520 + 0x14);

        Scoundrel::timeSinceRankShot = (currentTime - previousRankTime).count() / pow(10, 9);

        if ((currentTime - previousHitTime).count() / pow(10, 9) > 10) {
            inCombat = false;
            UI::m_Scoundrel->Reset();
        }

        evaluateScoundrel();

        current = 1 - current;
    }

    else if (player == NULL) {
        // if ((Scoundrel::Card) * reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 674) == Scoundrel::Card::ASH && (Scoundrel::Empowerment) * reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 675) == Scoundrel::Empowerment::SPREAD) {
            printf("Detected player!");
            player = scoundrelPtr;
        // }
    }

    return Scoundrel(__this, method);
}


void(__fastcall* Hooks::Text)(DWORD*, const char*, DWORD*);
void __stdcall Hooks::TextHook(DWORD* __this, const char* str, DWORD* method) {
    char data[256];
    wcstombs(data, (wchar_t*)str + 0xA, sizeof(data));

    if (std::string(data).substr(0, 4) == "Rank") {
        elapsedRankTime = currentTime - previousRankTime;
        previousRankTime = currentTime;
        std::string rank = std::string(data).substr(5);
        if (rank == "I") {
            Scoundrel::rankShot = 1;
        }
        else if (rank == "II") {
            Scoundrel::rankShot = 2;
        }
        else if (rank == "III") {
            Scoundrel::rankShot = 3;
        }
        else if (rank == "IV") {
            Scoundrel::rankShot = 4;
        }
        else if (rank == "V") {
            Scoundrel::rankShot = 5;
        }
        printf("%i     %f\n\n", Scoundrel::rankShot, elapsedRankTime.count() / pow(10,9));
    }


    return Text(__this, str, method);
}

void(__fastcall* Hooks::CardDraw)(DWORD*, DWORD*, DWORD*);
void __stdcall Hooks::CardDrawHook(DWORD* __this, DWORD* card, DWORD* method) {
    if (reinterpret_cast<uintptr_t>(__this) == player && inCombat) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(card);
        uint8_t* cardInt = reinterpret_cast<uint8_t*>(ptr + 0x10);

        if (*cardInt != 0) {
            lastCardRegen = std::chrono::steady_clock::now();
        }
        else {
            printf("Card Delay: %f\n\n", (currentTime - lastCardRegen).count() / pow(10, 9));
        }

        printf("Card: %i\n", *cardInt);
    }

    return CardDraw(__this, card, method);
}


void(__fastcall* Hooks::ScoundrelBullet)(DWORD*, DWORD*);
void __stdcall Hooks::ScoundrelBulletHook(DWORD* __this, DWORD* method) {
    uintptr_t bulletPtr = reinterpret_cast<uintptr_t>(__this);

    if (player == *reinterpret_cast<uintptr_t*>(bulletPtr + 0x10 + 48)) {
        if (bullets.find(bulletPtr) == bullets.end()) {
            Scoundrel::Bullet b = Scoundrel::Bullet();
            b.id = currentBulletID;
            b.wastedTime = wastedTime;
            bullets[bulletPtr] = b;
            printf("%i\n", bullets[bulletPtr].id);
        }
        bullets[bulletPtr].lastUpdate = currentTime;
    }

    return ScoundrelBullet(__this, method);
}

void(__fastcall* Hooks::ScoundrelBulletHit)(DWORD*, DWORD*, DWORD*, float, int32_t, bool, DWORD*);
void __stdcall Hooks::ScoundrelBulletHitHook(DWORD* __this, DWORD* entity, DWORD* vector, float speed, int32_t bullets_charged, bool fullChamber, DWORD* method) {
    uintptr_t scoundrelPtr = reinterpret_cast<uintptr_t>(__this);

    if (scoundrelPtr == player) {
        previousHitTime = currentTime;
        inCombat = true;
        hit = true;
        card = Scoundrel::activeCard[current];
        empowerment = Scoundrel::empowerment[current];
        rankShot = Scoundrel::rankShot;
        bulletsCharged = bullets_charged;
    }

    return ScoundrelBulletHit(__this, entity, vector, speed, bullets_charged, fullChamber, method);
}
