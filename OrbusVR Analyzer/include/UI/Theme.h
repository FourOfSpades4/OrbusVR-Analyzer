#pragma once

#include <wx/wx.h>

namespace GuiTheme {
	void InitializeTheme(wxColour panel_background, wxColour ui_background, wxColour ui_foreground);

	wxColour GetPanelBackgroundColor();
	wxColour GetUiBackgroundColor();
	wxColour GetUiForegroundColor();
}


#define CONFIGURE_UI_THEME(ui_element) \
(ui_element)->SetBackgroundColour(GuiTheme::GetUiBackgroundColor()); \
(ui_element)->SetForegroundColour(GuiTheme::GetUiForegroundColor())

#define CONFIGURE_UI_THEME_CUSTOM(ui_element, background, foreground) \
(ui_element)->SetBackgroundColour((background)); \
(ui_element)->SetForegroundColour((foreground))

#define CONFIGURE_TEXT_THEME(ui_element) \
(ui_element)->SetBackgroundColour(GuiTheme::GetPanelBackgroundColor()); \
(ui_element)->SetForegroundColour(GuiTheme::GetUiForegroundColor())