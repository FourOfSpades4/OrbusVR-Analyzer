#include "SystemSelectFrame.h"
#include "Theme.h"

SystemSelectFrame::SystemSelectFrame(NotificationCallback_t notificationCallback, void* param)
	: wxFrame(nullptr, wxID_ANY, "Select System", wxDefaultPosition, wxSize(360, 150), wxMINIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN), 
	  m_ChosenSystem(VrSystem::None), m_NotificationCallback(notificationCallback), m_NotificationCallbackParam(param)
{
	m_BackgroundPanel = std::make_unique<wxPanel>(this);
	m_BackgroundPanel->SetBackgroundColour(GuiTheme::GetPanelBackgroundColor());

	m_TextOrganizer = std::make_unique<wxStaticText>(m_BackgroundPanel.get(), wxID_ANY, "Please select a system before continuing");
	CONFIGURE_TEXT_THEME(m_TextOrganizer);
	m_TextOrganizer->SetFont(m_TextOrganizer->GetFont().Scale(1.2));

	m_SystemSelectChooser = std::make_unique<wxChoice>(m_BackgroundPanel.get(), wxID_ANY, wxDefaultPosition, wxSize(100, 25));
	CONFIGURE_TEXT_THEME(m_SystemSelectChooser);
	m_SystemSelectChooser->Append("Oculus");
	m_SystemSelectChooser->Append("Steam");
	m_SystemSelectChooser->Bind(wxEVT_COMMAND_CHOICE_SELECTED, &SystemSelectFrame::OnSystemSelectChooserSelect, this);

	m_FinishButton = std::make_unique<wxButton>(m_BackgroundPanel.get(), wxID_ANY, "Ok", wxDefaultPosition, wxSize(50, 25));
	CONFIGURE_TEXT_THEME(m_FinishButton);
	m_FinishButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &SystemSelectFrame::OnFinishButtonClick, this);

	m_SystemSelectChooser->SetPosition(wxPoint((GetSize().x / 2) - (m_SystemSelectChooser->GetSize().x + m_FinishButton->GetSize().x) / 2, (GetSize().y / 2) - m_SystemSelectChooser->GetSize().y));
	m_FinishButton->SetPosition(wxPoint(m_SystemSelectChooser->GetPosition().x + m_SystemSelectChooser->GetSize().x, m_SystemSelectChooser->GetPosition().y - 1));
	
	m_TextOrganizer->SetPosition(wxPoint((GetSize().x / 2) - m_TextOrganizer->GetSize().x / 2, m_SystemSelectChooser->GetPosition().y - m_TextOrganizer->GetSize().y - 5));
}

SystemSelectFrame::~SystemSelectFrame()
{
}

void SystemSelectFrame::OnSystemSelectChooserSelect(wxEvent& ev)
{
	m_ChosenSystem = (VrSystem::System)m_SystemSelectChooser->GetCurrentSelection();

	ev.Skip();
}

void SystemSelectFrame::OnFinishButtonClick(wxEvent& ev)
{
	m_NotificationCallback(m_NotificationCallbackParam);

	ev.Skip();
}
