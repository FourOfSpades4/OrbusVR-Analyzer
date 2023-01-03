#pragma once
#include <wx/wx.h>
#include <memory>
#include <string>

#include "VRSystem.h"
#include "ConfigParser.h"

class AnalyzerFrame : public wxFrame
{
public:
	AnalyzerFrame();
	~AnalyzerFrame();

private:
	std::unique_ptr<wxPanel> m_BackgroundPanel;
	
	std::unique_ptr<wxStaticText> m_SystemStatusText;
	std::unique_ptr<wxButton> m_ApplyChangesButton;
};

