#include <format>
#include "AnalyzerFrame.h"


std::unique_ptr<ScoundrelUIController> UI::m_Scoundrel;

AnalyzerFrame::AnalyzerFrame() : wxFrame(nullptr, wxID_ANY, "OrbusVR Class Analyzer", wxDefaultPosition, wxSize(1200, 640))
{
	// U.I Element creation
	m_BackgroundPanel = std::make_unique<wxPanel>(this);
	m_BackgroundPanel->SetBackgroundColour(GuiTheme::GetPanelBackgroundColor());

    m_ResetButton = std::make_unique<wxButton>(m_BackgroundPanel.get(), wxID_ANY, "Reset", wxPoint(5, 5), wxSize(120, 35));
	CONFIGURE_TEXT_THEME(m_ResetButton);
	m_ResetButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &AnalyzerFrame::OnResetButtonClick, this);

	UI::m_Scoundrel = std::make_unique<ScoundrelUIController>(m_BackgroundPanel.get(), 5, 45, (void*)this);
}

AnalyzerFrame::~AnalyzerFrame()
{
}

void AnalyzerFrame::Reset()
{
	UI::m_Scoundrel->Reset();
}

void AnalyzerFrame::OnResetButtonClick(wxEvent& ev)
{
	Reset();
	ev.Skip();
}

void AnalyzerFrame::UnbindPlayer(wxEvent& ev)
{
	Reset();
	ev.Skip();
}