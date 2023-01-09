#pragma once

#include <WinSock2.h>
#include <wx/msw/winundef.h>
#include <wx/wx.h>
#include <memory>
#include <NotificationCallback.h>
#include "Scoundrel.h"

// Encapsulates all the logic for Scoundrel cheat selectors
class ScoundrelUIController
{
public:
	ScoundrelUIController(wxWindow* parent, int offsetX, int offsetY, void* param);
	~ScoundrelUIController();
	void PrintShot(Scoundrel::Bullet bullet, bool first);
	void PrintUseCard(Scoundrel::Card card);
	void PrintUseEmpower(Scoundrel::Empowerment empower);
	void PrintDrawCard(Scoundrel::Card card, float time, bool first);
	void PrintActivateCard();
	void Reset();
	void WastedBullet();

private:
	std::unique_ptr<wxTextCtrl> m_BulletConsole;
	std::unique_ptr<wxTextCtrl> m_CardConsole;
	std::unique_ptr<wxStaticText> m_TextOrganizer;

	std::unique_ptr<wxStaticText> m_Bullet;
	std::unique_ptr<wxStaticText> m_Card;
	
	std::unique_ptr<wxStaticText> m_BulletRanking;
	std::unique_ptr<wxStaticText> m_BulletRankingRating;

	std::unique_ptr<wxStaticText> m_CardRanking;
	std::unique_ptr<wxStaticText> m_CardRankingRating;

	std::unique_ptr<wxStaticText> m_OverallRanking;
	std::unique_ptr<wxStaticText> m_OverallRankingRating;

	std::unique_ptr<wxStaticText> m_TotalPoison;
	std::unique_ptr<wxStaticText> m_TotalFlame;

	std::unique_ptr<wxStaticText> m_WastedBullets;
	std::unique_ptr<wxStaticText> m_BulletDelay;
	std::unique_ptr<wxStaticText> m_BulletDelayRating;
	std::unique_ptr<wxStaticText> m_CardDelay;
	std::unique_ptr<wxStaticText> m_CardDelayRating;


};