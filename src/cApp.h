#pragma once
#include "wx/wxprec.h"

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif
// wxIMPLEMENT_APP(cApp);
//#if COMPILING_DLL
//#define DLLEXPORT __declspec(dllexport)
//#else
//#define DLLEXPORT __declspec(dllimport)
//#endif

 

class cApp: public wxApp
{
public:
	// cApp();
	cApp() { m_showImages = true; m_showStates = true; m_showButtons = false; }
	~cApp();

    bool OnInit() wxOVERRIDE;

    void SetShowImages(bool show) { m_showImages = show; }
    bool ShowImages() const { return m_showImages; }

    void SetShowStates(bool show) { m_showStates = show; }
    bool ShowStates() const { return m_showStates; }

    void SetShowButtons(bool show) { m_showButtons = show; }
    bool ShowButtons() const { return m_showButtons; }
    wxFrame * panel_main;
private:
    bool m_showImages, m_showStates, m_showButtons;

// public:
	// virtual bool OnInit();
};


