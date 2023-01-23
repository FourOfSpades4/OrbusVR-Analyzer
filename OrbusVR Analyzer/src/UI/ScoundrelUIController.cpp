#include "ScoundrelUIController.h"
#include "Theme.h"

float bulletChargeThresholds[5] = { 0.1f, 0.2f, 0.3f, 0.5f, 1.0f };
float cardUseThresholds[5] = { 0.5f, 0.7f, 1.0f, 1.5f, 2.0f };
std::string ratings[6] = { "S", "A", "B", "C", "D", "F" };
wxColor ratingColors[6] = { wxColor(51, 242, 255), wxColor(79, 255, 51), wxColor(241, 255, 51), wxColor(255, 183, 51), wxColor(255, 51, 51), wxColor(167, 0, 0) };
wxColor defaultTextColor;
int bulletLines = 0;
int cardLines = 0;

int shots = 0;
int cards = 0;
int totalBulletRating = 0;
int totalWastedBullets = 0;
float totalBulletDelay = 0;
int totalCardRating = 0;
int totalCardRankShot = 0;
float totalCardDelay = 0;
int totalPoisons = 0;
int totalFlames = 0;

ScoundrelUIController::ScoundrelUIController(wxWindow* parent, int offsetX, int offsetY, void* param)
{
	
	m_TextOrganizer = std::make_unique<wxStaticText>(parent, wxID_ANY, "Scoundrel", wxPoint(offsetX, offsetY));
	CONFIGURE_TEXT_THEME(m_TextOrganizer);
	m_TextOrganizer->SetFont(m_TextOrganizer->GetFont().Scale(1.8));

	////
	m_Bullet = std::make_unique<wxStaticText>(parent, wxID_ANY, "Bullets", wxPoint(offsetX + 620 + 300 + 25, offsetY + 80));
	CONFIGURE_TEXT_THEME(m_Bullet);
	m_Bullet->SetFont(m_Bullet->GetFont().Scale(1.6));

	m_BulletRanking = std::make_unique<wxStaticText>(parent, wxID_ANY, "Bullet Ranking: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 123));
	CONFIGURE_TEXT_THEME(m_BulletRanking);
	m_BulletRankingRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "N/A", wxPoint(offsetX + 740 + 300 + 25, offsetY + 120));
	m_BulletRankingRating->SetFont(m_BulletRankingRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_BulletRankingRating);
	m_BulletRanking->SetToolTip("Average Rating of All Bullets.");

	m_WastedBullets = std::make_unique<wxStaticText>(parent, wxID_ANY, "Wasted Bullets: 0/0", wxPoint(offsetX + 620 + 300 + 25, offsetY + 160));
	CONFIGURE_TEXT_THEME(m_WastedBullets);
	m_BulletDelay = std::make_unique<wxStaticText>(parent, wxID_ANY, "Bullet Delay: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 197));
	CONFIGURE_TEXT_THEME(m_BulletDelay);
	m_BulletDelayRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "N/A", wxPoint(offsetX + 720 + 300 + 25, offsetY + 197));
	CONFIGURE_TEXT_THEME(m_BulletDelayRating);
	////

	////
	m_Card = std::make_unique<wxStaticText>(parent, wxID_ANY, "Cards", wxPoint(offsetX + 620 + 300 + 25, offsetY + 320));
	CONFIGURE_TEXT_THEME(m_Card);
	m_Card->SetFont(m_Card->GetFont().Scale(1.6));

	m_CardRanking = std::make_unique<wxStaticText>(parent, wxID_ANY, "Card Ranking: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 123 + 240));
	CONFIGURE_TEXT_THEME(m_CardRanking);
	m_CardRankingRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "N/A", wxPoint(offsetX + 740 + 300 + 25, offsetY + 120 + 240));
	m_CardRankingRating->SetFont(m_CardRankingRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_CardRankingRating);
	m_CardRanking->SetToolTip("Average Rating of All Card Draws.");

	m_TotalPoison = std::make_unique<wxStaticText>(parent, wxID_ANY, "Poison Cards: 0/0", wxPoint(offsetX + 620 + 300 + 25, offsetY + 160 + 240));
	CONFIGURE_TEXT_THEME(m_TotalPoison);
	m_CardDelay = std::make_unique<wxStaticText>(parent, wxID_ANY, "Card Delay: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 197 + 240));
	CONFIGURE_TEXT_THEME(m_CardDelay);
	m_CardDelayRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "N/A", wxPoint(offsetX + 720 + 300 + 25, offsetY + 197 + 240));
	CONFIGURE_TEXT_THEME(m_CardDelayRating);
	////

	m_BulletConsole = std::make_unique<wxTextCtrl>(parent, wxID_ANY, "", wxPoint(offsetX, offsetY + 80), wxSize(600, 400), wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH);
	CONFIGURE_TEXT_THEME(m_BulletConsole);
	m_BulletConsole->SetFont(m_BulletConsole->GetFont().Scale(1.2));
	m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	m_CardConsole = std::make_unique<wxTextCtrl>(parent, wxID_ANY, "", wxPoint(offsetX + 625, offsetY + 80), wxSize(300, 400), wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH);
	CONFIGURE_TEXT_THEME(m_CardConsole);
	m_CardConsole->SetFont(m_CardConsole->GetFont().Scale(1.2));
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	defaultTextColor = m_BulletConsole->GetForegroundColour();
}

