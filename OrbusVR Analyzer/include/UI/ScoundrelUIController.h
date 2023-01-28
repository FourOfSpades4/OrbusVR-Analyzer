#pragma once

#include <WinSock2.h>
#include <wx/msw/winundef.h>
#include <wx/wx.h>
#include <memory>
#include <NotificationCallback.h>
#include "Scoundrel.h"
#include <list>

namespace Scoundrel {
	struct CardArguement {
		enum CardArguementType { ActivateCard, UseCard, UseEmpower, DrawCard, CombatEnd };
		CardArguementType arguementType;
		Scoundrel::Card card;
		Scoundrel::Empowerment empowerment;
		float time;
		bool first;
	};

	struct BulletArguement {
		enum BulletArguementType { ShotBullet, WastedBullet, CombatEnd };
		BulletArguementType arguementType;
		Scoundrel::Bullet bullet;
		bool first;
	};

	class ScoundrelUICharacter
	{
	public:
		std::vector<CardArguement> cardArguements;
		std::vector<BulletArguement> bulletArguements;
		int shots = 0;
		int bullets = 0;
		int cards = 0;
		int totalBulletRating = 0;
		int totalWastedBullets = 0;
		float totalBulletDelay = 0;
		int totalCardRating = 0;
		int totalCardRankShot = 0;
		float totalCardDelay = 0;
		int totalPoisons = 0;
		int totalFlames = 0;
		int32_t maxBullets = 3;

		Scoundrel::Card card;
		Scoundrel::Empowerment empowerment;
	};
}

// Encapsulates all the logic for Scoundrel cheat selectors
class ScoundrelUIController
{
public:
	ScoundrelUIController(wxWindow* parent, int offsetX, int offsetY, void* param);
	~ScoundrelUIController();

	void ProcessPlayer(Scoundrel::ScoundrelUICharacter* character);

	void ProcessEndCombat(Scoundrel::ScoundrelCharacter* character);
	void PrintBulletEndCombat(Scoundrel::ScoundrelUICharacter* character);
	void PrintCardEndCombat(Scoundrel::ScoundrelUICharacter* character);

	void EnsureUICharacterExists(Scoundrel::ScoundrelCharacter* character);
	void SetPlayer(Scoundrel::ScoundrelCharacter* character);

	void ProcessShot(Scoundrel::ScoundrelCharacter* character, Scoundrel::Bullet bullet, bool first);
	void ProcessUseCard(Scoundrel::ScoundrelCharacter* character, Scoundrel::Card card);
	void ProcessUseEmpower(Scoundrel::ScoundrelCharacter* character, Scoundrel::Empowerment empower);
	void ProcessDrawCard(Scoundrel::ScoundrelCharacter* character, Scoundrel::Card card, float time, bool first);
	void ProcessActivateCard(Scoundrel::ScoundrelCharacter* character);
	void ProcessWastedBullet(Scoundrel::ScoundrelCharacter* character);

	void PrintShot(Scoundrel::ScoundrelUICharacter* character, Scoundrel::BulletArguement bullet);
	void PrintUseCard(Scoundrel::ScoundrelUICharacter* character, Scoundrel::CardArguement card);
	void PrintUseEmpower(Scoundrel::ScoundrelUICharacter* character, Scoundrel::CardArguement card);
	void PrintDrawCard(Scoundrel::ScoundrelUICharacter* character, Scoundrel::CardArguement card);
	void PrintActivateCard(Scoundrel::ScoundrelUICharacter* character, Scoundrel::CardArguement card);
	void PrintWastedBullet(Scoundrel::ScoundrelUICharacter* character, Scoundrel::BulletArguement bullet);

	// void OnPlayerSelect(wxEvent& ev);
	void Reset();

private:
	std::unique_ptr<wxTextCtrl> m_BulletConsole;
	std::unique_ptr<wxTextCtrl> m_BulletRotationConsole;
	std::unique_ptr<wxTextCtrl> m_CardConsole;
	std::unique_ptr<wxStaticText> m_TextOrganizer;
	std::unique_ptr<wxStaticText> m_PlayerName;

	std::unique_ptr<wxStaticText> m_Bullet;
	std::unique_ptr<wxStaticText> m_Card;
	
	std::unique_ptr<wxStaticText> m_BulletRanking;
	std::unique_ptr<wxStaticText> m_BulletRankingRating;

	std::unique_ptr<wxStaticText> m_CardRanking;
	std::unique_ptr<wxStaticText> m_CardRankingRating;

	std::unique_ptr<wxStaticText> m_OverallRanking;
	std::unique_ptr<wxStaticText> m_OverallRankingRating;

	std::unique_ptr<wxStaticText> m_TotalPoison;
	std::unique_ptr<wxStaticText> m_TotalPoisonRating;
		
	std::unique_ptr<wxStaticText> m_TotalFlame;

	std::unique_ptr<wxStaticText> m_BulletRankShot;
	std::unique_ptr<wxStaticText> m_BulletRankShotRating;

	std::unique_ptr<wxStaticText> m_WastedBullets;
	std::unique_ptr<wxStaticText> m_WastedBulletsRating;

	std::unique_ptr<wxStaticText> m_BulletDelay;
	std::unique_ptr<wxStaticText> m_BulletDelayRating;
	std::unique_ptr<wxStaticText> m_CardDelay;
	std::unique_ptr<wxStaticText> m_CardDelayRating;
};
