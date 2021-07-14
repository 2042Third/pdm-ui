#pragma once

#include "wx/wxprec.h"
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/dirctrl.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/filepicker.h>
#include <wx/sizer.h>
#include <wx/dialog.h>

class pdmFile : public wxDialog
{
private:

	wxGenericDirCtrl* m_genericDirCtrl3;
	wxFilePickerCtrl* m_filePicker2;

public:

	pdmFile(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize(306, 351), long style = wxCAPTION | wxCLOSE_BOX | wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxSTAY_ON_TOP);
	~pdmFile();

};