int GetRating(float val, float threshold[]) {
	for (int i = 0; i < 5; i++) {
		if (val <= threshold[i]) {
			return i;
		}
	}
	return 5;
}

int GetCardRating(Scoundrel::Bullet bullet) {
	if (bullet.card == Scoundrel::POISON) {
		if (bullet.empowerment == Scoundrel::COPY)
			return 0;

		if (bullet.empowerment == Scoundrel::EMPOWER)
			return 1;

		if (bullet.empowerment == Scoundrel::SPREAD)
			return 3;

		if (bullet.empowerment == Scoundrel::NO_EMPOWER)
			return 2;
	}

	else if (bullet.card == Scoundrel::FLAME) {
		if (bullet.empowerment == Scoundrel::COPY)
			return 3;

		if (bullet.empowerment == Scoundrel::EMPOWER)
			return 1;

		if (bullet.empowerment == Scoundrel::SPREAD)
			return 3;

		if (bullet.empowerment == Scoundrel::NO_EMPOWER)
			return 2;
	}

	else if (bullet.empowerment == Scoundrel::COPY) {
		return 4;
	}

	else if (bullet.card == Scoundrel::ASH || bullet.card == Scoundrel::FLINT) {
		return 4;
	}

	else if (bullet.empowerment == Scoundrel::EMPOWER && (bullet.card == Scoundrel::ICE || bullet.card == Scoundrel::LIGHT)) {
		return 2;
	}

	else {
		return 3;
	}
}

int GetCardRating(Scoundrel::Card card, Scoundrel::Empowerment empowerment) {
	if (card == Scoundrel::POISON) {
		if (empowerment == Scoundrel::COPY)
			return 0;

		if (empowerment == Scoundrel::EMPOWER)
			return 1;

		if (empowerment == Scoundrel::SPREAD)
			return 3;

		if (empowerment == Scoundrel::NO_EMPOWER)
			return 2;
	}

	else if (card == Scoundrel::FLAME) {
		if (empowerment == Scoundrel::COPY)
			return 3;

		if (empowerment == Scoundrel::EMPOWER)
			return 1;

		if (empowerment == Scoundrel::SPREAD)
			return 3;

		if (empowerment == Scoundrel::NO_EMPOWER)
			return 2;
	}

	else if (empowerment == Scoundrel::COPY) {
		return 4;
	}

	else if (card == Scoundrel::ASH || card == Scoundrel::FLINT) {
		return 4;
	}

	else if (empowerment == Scoundrel::EMPOWER && (card == Scoundrel::ICE || card == Scoundrel::LIGHT)) {
		return 2;
	}

	else {
		return 3;
	}
}

int GetCardRating(Scoundrel::Card card) {
	switch (card) {
		case Scoundrel::POISON: return 0;
		case Scoundrel::FLAME: return 1;
		case Scoundrel::ICE: return 2;
		case Scoundrel::LIGHT: return 2;
		case Scoundrel::ASH: return 4;
		case Scoundrel::FLINT: return 4;
	}
	return 5;
}

int GetCardRating(Scoundrel::Empowerment empower) {
	switch (empower) {
	case Scoundrel::EMPOWER: return 1;
	case Scoundrel::SPREAD: return 4;
	case Scoundrel::COPY: return 0;
	}
	return 5;
}

