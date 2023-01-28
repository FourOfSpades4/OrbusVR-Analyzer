#include "ScoundrelUIController.h"
#include "Theme.h"
#include <map>

float bulletChargeThresholds[5] = { 0.1f, 0.2f, 0.3f, 0.5f, 1.0f };
float cardUseThresholds[5] = { 0.5f, 0.7f, 1.0f, 1.5f, 2.0f };
float wastedBulletThresholds[5] = { 0.0f, 0.05f, 0.1f, 0.2f, 0.3f };
float rankShotThresholds[5] = { 0.1f, 1.0f, 2.0f, 3.0f, 4.0f };
float poisonCardThresholds[5] = { 0.65f, 0.725f, 0.825f, 0.875f, 0.95f };
std::string ratings[6] = { "S", "A", "B", "C", "D", "F" };
wxColor ratingColors[6] = { wxColor(51, 242, 255), wxColor(79, 255, 51), wxColor(241, 255, 51), wxColor(255, 183, 51), wxColor(255, 51, 51), wxColor(167, 0, 0) };
wxColor defaultTextColor;
int bulletLines = 0;
int cardLines = 0;

std::map<std::string, Scoundrel::ScoundrelUICharacter*> uiPlayers;


ScoundrelUIController::ScoundrelUIController(wxWindow* parent, int offsetX, int offsetY, void* param)
{
	
	int ratingOffset = 740;

	m_TextOrganizer = std::make_unique<wxStaticText>(parent, wxID_ANY, "Scoundrel", wxPoint(offsetX, offsetY));
	CONFIGURE_TEXT_THEME(m_TextOrganizer);
	m_TextOrganizer->SetFont(m_TextOrganizer->GetFont().Scale(1.8));

	m_PlayerName = std::make_unique<wxStaticText>(parent, wxID_ANY, "No Player Found", wxPoint(offsetX, offsetY + 40));
	CONFIGURE_TEXT_THEME(m_PlayerName);
	m_PlayerName->SetFont(m_PlayerName->GetFont().Scale(1.5));

	////
	m_Bullet = std::make_unique<wxStaticText>(parent, wxID_ANY, "Bullets", wxPoint(offsetX + 620 + 300 + 25, offsetY + 80));
	CONFIGURE_TEXT_THEME(m_Bullet);
	m_Bullet->SetFont(m_Bullet->GetFont().Scale(1.6));

	m_BulletRanking = std::make_unique<wxStaticText>(parent, wxID_ANY, "Bullet Ranking: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 123));
	CONFIGURE_TEXT_THEME(m_BulletRanking);
	m_BulletRankingRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "N/A", wxPoint(offsetX + ratingOffset + 300 + 25, offsetY + 120));
	m_BulletRankingRating->SetFont(m_BulletRankingRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_BulletRankingRating);
	m_BulletRanking->SetToolTip("Average Rating of All Bullets.");


	m_WastedBullets = std::make_unique<wxStaticText>(parent, wxID_ANY, "Wasted Bullets: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 163));
	CONFIGURE_TEXT_THEME(m_WastedBullets);
	m_WastedBulletsRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "0/0", wxPoint(offsetX + ratingOffset + 300 + 25, offsetY + 160));
	m_WastedBulletsRating->SetFont(m_WastedBulletsRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_WastedBulletsRating);
	

	m_BulletRankShot = std::make_unique<wxStaticText>(parent, wxID_ANY, "Average Rank: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 200));
	CONFIGURE_TEXT_THEME(m_BulletRankShot);
	m_BulletRankShotRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "0", wxPoint(offsetX + ratingOffset + 300 + 25, offsetY + 197));
	m_BulletRankShotRating->SetFont(m_BulletRankShotRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_BulletRankShotRating);

	m_BulletDelay = std::make_unique<wxStaticText>(parent, wxID_ANY, "Bullet Delay: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 237));
	CONFIGURE_TEXT_THEME(m_BulletDelay);
	m_BulletDelayRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "N/A", wxPoint(offsetX + ratingOffset + 300 + 25, offsetY + 234));
	m_BulletDelayRating->SetFont(m_BulletDelayRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_BulletDelayRating);

	////
	m_Card = std::make_unique<wxStaticText>(parent, wxID_ANY, "Cards", wxPoint(offsetX + 620 + 300 + 25, offsetY + 320));
	CONFIGURE_TEXT_THEME(m_Card);
	m_Card->SetFont(m_Card->GetFont().Scale(1.6));

	m_CardRanking = std::make_unique<wxStaticText>(parent, wxID_ANY, "Card Ranking: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 123 + 243));
	CONFIGURE_TEXT_THEME(m_CardRanking);
	m_CardRankingRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "N/A", wxPoint(offsetX + ratingOffset + 300 + 25, offsetY + 123 + 240));
	m_CardRankingRating->SetFont(m_CardRankingRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_CardRankingRating);
	// m_CardRanking->SetToolTip("Average Rating of All Card Draws.");

	m_TotalPoison = std::make_unique<wxStaticText>(parent, wxID_ANY, "Poison Cards: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 160 + 243));
	CONFIGURE_TEXT_THEME(m_TotalPoison);
	m_TotalPoisonRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "0/0", wxPoint(offsetX + ratingOffset + 300 + 25, offsetY + 160 + 240));
	m_TotalPoisonRating->SetFont(m_TotalPoisonRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_TotalPoisonRating);

	m_CardDelay = std::make_unique<wxStaticText>(parent, wxID_ANY, "Card Delay: ", wxPoint(offsetX + 620 + 300 + 25, offsetY + 197 + 243));
	CONFIGURE_TEXT_THEME(m_CardDelay);
	m_CardDelayRating = std::make_unique<wxStaticText>(parent, wxID_ANY, "N/A", wxPoint(offsetX + ratingOffset + 300 + 25, offsetY + 197 + 240));
	m_CardDelayRating->SetFont(m_CardDelayRating->GetFont().Scale(1.3));
	CONFIGURE_TEXT_THEME(m_CardDelayRating);
	////

	m_BulletConsole = std::make_unique<wxTextCtrl>(parent, wxID_ANY, "", wxPoint(offsetX, offsetY + 80), wxSize(600, 400), wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH);
	CONFIGURE_TEXT_THEME(m_BulletConsole);
	defaultTextColor = m_BulletConsole->GetForegroundColour();
	m_BulletConsole->SetFont(m_BulletConsole->GetFont().Scale(1.2));
	m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	m_CardConsole = std::make_unique<wxTextCtrl>(parent, wxID_ANY, "", wxPoint(offsetX + 625, offsetY + 80), wxSize(300, 400), wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH);
	CONFIGURE_TEXT_THEME(m_CardConsole);
	m_CardConsole->SetFont(m_CardConsole->GetFont().Scale(1.2));
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	m_BulletRotationConsole = std::make_unique<wxTextCtrl>(parent, wxID_ANY, "", wxPoint(offsetX, offsetY + 500), wxSize(600, 30), wxTE_READONLY | wxTE_NO_VSCROLL | wxTE_RICH);
	CONFIGURE_TEXT_THEME(m_BulletRotationConsole);
	m_BulletRotationConsole->SetFont(m_BulletRotationConsole->GetFont().Scale(1.15));
	m_BulletRotationConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

}

