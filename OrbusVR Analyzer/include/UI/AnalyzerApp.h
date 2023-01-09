#pragma once

#include <wx/wx.h>
#include "AnalyzerFrame.h"
#include "SystemSelectFrame.h"

class AnalyzerApp : public wxApp
{
public:
	AnalyzerApp();
	~AnalyzerApp();

public:
	virtual bool OnInit();

private:
	void ParseThemeFileAndUpdate();


private:
	SystemSelectFrame* m_SystemSelectFrame;
	AnalyzerFrame* m_AnalyzerFrame;
};

