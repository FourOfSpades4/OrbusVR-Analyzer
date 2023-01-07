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
	void PrintShot(Scoundrel::Bullet bullet);
	void Reset();
	void WastedBullet();

private:
	std::unique_ptr<wxTextCtrl> m_BulletConsole;
	std::unique_ptr<wxStaticText> m_TextOrganizer;
};