void ScoundrelUIController::SetPlayer(Scoundrel::ScoundrelCharacter* character) {
	m_PlayerName->SetLabelText(character->name);
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




void ScoundrelUIController::EnsureUICharacterExists(Scoundrel::ScoundrelCharacter* character) {
	if (!uiPlayers.contains(character->name)) {
		Scoundrel::ScoundrelUICharacter* uiCharacter = new Scoundrel::ScoundrelUICharacter;
		uiCharacter->maxBullets = character->maxBullets;
		uiPlayers.insert(std::make_pair(character->name, uiCharacter));
	}
}





void ScoundrelUIController::PrintActivateCard(Scoundrel::ScoundrelUICharacter* character, Scoundrel::CardArguement arguement) {
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	if (character->card != Scoundrel::NO_CARD) {
		m_CardConsole->WriteText("      Activated: ");

		if (character->card == Scoundrel::FLAME) {
			character->totalFlames++;
		}
		if (character->card == Scoundrel::POISON) {
			character->totalPoisons++;
		}

		m_TotalPoisonRating->SetForegroundColour(ratingColors[GetRating(1.0 - ((float)character->totalPoisons / (float)character->cards), poisonCardThresholds)]);
		m_TotalPoisonRating->SetLabelText(std::to_string(character->totalPoisons) + "/" + std::to_string(character->cards));

		m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetCardRating(character->card, character->empowerment)]));
		m_CardConsole->WriteText(to_string(character->empowerment) + " " + to_string(character->card));
		m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

		character->card = Scoundrel::Card::NO_CARD;
		character->empowerment = Scoundrel::Empowerment::NO_EMPOWER;

		m_CardConsole->WriteText("\n");
		cardLines++;

		m_CardConsole->Freeze();
		m_CardConsole->ScrollLines(cardLines);
		m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
		m_CardConsole->Thaw();
	}
}

