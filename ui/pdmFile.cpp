#include "pdmFile.h"
pdmFile::pdmFile(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style) : wxDialog(parent, id, title, pos, size, style)
{
	this->SetSizeHints(wxDefaultSize, wxDefaultSize);

	wxBoxSizer* bSizer2;
	bSizer2 = new wxBoxSizer(wxVERTICAL);

	m_genericDirCtrl3 = new wxGenericDirCtrl(this, wxID_FILE, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxDIRCTRL_3D_INTERNAL | wxDIRCTRL_SHOW_FILTERS | wxSUNKEN_BORDER, wxEmptyString, 0);

	m_genericDirCtrl3->ShowHidden(true);
	bSizer2->Add(m_genericDirCtrl3, 1, wxEXPAND | wxALL, 5);

	m_filePicker2 = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString, wxT("Select a file"), wxT("*.*"), wxDefaultPosition, wxDefaultSize, wxFLP_CHANGE_DIR | wxFLP_DEFAULT_STYLE | wxFLP_FILE_MUST_EXIST | wxFLP_OPEN | wxFLP_OVERWRITE_PROMPT);
	bSizer2->Add(m_filePicker2, 0, wxALL, 5);


	this->SetSizer(bSizer2);
	this->Layout();

	this->Centre(wxBOTH);
}

pdmFile::~pdmFile()
{
}