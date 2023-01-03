#pragma once

#include "UI/Theme.h"

////static const wxColour PANEL_BACKGROUND_COLOR = wxColour(26, 26, 26); // Normal background
//static wxColour PANEL_BACKGROUND_COLOR = wxColour(31, 34, 38); // Cindy background
//static wxColour UI_BACKGROUND_COLOR = wxColour(35, 35, 35);
////static const wxColour UI_FOREGROUND_COLOR = wxColour(255, 255, 255); // Full white foreground
//static wxColour UI_FOREGROUND_COLOR = wxColour(199, 115, 175); // Cindy foreground color

static wxColour s_PanelBackgroundColor;
static wxColour s_UiBackgroundColor;
static wxColour s_UiForegroundColor;

namespace GuiTheme {
	void InitializeTheme(wxColour panel_background, wxColour ui_background, wxColour ui_foreground)
	{
		s_PanelBackgroundColor = panel_background;
		s_UiBackgroundColor = ui_background;
		s_UiForegroundColor = ui_foreground;
	}

	wxColour GetPanelBackgroundColor()
	{
		return s_PanelBackgroundColor;
	}

	wxColour GetUiBackgroundColor()
	{
		return s_UiBackgroundColor;
	}

	wxColour GetUiForegroundColor()
	{
		return s_UiForegroundColor;
	}
}