void ScoundrelUIController::ProcessActivateCard(Scoundrel::ScoundrelCharacter* character) {
	printf("Activate Card Called\n");
	EnsureUICharacterExists(character);
	Scoundrel::ScoundrelUICharacter *uiCharacter = uiPlayers.find(character->name)->second;
	Scoundrel::CardArguement arguement;
	arguement.arguementType = Scoundrel::CardArguement::CardArguementType::ActivateCard;
	arguement.card = character->activeCard;
	arguement.empowerment = character->empowerment;
	uiCharacter->cardArguements.push_back(arguement);
	
	// if (character->name == std::string(ScoundrelUIController::playerChoice->GetStringSelection().mb_str())) {
	if (true) {
		PrintActivateCard(uiCharacter, arguement);
	}
	
}




void ScoundrelUIController::PrintUseCard(Scoundrel::ScoundrelUICharacter* character, Scoundrel::CardArguement arguement) {
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	character->card = arguement.card;

	m_CardConsole->WriteText("      Use: ");

	m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetCardRating(arguement.card)]));
	m_CardConsole->WriteText(to_string(arguement.card));
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	m_CardConsole->WriteText("\n");
	cardLines++;

	m_CardConsole->Freeze();
	m_CardConsole->ScrollLines(cardLines);
	m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
	m_CardConsole->Thaw();
}

void ScoundrelUIController::ProcessUseCard(Scoundrel::ScoundrelCharacter* character, Scoundrel::Card card) {
	printf("Use Card Called\n");
	EnsureUICharacterExists(character);
	Scoundrel::ScoundrelUICharacter *uiCharacter = uiPlayers.find(character->name)->second;
	Scoundrel::CardArguement arguement;
	arguement.card = card;
	arguement.arguementType = Scoundrel::CardArguement::CardArguementType::UseCard;
	uiCharacter->cardArguements.push_back(arguement);
	
	// if (character->name == std::string(ScoundrelUIController::playerChoice->GetStringSelection().mb_str())) {
	if (true) {
		PrintUseCard(uiCharacter, arguement);
	}
	
}





void ScoundrelUIController::PrintUseEmpower(Scoundrel::ScoundrelUICharacter* character, Scoundrel::CardArguement arguement) {
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	character->empowerment = arguement.empowerment;

	m_CardConsole->WriteText("      Empower: ");

	m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetCardRating(arguement.empowerment)]));
	m_CardConsole->WriteText(to_string(arguement.empowerment));
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

	m_CardConsole->WriteText("\n");
	cardLines++;

	m_CardConsole->Freeze();
	m_CardConsole->ScrollLines(cardLines);
	m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
	m_CardConsole->Thaw();
}

void ScoundrelUIController::ProcessUseEmpower(Scoundrel::ScoundrelCharacter* character, Scoundrel::Empowerment empower) {
	printf("Empower Called\n");
	EnsureUICharacterExists(character);
	Scoundrel::ScoundrelUICharacter *uiCharacter = uiPlayers.find(character->name)->second;
	Scoundrel::CardArguement arguement;
	arguement.empowerment = empower;
	arguement.arguementType = Scoundrel::CardArguement::CardArguementType::UseEmpower;
	uiCharacter->cardArguements.push_back(arguement);

	// if (character->name == std::string(ScoundrelUIController::playerChoice->GetStringSelection().mb_str())) {
	if (true) {
		PrintUseEmpower(uiCharacter, arguement);
	}
	
}