void ScoundrelUIController::PrintActivateCard() {
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	if (Scoundrel::activeCard != Scoundrel::NO_CARD) {
		m_CardConsole->WriteText("      Activated: ");

		if (Scoundrel::activeCard == Scoundrel::FLAME) {
			totalFlames++;
		}
		if (Scoundrel::activeCard == Scoundrel::POISON) {
			totalPoisons++;
		}

		m_TotalPoison->SetLabelText("Poison Cards: " + std::to_string(totalPoisons) + "/" + std::to_string(cards));

		m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetCardRating(Scoundrel::activeCard, Scoundrel::empowerment)]));
		m_CardConsole->WriteText(to_string(Scoundrel::empowerment) + " " + to_string(Scoundrel::activeCard));
		m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

		m_CardConsole->WriteText("\n");
		cardLines++;

		m_CardConsole->Freeze();
		m_CardConsole->ScrollLines(cardLines);
		m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
		m_CardConsole->Thaw();

		Scoundrel::activeCard = Scoundrel::NO_CARD;
		Scoundrel::empowerment = Scoundrel::NO_EMPOWER;
	}
}

void ScoundrelUIController::PrintUseCard(Scoundrel::Card card) {
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	Scoundrel::activeCard = card;

	m_CardConsole->WriteText("      Use: "); 

	m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetCardRating(card)]));
	m_CardConsole->WriteText(to_string(card));
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	m_CardConsole->WriteText("\n");
	cardLines++;

	m_CardConsole->Freeze();
	m_CardConsole->ScrollLines(cardLines);
	m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
	m_CardConsole->Thaw();
}

void ScoundrelUIController::PrintUseEmpower(Scoundrel::Empowerment empower) {
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	Scoundrel::empowerment = empower;

	m_CardConsole->WriteText("      Empower: ");

	m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetCardRating(empower)]));
	m_CardConsole->WriteText(to_string(empower));
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	m_CardConsole->WriteText("\n");
	cardLines++;

	m_CardConsole->Freeze();
	m_CardConsole->ScrollLines(cardLines);
	m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
	m_CardConsole->Thaw();
}

void ScoundrelUIController::PrintDrawCard(Scoundrel::Card card, float time, bool first) {
	if (first) {
		time = 0.4;
	}

	cards++;
	totalCardDelay += time;

	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	int rating = GetRating(time, cardUseThresholds);

	totalCardRating += rating;

	m_CardRankingRating->SetForegroundColour(ratingColors[(int)std::round(totalCardRating / cards)]);
	m_CardRankingRating->SetLabelText(ratings[(int)std::round(totalCardRating / cards)]);

	m_CardDelayRating->SetForegroundColour(ratingColors[GetRating(totalCardDelay / cards, cardUseThresholds)]);
	m_CardDelayRating->SetLabelText(std::to_string(totalCardDelay / cards));

	m_CardConsole->WriteText("  ");
	m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[rating]));
	m_CardConsole->WriteText(ratings[rating]);
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	m_CardConsole->WriteText(": Draw ");

	m_CardConsole->WriteText(to_string(card));

	m_CardConsole->WriteText(" (");
	m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[rating]));
	m_CardConsole->WriteText(std::to_string(time) + "s");
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	m_CardConsole->WriteText(")");

	m_CardConsole->WriteText("\n");
	cardLines++;

	m_CardConsole->Freeze();
	m_CardConsole->ScrollLines(cardLines);
	m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
	m_CardConsole->Thaw();
}

