#include "ScoundrelUIController.h"
#include "Theme.h"

float bulletChargeThresholds[5] = { 0.1f, 0.2f, 0.3f, 0.5f, 1.0f };
std::string ratings[6] = { "S", "A", "B", "C", "D", "F" };
wxColor ratingColors[6] = { wxColor(51, 242, 255), wxColor(79, 255, 51), wxColor(241, 255, 51), wxColor(255, 183, 51), wxColor(255, 51, 51), wxColor(167, 0, 0) };
wxColor defaultTextColor;
float bulletLines = 0;

ScoundrelUIController::ScoundrelUIController(wxWindow* parent, int offsetX, int offsetY, void* param)
{
	m_TextOrganizer = std::make_unique<wxStaticText>(parent, wxID_ANY, "Scoundrel", wxPoint(offsetX, offsetY));
	CONFIGURE_TEXT_THEME(m_TextOrganizer);
	m_TextOrganizer->SetFont(m_TextOrganizer->GetFont().Scale(2.0));

	m_BulletConsole = std::make_unique<wxTextCtrl>(parent, wxID_ANY, "", wxPoint(offsetX, offsetY + 80), wxSize(600, 400), wxTE_READONLY | wxTE_MULTILINE | wxTE_RICH);
	CONFIGURE_TEXT_THEME(m_BulletConsole);
	m_BulletConsole->SetFont(m_BulletConsole->GetFont().Scale(1.2));
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

void ScoundrelUIController::PrintShot(Scoundrel::Bullet bullet) {
	std::string string = "";
	int rating = GetRating(bullet.wastedTime, bulletChargeThresholds);

	rating += (3 - (int)((bullet.rank + 1) / 2));

	if (rating > 5)
		rating = 5;

	if (!bullet.hitStatus)
		rating = 5;

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
		if (bullet.card != Scoundrel::Card::NO_CARD) {
			string = "";
			m_BulletConsole->WriteText(" (");

			if (bullet.empowerment != Scoundrel::Empowerment::NO_EMPOWER) {
				string += to_string(bullet.empowerment);
				string += " ";
			}
			string += to_string(bullet.card);
			m_BulletConsole->WriteText(string);

			m_BulletConsole->WriteText(")");
		}

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
	// m_BulletConsole->Clear();
}

ScoundrelUIController::~ScoundrelUIController()
{
}