void ScoundrelUIController::PrintDrawCard(Scoundrel::ScoundrelUICharacter* character, Scoundrel::CardArguement arguement) {
	if (arguement.first) {
		arguement.time = 0.4;
	}
	character->cards++;
	character->totalCardDelay += arguement.time;

	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	int rating = GetRating(arguement.time, cardUseThresholds);

	character->totalCardRating += rating;

	int totalRating = 2 * GetRating(character->totalCardDelay / character->cards, cardUseThresholds) + GetRating(1.0 - ((float)character->totalPoisons / (float)character->cards), poisonCardThresholds);
	m_CardRankingRating->SetForegroundColour(ratingColors[(int)std::round(totalRating / 3.0)]);
	m_CardRankingRating->SetLabelText(ratings[(int)std::round(totalRating / 3.0)]);

	m_CardDelayRating->SetForegroundColour(ratingColors[GetRating(character->totalCardDelay / character->cards, cardUseThresholds)]);
	m_CardDelayRating->SetLabelText(std::to_string(character->totalCardDelay / character->cards));

	m_CardConsole->WriteText("  ");
	m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[rating]));
	m_CardConsole->WriteText(ratings[rating]);
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	m_CardConsole->WriteText(": Draw ");

	m_CardConsole->WriteText(to_string(arguement.card));

	m_CardConsole->WriteText(" (");
	m_CardConsole->SetDefaultStyle(wxTextAttr(ratingColors[rating]));
	m_CardConsole->WriteText(std::to_string(arguement.time) + "s");
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	m_CardConsole->WriteText(")");

	m_CardConsole->WriteText("\n");
	cardLines++;

	m_CardConsole->Freeze();
	m_CardConsole->ScrollLines(cardLines);
	m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
	m_CardConsole->Thaw();
}

void ScoundrelUIController::ProcessDrawCard(Scoundrel::ScoundrelCharacter* character, Scoundrel::Card card, float time, bool first) {
	printf("Draw Card Called\n");
	EnsureUICharacterExists(character);
	Scoundrel::ScoundrelUICharacter *uiCharacter = uiPlayers.find(character->name)->second;
	Scoundrel::CardArguement arguement;
	arguement.card = card;
	arguement.time = time;
	arguement.first = first;
	arguement.arguementType = Scoundrel::CardArguement::CardArguementType::DrawCard;
	uiCharacter->cardArguements.push_back(arguement);

	// if (character->name == std::string(ScoundrelUIController::playerChoice->GetStringSelection().mb_str())) {
	if (true) {
		PrintDrawCard(uiCharacter, arguement);
	}
	
}




