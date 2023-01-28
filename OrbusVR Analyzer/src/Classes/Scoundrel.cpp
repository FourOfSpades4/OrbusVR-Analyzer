#include "Scoundrel.h"
#include <iostream>
#include <chrono>
#include <map>
#include "AnalyzerFrame.h"

std::map<std::string, Scoundrel::ScoundrelCharacter*> players;

Scoundrel::ScoundrelCharacter* myPlayer;
Scoundrel::ScoundrelCharacter* lastShotPlayer;

bool myPlayerSet = false;

auto currentTime = std::chrono::steady_clock::now();

uintptr_t getMonster(uintptr_t gambler) {
    uintptr_t playerAvatar = *reinterpret_cast<uintptr_t*>(gambler + 0x10 + 40);
    uintptr_t monster = *reinterpret_cast<uintptr_t*>(playerAvatar + 0x10 + 368);
    return monster;
}

std::string getPlayerName(uintptr_t gambler) {
    uintptr_t monster = getMonster(gambler);

    wchar_t* str = reinterpret_cast<wchar_t*>(*reinterpret_cast<uintptr_t*>(monster + 0x10 + 288));
    char nameArr[32];
    int res = wcstombs(nameArr, str + 0xA, sizeof(nameArr));
    std::string name(nameArr);

    return name;
}

bool isMyPlayer(uintptr_t gambler) {
    uintptr_t monster = getMonster(gambler);

    bool isMyPlayer = *reinterpret_cast<bool*>(monster + 0x10 + 45);

    return isMyPlayer;
}

Scoundrel::ScoundrelCharacter* getPlayer(uintptr_t scoundrelPtr) {
    std::string playerName = getPlayerName(scoundrelPtr);
    Scoundrel::ScoundrelCharacter* character = new Scoundrel::ScoundrelCharacter;

    std::map<std::string, Scoundrel::ScoundrelCharacter*>::iterator it = players.find(playerName);
    if (it == players.end()) {
        character->name = playerName;
        players.insert(std::make_pair(playerName, character));
    }
    else {
        character = it->second;
    }

    return character;
}

void shotBullet(Scoundrel::ScoundrelCharacter* character) {
    character->currentBulletID++;
    character->wastedShotTime = character->chargeProgress[character->current];
    if (character->finishedCharge) {
        character->wastedShotTime += (currentTime - character->chargeFinishTime).count() / pow(10, 9);
    }
    character->finishedCharge = false;
}

void evaluateScoundrel(Scoundrel::ScoundrelCharacter* character) {
    if (character->chargingBullet[0] != character->chargingBullet[1] && !character->chargingBullet[character->current]) {
        shotBullet(character);
    }

    for (auto const& [key, val] : character->trackedBullets)
    {
        if ((currentTime - val.lastUpdate).count() / pow(10, 9) > 0.2) {
            character->trackedBullets[key].hitStatus = character->hit;
            character->trackedBullets[key].rank = character->rankShot;
            if (character->trackedBullets[key].rank > 5) {
                character->trackedBullets[key].rank = 0;
            }
            // character->trackedBullets[key].bulletsCharged = character->bulletsCharged;
            if (character->activeCard != Scoundrel::NO_CARD) {
                character->trackedBullets[key].card = character->activeCard;
                character->trackedBullets[key].empowerment = character->empowerment;
                character->activeCard = Scoundrel::NO_CARD;
                character->empowerment = Scoundrel::NO_EMPOWER;
            }
            else {
                character->trackedBullets[key].card = Scoundrel::NO_CARD;
                character->trackedBullets[key].empowerment = Scoundrel::NO_EMPOWER;
            }
            if (character == myPlayer) {
                if (character->inCombat) {
                    UI::m_Scoundrel->ProcessShot(character, character->trackedBullets[key], character->firstBullet);
                    character->firstBullet = false;
                }
            }
            
            else {
                character->inCombat = true;
                character->previousHitTime = currentTime;
                UI::m_Scoundrel->ProcessShot(character, character->trackedBullets[key], character->firstBullet);
            }
            
            character->trackedBullets.erase(key);
            character->hit = false;
        }
    }

    if (!character->finishedCharge && character->chargedBullets[character->current] == character->maxBullets) {
        character->finishedCharge = true;
        character->chargeFinishTime = currentTime;
    }

    if (character->bulletRegenClock[character->current] < character->bulletRegenClock[1 - character->current] && character->bullets[1 - character->current] == 6) {
        if (character->inCombat)
            UI::m_Scoundrel->ProcessWastedBullet(character);
    }

    if (character->timeSinceRankShot > Scoundrel::rankShotDuration) {
        character->rankShot = 0;
    }
}


