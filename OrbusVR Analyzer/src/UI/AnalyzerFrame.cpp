#include <format>

#include "AnalyzerFrame.h"
#include "Theme.h"
#include "ConfigParser.h"
#include "GetDLLPath.h"

AnalyzerFrame::AnalyzerFrame() : wxFrame(nullptr, wxID_ANY, "OrbusVR Class Analyzer", wxDefaultPosition, wxSize(600, 400))
{
	// U.I Element creation
	m_BackgroundPanel = std::make_unique<wxPanel>(this);
	m_BackgroundPanel->SetBackgroundColour(GuiTheme::GetPanelBackgroundColor());
}

AnalyzerFrame::~AnalyzerFrame()
{
}