void ScoundrelUIController::PrintShot(Scoundrel::ScoundrelUICharacter* character, Scoundrel::BulletArguement arguement) {
	std::string string = "";
	if (arguement.first) {
		arguement.bullet.wastedTime = 0;
	}

	int rating = GetRating(arguement.bullet.wastedTime, bulletChargeThresholds);

	rating += (3 - (int)((arguement.bullet.rank + 1) / 2));

	if (rating > 5)
		rating = 5;

	if (!arguement.bullet.hitStatus)
		rating = 5;

	character->shots++;
	character->totalCardRankShot += arguement.bullet.rank;
	character->totalBulletDelay += arguement.bullet.wastedTime;
	character->totalBulletRating += rating;

	m_BulletRankShotRating->SetForegroundColour(ratingColors[GetRating(5.0 - ((float)character->totalCardRankShot / character->shots), rankShotThresholds)]);
	m_BulletRankShotRating->SetLabelText(std::to_string((float)character->totalCardRankShot / character->shots));

	m_BulletDelayRating->SetForegroundColour(ratingColors[GetRating(character->totalBulletDelay / character->shots, bulletChargeThresholds)]);
	m_BulletDelayRating->SetLabelText(std::to_string(character->totalBulletDelay / (float)character->shots));

	m_BulletRankingRating->SetForegroundColour(ratingColors[(int)std::round(character->totalBulletRating / character->shots)]);
	m_BulletRankingRating->SetLabelText(ratings[(int)std::round(character->totalBulletRating / character->shots)]);


	m_BulletConsole->WriteText("  ");
	m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[rating]));
	m_BulletConsole->WriteText(ratings[rating]);
	m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	m_BulletConsole->WriteText(": ");

	if (arguement.bullet.bulletsCharged > 1) {
		string = std::to_string(arguement.bullet.bulletsCharged) + " Bullet Charge";
	}
	else {
		string = "Normal Shot";
	}
	m_BulletConsole->WriteText(string);

	if (!arguement.bullet.hitStatus) {
		m_BulletConsole->WriteText(" (");
		m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[rating]));
		m_BulletConsole->WriteText("Miss");
		m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
		m_BulletConsole->WriteText(")");

		character->totalWastedBullets += arguement.bullet.bulletsCharged;
	}
	else {
		character->bullets+=arguement.bullet.bulletsCharged;
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

		m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[5 - arguement.bullet.rank]));
		string = "Rank ";
		string += std::to_string(arguement.bullet.rank);
		m_BulletConsole->WriteText(string);
		m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

		m_BulletConsole->WriteText(")");

		m_BulletConsole->WriteText(" (");

		m_BulletConsole->SetDefaultStyle(wxTextAttr(ratingColors[GetRating(arguement.bullet.wastedTime, bulletChargeThresholds)]));
		string = std::to_string(arguement.bullet.wastedTime);
		string += "s";
		m_BulletConsole->WriteText(string);
		m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));

		m_BulletConsole->WriteText(")");


		if (arguement.bullet.bulletsCharged == character->maxBullets) {
			m_BulletRotationConsole->SetDefaultStyle(ratingColors[0]);
			m_BulletRotationConsole->WriteText(" ");
		}
		else if (arguement.bullet.bulletsCharged > 1) {
			m_BulletRotationConsole->SetDefaultStyle(ratingColors[4]);
		}
		else {
			m_BulletRotationConsole->SetDefaultStyle(ratingColors[1]);
		}
		m_BulletRotationConsole->WriteText(std::to_string(arguement.bullet.bulletsCharged));
		m_BulletRotationConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	}

	m_WastedBulletsRating->SetForegroundColour(ratingColors[GetRating((float)character->totalWastedBullets / (float)(character->bullets + character->totalWastedBullets), wastedBulletThresholds)]);
	m_WastedBulletsRating->SetLabelText(std::to_string(character->totalWastedBullets) + "/" + std::to_string(character->bullets + character->totalWastedBullets));

	m_BulletConsole->WriteText("\n");
	bulletLines++;

	m_BulletConsole->Freeze();
	m_BulletConsole->ScrollLines(bulletLines);
	m_BulletConsole->ShowPosition(m_BulletConsole->GetLastPosition());
	m_BulletConsole->Thaw();
}


void ScoundrelUIController::ProcessShot(Scoundrel::ScoundrelCharacter* character, Scoundrel::Bullet bullet, bool first) {
	EnsureUICharacterExists(character);
	Scoundrel::ScoundrelUICharacter *uiCharacter = uiPlayers.find(character->name)->second;
	Scoundrel::BulletArguement arguement;
	arguement.bullet = bullet;
	arguement.first = first;
	arguement.arguementType = Scoundrel::BulletArguement::BulletArguementType::ShotBullet;
	uiCharacter->bulletArguements.push_back(arguement);

	// if (character->name == std::string(ScoundrelUIController::playerChoice->GetStringSelection().mb_str())) {
	if (true) {
		PrintShot(uiCharacter, arguement);
	}
	
}






