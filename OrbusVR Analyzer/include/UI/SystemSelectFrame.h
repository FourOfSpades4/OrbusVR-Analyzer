#pragma once

#include <WinSock2.h>
#include <wx/msw/winundef.h>
#include <wx/wx.h>
#include <memory>
#include <VRSystem.h>
#include <NotificationCallback.h>

class SystemSelectFrame : public wxFrame
{
public:
	SystemSelectFrame(NotificationCallback_t notificationCallback, void* param);
	~SystemSelectFrame();

public:
	inline const VrSystem::System GetSelectedSystem() const { return m_ChosenSystem; }

public:
	VrSystem::System m_ChosenSystem;

private:
	void OnSystemSelectChooserSelect(wxEvent& ev);
	void OnFinishButtonClick(wxEvent& ev);

private:
	NotificationCallback_t m_NotificationCallback;
	void* m_NotificationCallbackParam;

private:
	std::unique_ptr<wxPanel> m_BackgroundPanel;
	std::unique_ptr<wxStaticText> m_TextOrganizer;
	std::unique_ptr<wxChoice> m_SystemSelectChooser;
	std::unique_ptr<wxButton> m_FinishButton;
};

