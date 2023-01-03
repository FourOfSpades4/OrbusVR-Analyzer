#include <filesystem>
#include <fstream>
#include <iostream>

#include "AnalyzerApp.h"
#include "Theme.h"
#include "GetDLLPath.h"
#include "ConfigParser.h"
#include "Hooks.h"



wxIMPLEMENT_APP_NO_MAIN(AnalyzerApp);

AnalyzerApp::AnalyzerApp()
{
}

AnalyzerApp::~AnalyzerApp()
{
}

bool AnalyzerApp::OnInit()
{
	SetExitOnFrameDelete(false);
	ParseThemeFileAndUpdate();

	m_SystemSelectFrame = new SystemSelectFrame((NotificationCallback_t)&AnalyzerApp::OnSystemSelectNotification, this);
	m_SystemSelectFrame->Show();

	// This is very sketchy, and crash prone. But we actually need to construct this AFTER the system is selected
	// otherwise it won't read the up to date system.
	m_OrbusCheatsFrame = nullptr;
	

	return true;
}

void AnalyzerApp::OnSystemSelectNotification(AnalyzerApp* self)
{
	VrSystem::System selectedSystem = self->m_SystemSelectFrame->GetSelectedSystem();

	if (selectedSystem != VrSystem::None) {
		Hooks::Init(selectedSystem);

		self->m_OrbusCheatsFrame = new AnalyzerFrame();
		self->m_OrbusCheatsFrame->Show();

		self->m_SystemSelectFrame->Close();
	}
	else {
		wxMessageBox("Please select which system you are using before continuing", "Please select a system", wxICON_INFORMATION | wxOK);
	}
}

void AnalyzerApp::ParseThemeFileAndUpdate()
{
	std::string configPath = DllPath::MakeRelative("theme.ini");

	if (std::filesystem::exists(configPath) == false) {
		std::cout << "theme.ini file not found. Creating.\n";

		std::ofstream output_file(configPath);

		std::string_view defaultConfig =
			"[panel_background]\n"
			"red = 31\n"
			"green = 34\n"
			"blue = 38\n"


			"\n[ui_background]\n"
			"red = 35\n"
			"green = 35\n"
			"blue = 35\n"

			"\n[ui_foreground]\n"
			"red = 199\n"
			"green = 115\n"
			"blue = 175\n";

		output_file << defaultConfig;

		output_file.close();
	}

	auto expectedConfigData = ConfigData::ParseFile(DllPath::MakeRelative("theme.ini"));

	if (expectedConfigData.has_value() == false) {
		ConfigParserError errorCode = expectedConfigData.error();

		switch (errorCode) {
		case ConfigParserError::BadFile:
			std::cout << "Could not open theme.ini file for reading.\n";
			return;
		case ConfigParserError::BadSection:
			std::cout << "Error parsing theme.ini: Bad section declaration. Try deleting the config file, then reinjecting the DLL.\n";
			return;
		case ConfigParserError::BadProperty:
			std::cout << "Error parsing theme.ini: Bad property definition. Try deleting the config file, then reinjecting the DLL.\n";
			return;
		}
	}

	ConfigData configData = std::move(expectedConfigData.value());

	bool panelBackgroundFail = false;
	bool uiBackgroundFail = false;
	bool uiForegroundFail = false;

	// This is some Rust in C++ gaming code because std::optional & std::expected >>>> exceptions
	int64_t panel_background_red = configData.Geti64("panel_background", "red")
		.or_else([&]() {panelBackgroundFail = true; return std::optional<int64_t>(31); }).value();
	int64_t panel_background_green = configData.Geti64("panel_background", "green")
		.or_else([&]() {panelBackgroundFail = true; return std::optional<int64_t>(34); }).value();
	int64_t panel_background_blue = configData.Geti64("panel_background", "blue")
		.or_else([&]() {panelBackgroundFail = true; return std::optional<int64_t>(38); }).value();

	int64_t ui_background_red = configData.Geti64("ui_background", "red")
		.or_else([&]() {uiBackgroundFail = true; return std::optional<int64_t>(35); }).value();
	int64_t ui_background_green = configData.Geti64("ui_background", "green")
		.or_else([&]() {uiBackgroundFail = true; return std::optional<int64_t>(35); }).value();
	int64_t ui_background_blue = configData.Geti64("ui_background", "blue")
		.or_else([&]() {uiBackgroundFail = true; return std::optional<int64_t>(35); }).value();

	int64_t ui_foreground_red = configData.Geti64("ui_foreground", "red")
		.or_else([&]() {uiForegroundFail = true; return std::optional<int64_t>(199); }).value();
	int64_t ui_foreground_green = configData.Geti64("ui_foreground", "green")
		.or_else([&]() {uiForegroundFail = true; return std::optional<int64_t>(115); }).value();
	int64_t ui_foreground_blue = configData.Geti64("ui_foreground", "blue")
		.or_else([&]() {uiForegroundFail = true; return std::optional<int64_t>(175); }).value();

	// I chose to clamp all the values on a seperate line just to prevent the above lines from getting more out of hand than they already are
	panel_background_red = std::clamp(panel_background_red, (int64_t)0, (int64_t)255);
	panel_background_green = std::clamp(panel_background_green, (int64_t)0, (int64_t)255);
	panel_background_blue = std::clamp(panel_background_blue, (int64_t)0, (int64_t)255);

	ui_background_red = std::clamp(ui_background_red, (int64_t)0, (int64_t)255);
	ui_background_green = std::clamp(ui_background_green, (int64_t)0, (int64_t)255);
	ui_background_blue = std::clamp(ui_background_blue, (int64_t)0, (int64_t)255);

	ui_foreground_red = std::clamp(ui_foreground_red, (int64_t)0, (int64_t)255);
	ui_foreground_green = std::clamp(ui_foreground_green, (int64_t)0, (int64_t)255);
	ui_foreground_blue = std::clamp(ui_foreground_blue, (int64_t)0, (int64_t)255);

	if (panelBackgroundFail) {
		std::cout << "Error parsing one or more panel background color channels. Try deleting the config file, then reinjecting the DLL.\n";
	}

	if (uiBackgroundFail) {
		std::cout << "Error prasing one or more background color channels. Try deleting the config file, then reinjecting the DLL.\n";
	}

	if (uiForegroundFail) {
		std::cout << "Error parsing one or more foreground color channels. Try deleting the config file, then reinjecting the DLL.\n";
	}

	GuiTheme::InitializeTheme(wxColour(panel_background_red, panel_background_green, panel_background_blue),
		wxColour(ui_background_red, ui_background_green, ui_background_blue),
		wxColour(ui_foreground_red, ui_foreground_green, ui_foreground_blue));
}