void ScoundrelUIController::PrintWastedBullet(Scoundrel::ScoundrelUICharacter* character, Scoundrel::BulletArguement arguement) {
	character->totalWastedBullets++;

	m_WastedBulletsRating->SetForegroundColour(ratingColors[GetRating((float)character->totalWastedBullets / (float)(character->bullets + character->totalWastedBullets), wastedBulletThresholds)]);
	m_WastedBulletsRating->SetLabelText(std::to_string(character->totalWastedBullets) + "/" + std::to_string(character->bullets + character->totalWastedBullets));

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

void ScoundrelUIController::ProcessWastedBullet(Scoundrel::ScoundrelCharacter* character) {
	EnsureUICharacterExists(character);
	Scoundrel::ScoundrelUICharacter *uiCharacter = uiPlayers.find(character->name)->second;
	Scoundrel::BulletArguement arguement;
	arguement.arguementType = Scoundrel::BulletArguement::BulletArguementType::WastedBullet;
	uiCharacter->bulletArguements.push_back(arguement);

	// if (character->name == std::string(ScoundrelUIController::playerChoice->GetStringSelection().mb_str())) {
	if (true) {
		PrintWastedBullet(uiCharacter, arguement);
	}
	
}




void ScoundrelUIController::PrintBulletEndCombat(Scoundrel::ScoundrelUICharacter* character) {
	m_BulletConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	m_BulletConsole->WriteText("  Combat End!\n");

	m_BulletConsole->WriteText("\n");
	bulletLines++;

	m_BulletConsole->Freeze();
	m_BulletConsole->ScrollLines(bulletLines);
	m_BulletConsole->ShowPosition(m_BulletConsole->GetLastPosition());
	m_BulletConsole->Thaw();
}

void ScoundrelUIController::PrintCardEndCombat(Scoundrel::ScoundrelUICharacter* character) {
	m_CardConsole->SetDefaultStyle(wxTextAttr(defaultTextColor));
	m_CardConsole->WriteText("  Combat End!\n");

	m_CardConsole->WriteText("\n");
	cardLines++;

	m_CardConsole->Freeze();
	m_CardConsole->ScrollLines(cardLines);
	m_CardConsole->ShowPosition(m_CardConsole->GetLastPosition());
	m_CardConsole->Thaw();
}

void ScoundrelUIController::ProcessEndCombat(Scoundrel::ScoundrelCharacter* character) {
	EnsureUICharacterExists(character);
	Scoundrel::ScoundrelUICharacter *uiCharacter = uiPlayers.find(character->name)->second;

	std::vector<Scoundrel::BulletArguement>::reverse_iterator i = uiCharacter->bulletArguements.rbegin();

	int lines = 0;

	while (i != uiCharacter->bulletArguements.rend())
	{
		if ((*i).arguementType == Scoundrel::BulletArguement::BulletArguementType::WastedBullet) {
			uiCharacter->bulletArguements.erase(std::next(i).base());
			uiCharacter->totalWastedBullets--;
			lines++;
		}
		else if ((*i).arguementType == Scoundrel::BulletArguement::BulletArguementType::ShotBullet && (*i).bullet.hitStatus == false) {
			uiCharacter->bulletArguements.erase(std::next(i).base());
			uiCharacter->totalWastedBullets -= (*i).bullet.bulletsCharged;
			lines++;
		}
		else {
			break;
		}
		++i;
	}

	int nNumLines = m_BulletConsole->GetNumberOfLines();
	int nStartLastLine = m_BulletConsole->XYToPosition(0, nNumLines - lines - 1);
	int nEndLastLine = m_BulletConsole->GetLastPosition() + 1;
	m_BulletConsole->Remove(nStartLastLine, nEndLastLine);

	m_WastedBulletsRating->SetForegroundColour(ratingColors[GetRating((float)uiCharacter->totalWastedBullets / (float)(uiCharacter->bullets + uiCharacter->totalWastedBullets), wastedBulletThresholds)]);
	m_WastedBulletsRating->SetLabelText(std::to_string(uiCharacter->totalWastedBullets) + "/" + std::to_string(uiCharacter->bullets + uiCharacter->totalWastedBullets));

	Scoundrel::BulletArguement bArguement;
	Scoundrel::CardArguement cArguement;
	bArguement.arguementType = Scoundrel::BulletArguement::BulletArguementType::CombatEnd;
	cArguement.arguementType = Scoundrel::CardArguement::CardArguementType::CombatEnd;
	uiCharacter->bulletArguements.push_back(bArguement);
	uiCharacter->cardArguements.push_back(cArguement);

	/*
	if (uiPlayers.contains(ScoundrelUIController::playerChoice->GetStringSelection().ToStdString())) {
		ProcessPlayer(uiPlayers.find(ScoundrelUIController::playerChoice->GetStringSelection().ToStdString())->second);
	}
	*/
	if (true) {
		PrintCardEndCombat(uiCharacter);
		PrintBulletEndCombat(uiCharacter);
	}
}




void ScoundrelUIController::ProcessPlayer(Scoundrel::ScoundrelUICharacter* character) {
	for (Scoundrel::BulletArguement arguement : character->bulletArguements) {
		if (arguement.arguementType == Scoundrel::BulletArguement::BulletArguementType::ShotBullet) {
			PrintShot(character, arguement);
		}
		else if (arguement.arguementType == Scoundrel::BulletArguement::BulletArguementType::WastedBullet) {
			PrintWastedBullet(character, arguement);
		}
		else {
			PrintBulletEndCombat(character);
		}
	}

	for (Scoundrel::CardArguement arguement : character->cardArguements) {
		if (arguement.arguementType == Scoundrel::CardArguement::CardArguementType::UseCard) {
			PrintUseCard(character, arguement);
		}
		else if (arguement.arguementType == Scoundrel::CardArguement::CardArguementType::UseEmpower) {
			PrintUseEmpower(character, arguement);
		}
		else if (arguement.arguementType == Scoundrel::CardArguement::CardArguementType::DrawCard) {
			PrintDrawCard(character, arguement);
		}
		else if (arguement.arguementType == Scoundrel::CardArguement::CardArguementType::ActivateCard) {
			PrintActivateCard(character, arguement);
		}
		else {
			PrintCardEndCombat(character);
		}
	}
}



void ScoundrelUIController::Reset() {
	uiPlayers.clear();
	// playerChoice->Clear();
	// playerChoice->Append("");

	Scoundrel::Reset();

	m_BulletConsole->Clear();
	m_BulletRotationConsole->Clear();
	m_CardConsole->Clear();

	bulletLines = 0;
	cardLines = 0;

	m_BulletRankingRating->SetForegroundColour(defaultTextColor);
	m_BulletRankingRating->SetLabelText("N/A");
	m_WastedBulletsRating->SetForegroundColour(defaultTextColor);
	m_WastedBulletsRating->SetLabelText("0/0");
	m_BulletDelayRating->SetForegroundColour(defaultTextColor);
	m_BulletDelayRating->SetLabelText("N/A");
	m_BulletRankShotRating->SetForegroundColour(defaultTextColor);
	m_BulletRankShotRating->SetLabelText("0");


	m_CardRankingRating->SetForegroundColour(defaultTextColor);
	m_CardRankingRating->SetLabelText("N/A");
	m_TotalPoisonRating->SetForegroundColour(defaultTextColor);
	m_TotalPoisonRating->SetLabelText("0/0");
	m_CardDelayRating->SetForegroundColour(defaultTextColor);
	m_CardDelayRating->SetLabelText("N/A");
}


/*
void ScoundrelUIController::OnPlayerSelect(wxEvent& ev)
{
	m_BulletConsole->Clear();
	m_BulletRotationConsole->Clear();
	m_CardConsole->Clear();

	bulletLines = 0;
	cardLines = 0;

	m_BulletRankingRating->SetForegroundColour(defaultTextColor);
	m_BulletRankingRating->SetLabelText("N/A");
	m_WastedBullets->SetLabelText("Wasted Bullets: 0/0");
	m_BulletDelayRating->SetForegroundColour(defaultTextColor);
	m_BulletDelayRating->SetLabelText("N/A");
	m_BulletRankShot->SetLabelText("Average Rank: 0");


	m_CardRankingRating->SetForegroundColour(defaultTextColor);
	m_CardRankingRating->SetLabelText("N/A");
	m_TotalPoison->SetLabelText("Poison Cards: 0/0");
	m_CardDelayRating->SetForegroundColour(defaultTextColor);
	m_CardDelayRating->SetLabelText("N/A");

	std::cout << "Selected " + std::string(ScoundrelUIController::playerChoice->GetStringSelection().mb_str()) << std::endl;

	if (uiPlayers.contains(ScoundrelUIController::playerChoice->GetStringSelection().ToStdString())) {
		ProcessPlayer(uiPlayers.find(ScoundrelUIController::playerChoice->GetStringSelection().ToStdString())->second);
	}
	ev.Skip();
}
*/


ScoundrelUIController::~ScoundrelUIController()
{
}