void(__fastcall* Hooks::Scoundrel)(DWORD*, DWORD*);
void __stdcall Hooks::ScoundrelHook(DWORD* __this, DWORD* method) {
    currentTime = std::chrono::steady_clock::now();
    uintptr_t scoundrelPtr = reinterpret_cast<uintptr_t>(__this);

    Scoundrel::ScoundrelCharacter* character = getPlayer(scoundrelPtr);

    if (character == myPlayer) {
        bool oneBasket = *reinterpret_cast<bool*>(scoundrelPtr + 0x10 + 676);
        if (oneBasket) {
            character->maxBullets = 4;
        }
        else {
            character->maxBullets = 3;
        }

        character->chargingBullet[character->current] = *reinterpret_cast<bool*>(scoundrelPtr + 0x10 + 213);
        character->timeSinceLastShot[character->current] = *reinterpret_cast<bool*>(scoundrelPtr + 0x10 + 0x14 + 216);

        character->chargeProgress[character->current] = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 552);
        character->necessaryChargeProgress[character->current] = 0.5;

        // std::cout << to_string((Scoundrel::Card) * reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 672)) << std::endl;
        // Scoundrel::activeCard[current] = (Scoundrel::Card) * reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 674);
        // Scoundrel::empowerment[current] = (Scoundrel::Empowerment) * reinterpret_cast<uint8_t*>(scoundrelPtr + 0x10 + 675);

        character->bullets[character->current] = *reinterpret_cast<int32_t*>(scoundrelPtr + 0x10 + 384 + 0xC);
        character->chargedBullets[character->current] = *reinterpret_cast<int32_t*>(scoundrelPtr + 0x10 + 580 + 0xC);

        character->bulletRegenClock[character->current] = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 456 + 0x14);
        character->cardRegenClock[character->current] = *reinterpret_cast<float*>(scoundrelPtr + 0x10 + 520 + 0x14);

        character->timeSinceRankShot = (currentTime - character->previousRankTime).count() / pow(10, 9);

        if ((currentTime - character->previousHitTime).count() / pow(10, 9) > 10 && character->inCombat) {
            UI::m_Scoundrel->ProcessEndCombat(character);
            character->inCombat = false;
            character->firstBullet = true;
            character->firstCard = true;
            character->trackedBullets.clear();
        }

        evaluateScoundrel(character);

        character->current = 1 - character->current;
    }

    Scoundrel(__this, method);
}


void(__fastcall* Hooks::Text)(DWORD*, const char*, DWORD*);
void __stdcall Hooks::TextHook(DWORD* __this, const char* str, DWORD* method) {
    char data[256];
    wcstombs(data, (wchar_t*)str + 0xA, sizeof(data));

    if (std::string(data).substr(0, 4) == "Rank") {
        myPlayer->previousRankTime = currentTime;
        std::string rank = std::string(data).substr(5);
        if (rank == "I") {
            myPlayer->rankShot = 1;
        }
        else if (rank == "II") {
            myPlayer->rankShot = 2;
        }
        else if (rank == "III") {
            myPlayer->rankShot = 3;
        }
        else if (rank == "IV") {
            myPlayer->rankShot = 4;
        }
        else if (rank == "V") {
            myPlayer->rankShot = 5;
        }
    }


    return Text(__this, str, method);
}

void(__fastcall* Hooks::CardDraw)(DWORD*, DWORD*, DWORD*);
void __stdcall Hooks::CardDrawHook(DWORD* __this, DWORD* c, DWORD* method) {
    uintptr_t scoundrelPtr = reinterpret_cast<uintptr_t>(__this);
    Scoundrel::ScoundrelCharacter* character = getPlayer(scoundrelPtr);

    if (character == myPlayer && character->inCombat) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(c);
        uint8_t* cardInt = reinterpret_cast<uint8_t*>(ptr + 0x10);

        if (*cardInt != 0) {
            character->lastCardRegen = std::chrono::steady_clock::now();
            character->nextCard = (Scoundrel::Card)*cardInt;
        }
        else {
            UI::m_Scoundrel->ProcessDrawCard(character, character->nextCard, (currentTime - character->lastCardRegen).count() / pow(10, 9), character->firstCard);
            character->firstCard = false;
        }
    }

    return CardDraw(__this, c, method);
}

