#pragma once

#include <wx/wx.h>
#include <memory>
#include <string>

#include "VRSystem.h"
#include "ConfigParser.h"
#include "ScoundrelUIController.h"
#include "Theme.h"
#include "ConfigParser.h"
#include "GetDLLPath.h"
#include "Scoundrel.h"

namespace UI {
	extern std::unique_ptr<ScoundrelUIController> m_Scoundrel;
}

class AnalyzerFrame : public wxFrame
{
public:
	AnalyzerFrame();
	~AnalyzerFrame();

private:
	std::unique_ptr<wxPanel> m_BackgroundPanel;
	std::unique_ptr<wxButton> m_ResetButton;
	std::unique_ptr<wxButton> m_UnbindPlayerButton;

	void Reset();
	void OnResetButtonClick(wxEvent& ev);
	void UnbindPlayer(wxEvent& ev);
};