void ScoundrelUIController::PrintShot(Scoundrel::Bullet bullet, bool first) {
	std::string string = "";
	if (first) {
		bullet.wastedTime = 0;
	}
	int rating = GetRating(bullet.wastedTime, bulletChargeThresholds);

	rating += (3 - (int)((bullet.rank + 1) / 2));

	if (rating > 5)
		rating = 5;

	if (!bullet.hitStatus)
		rating = 5;

	shots++;
	totalBulletDelay += bullet.wastedTime;
	totalBulletRating += rating;

	m_WastedBullets->SetLabelText("Wasted Bullets: " + std::to_string(totalWastedBullets) + "/" + std::to_string(shots + totalWastedBullets));
	m_BulletDelayRating->SetForegroundColour(ratingColors[GetRating(totalBulletDelay / shots, bulletChargeThresholds)]);
	m_BulletDelayRating->SetLabelText(std::to_string(totalBulletDelay / (float)shots));

	m_BulletRankingRating->SetForegroundColour(ratingColors[(int)std::round(totalBulletRating / shots)]);
	m_BulletRankingRating->SetLabelText(ratings[(int)std::round(totalBulletRating / shots)]);


	m_BulletConsole->WriteText("  ");
	m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[rating]));
	m_BulletConsole->WriteText(ratings[rating]);
	m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	m_BulletConsole->WriteText(": ");

	if (bullet.bulletsCharged > 1) {
		string = std::to_string(bullet.bulletsCharged) + " Bullet Charge";
	}
	else {
		string = "Normal Shot";
	}
	m_BulletConsole->WriteText(string);

	if (!bullet.hitStatus) {
		m_BulletConsole->WriteText(" (");
		m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[rating]));
		m_BulletConsole->WriteText("Miss");
		m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
		m_BulletConsole->WriteText(")");
	}
	else {
		/*
		if (bullet.card != Scoundrel::Card::NO_CARD) {
			string = "";
			m_BulletConsole->WriteText(" (");

			m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetCardRating(bullet)]));
			if (bullet.empowerment != Scoundrel::Empowerment::NO_EMPOWER) {
				string += to_string(bullet.empowerment);
				string += " ";
			}
			string += to_string(bullet.card);
			m_BulletConsole->WriteText(string);
			m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

			m_BulletConsole->WriteText(")");
		}
		*/

		m_BulletConsole->WriteText(" (");

		m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[5 - bullet.rank]));
		string = "Rank ";
		string += std::to_string(bullet.rank);
		m_BulletConsole->WriteText(string);
		m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

		m_BulletConsole->WriteText(")");

		m_BulletConsole->WriteText(" (");

		m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetRating(bullet.wastedTime, bulletChargeThresholds)]));
		string = std::to_string(bullet.wastedTime);
		string += "s";
		m_BulletConsole->WriteText(string);
		m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

		m_BulletConsole->WriteText(")");
	}
	m_BulletConsole->WriteText("\n");
	bulletLines++;

	m_BulletConsole->Freeze();
	m_BulletConsole->ScrollLines(bulletLines);
	m_BulletConsole->ShowPosition(m_BulletConsole->GetLastPosition());
	m_BulletConsole->Thaw();
}

void ScoundrelUIController::WastedBullet() {
	totalWastedBullets++;
	m_WastedBullets->SetLabelText("Wasted Bullets: " + std::to_string(totalWastedBullets) + "/" + std::to_string(shots + totalWastedBullets));

	m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[4]));
	m_BulletConsole->WriteText("  Wasted Bullet");
	m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	m_BulletConsole->WriteText("\n");
	bulletLines++;

	m_BulletConsole->Freeze();
	m_BulletConsole->ScrollLines(bulletLines);
	m_BulletConsole->ShowPosition(m_BulletConsole->GetLastPosition());
	m_BulletConsole->Thaw();
}

void ScoundrelUIController::Reset() {
	shots = 0;
	cards = 0;
	totalBulletRating = 0;
	totalWastedBullets = 0;
	totalCardRating = 0;
	totalBulletDelay = 0;
	totalCardDelay = 0;
	totalPoisons = 0;
	totalFlames = 0;
	totalCardRankShot = 0;

	Scoundrel::Reset();

	m_BulletConsole->Clear();
	m_CardConsole->Clear();

	m_BulletRankingRating->SetForegroundColour(defaultTextColor);
	m_BulletRankingRating->SetLabelText("N/A");
	m_WastedBullets->SetLabelText("Wasted Bullets: 0/0");
	m_BulletDelayRating->SetForegroundColour(defaultTextColor);
	m_BulletDelayRating->SetLabelText("N/A");

	m_CardRankingRating->SetForegroundColour(defaultTextColor);
	m_CardRankingRating->SetLabelText("N/A");
	m_TotalPoison->SetLabelText("Poison Cards: 0/0");
	m_CardDelayRating->SetForegroundColour(defaultTextColor);
	m_CardDelayRating->SetLabelText("N/A");
}

void UnbindPlayer(wxEvent& ev)
{
	Scoundrel::ResetPlayer();
	ev.Skip();
}

ScoundrelUIController::~ScoundrelUIController()
{
}