void(__fastcall* Hooks::CardUse)(DWORD*, DWORD*, DWORD*);
void __stdcall Hooks::CardUseHook(DWORD* __this, DWORD* c, DWORD* method) {
    uintptr_t scoundrelPtr = reinterpret_cast<uintptr_t>(__this);
    Scoundrel::ScoundrelCharacter* character = getPlayer(scoundrelPtr);

    if (character == myPlayer && character->inCombat) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(c);
        uint8_t* cardInt = reinterpret_cast<uint8_t*>(ptr + 0x10);
        if (*cardInt != 0) {
            UI::m_Scoundrel->ProcessUseCard(character, (Scoundrel::Card)*cardInt);
        }
        else {
            UI::m_Scoundrel->ProcessActivateCard(character);
        }
    }

    return CardUse(__this, c, method);
}

void(__fastcall* Hooks::Empower)(DWORD*, DWORD*, DWORD*);
void __stdcall Hooks::EmpowerHook(DWORD* __this, DWORD* e, DWORD* method) {
    uintptr_t scoundrelPtr = reinterpret_cast<uintptr_t>(__this);
    Scoundrel::ScoundrelCharacter* character = getPlayer(scoundrelPtr);

    if (character == myPlayer && character->inCombat) {
        uintptr_t ptr = reinterpret_cast<uintptr_t>(e);
        uint8_t* empowerInt = reinterpret_cast<uint8_t*>(ptr + 0x10);
        if (*empowerInt != 0) {
            UI::m_Scoundrel->ProcessUseEmpower(character, (Scoundrel::Empowerment)*empowerInt);
        }
    }

    return Empower(__this, e, method);
}


void(__fastcall* Hooks::ScoundrelBullet)(DWORD*, DWORD*);
void __stdcall Hooks::ScoundrelBulletHook(DWORD* __this, DWORD* method) {
    uintptr_t bulletPtr = reinterpret_cast<uintptr_t>(__this);
    uintptr_t scoundrelPtr = *reinterpret_cast<uintptr_t*>(bulletPtr + 0x10 + 48);
    /*
    if (scoundrelPtr != 0) {
        character = getPlayer(scoundrelPtr);
    }
    else {
        if (reinterpret_cast<uintptr_t>(lastShotPlayer) != 0) {
            character = lastShotPlayer;
            lastShotPlayer = NULL;
        }
        else {
            return ScoundrelBullet(__this, method);
        }
    }
    */
    if (scoundrelPtr != 0) {
        if (!myPlayerSet) {
            myPlayer = getPlayer(scoundrelPtr);
            UI::m_Scoundrel->SetPlayer(myPlayer);
            myPlayerSet = true;
        }

        if (myPlayer->trackedBullets.find(bulletPtr) == myPlayer->trackedBullets.end()) {
            Scoundrel::Bullet b = Scoundrel::Bullet();
            b.id = myPlayer->currentBulletID;
            b.wastedTime = myPlayer->wastedShotTime;
            b.bulletsCharged = *reinterpret_cast<int32_t*>(bulletPtr + 0x48);
            myPlayer->trackedBullets[bulletPtr] = b;
        }
        myPlayer->trackedBullets[bulletPtr].lastUpdate = currentTime;
    }

    return ScoundrelBullet(__this, method);
}

void(__fastcall* Hooks::ScoundrelBulletHit)(DWORD*, DWORD*, DWORD*, float, int32_t, bool, DWORD*);
void __stdcall Hooks::ScoundrelBulletHitHook(DWORD* __this, DWORD* entity, DWORD* vector, float speed, int32_t bullets_charged, bool fullChamber, DWORD* method) {
    uintptr_t scoundrelPtr = reinterpret_cast<uintptr_t>(__this);


    myPlayer->inCombat = true;
    myPlayer->previousHitTime = currentTime;

    if (myPlayer->inCombat) {
        myPlayer->hit = true;
        myPlayer->rankShot = myPlayer->rankShot;
    }
    else {
        myPlayer->trackedBullets.clear();
    }

    return ScoundrelBulletHit(__this, entity, vector, speed, bullets_charged, fullChamber, method);
}

void Scoundrel::Reset() {
    players.clear();
    myPlayer = NULL;
    myPlayerSet = false;
}