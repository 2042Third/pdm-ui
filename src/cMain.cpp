﻿#pragma once
#include "cMain.h"
#include "cApp.h"
#include "Tree_Ctrl.h"
#include <wx/richtext/richtextbuffer.h>
#include <vector>
// Tree added includes
#include "wx/colordlg.h"
#include "wx/numdlg.h"
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include <wx/arrstr.h>
#include <iostream>
#include "wx/artprov.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"
#include "wx/math.h"
#include "wx/mstream.h"
#include "wx/renderer.h"
#include "wx/wupdlock.h"
#include "wx/dnd.h"
//#include <>


//std::vector<wxString> current_files={};
// TESTING ONLY -- Adapted from offical manual


// END TESTING ONLY

#define MENU_LINK(name) EVT_MENU(Tree_##name, MyFrame::On##name)

	wxDECLARE_APP(cApp);
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	//EVT_TEXT_ENTER(10001,OnKeyEnter)
	EVT_UPDATE_UI(wxEVT_DROP_FILES,cMain::cMainOnFile)
//	EVT_DROP_FILES(wxEVT_DROP_FILES,cMain::OnDropFiles)
	EVT_MENU(wxID_SAVE, cMain::stc_save)
	EVT_MENU(wxID_OPEN, cMain::stc_open)
	EVT_MENU(wxID_EXIT, cMain::stc_quit)
	EVT_MENU(wxID_ABOUT,cMain::c_about)
	EVT_CLOSE(cMain::on_close)
wxEND_EVENT_TABLE()




cMain::cMain(wxWindow* parent,
	wxWindowID id,
	const wxString& title,
	const wxPoint& pos ,
	const wxSize& size,
	long style ,
	const wxString& name)
	: wxFrame(nullptr, id, title, wxDefaultPosition, size,style,name)
{


	this->size = size;
	// Initalize
	menu_bar = new wxMenuBar();
	menu_file = new wxMenu();

	file_open = new wxMenuItem(menu_file, wxID_OPEN, wxString(wxT("&Open\tCtrl+o")), wxEmptyString, wxITEM_NORMAL);
	file_save = new wxMenuItem(menu_file, wxID_SAVE, wxString(wxT("&Save")) + wxT('\t') + wxT("Ctrl+s"), wxEmptyString, wxITEM_NORMAL);
	file_save_as = new wxMenuItem(menu_file, wxID_ANY, wxString(wxT("&Save As")), wxEmptyString, wxITEM_NORMAL);
	file_about = new wxMenuItem(menu_file, wxID_ABOUT,wxString(wxT("&About")),wxEmptyString,wxITEM_NORMAL);
	file_quit = new wxMenuItem(menu_file,wxID_EXIT);

	#ifdef __WXMSW__
	file_open->SetBitmaps(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	file_open->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_MENU));
	#endif

	#ifdef __WXMSW__
	file_save->SetBitmaps(wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MENU));
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	file_save->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_MENU));
	#endif

	#ifdef __WXMSW__
	file_save_as->SetBitmaps(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
	#elif (defined( __WXGTK__ ) || defined( __WXOSX__ ))
	file_save_as->SetBitmap(wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS, wxART_MENU));
	#endif
	menu_file->Append(file_open);
	menu_file->Append(file_save);
	menu_file->Append(file_save_as);
	menu_file->AppendSeparator();
	menu_file->Append(file_about);
	menu_file->AppendSeparator();
	menu_file->Append(file_quit);

	// Menu event

	// Menu event

	attr.SetTextColour(*wxWHITE);

	panel = new wxPanel(this);

	panel->SetMinSize(GetBestSize());
	pane_sizer = new wxBoxSizer(4);
	SetMenuBar(menu_bar);
	pane_sizer->SetMinSize(GetBestSize());

	txt = new wxStaticText(panel, wxID_ANY, wxT("Decrypted Files"));

	pane_files = new wxRichTextCtrl(panel, wxEVT_DROP_FILES, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
	this->create_dec_tree();


	pane_files->SetScrollPos(wxVERTICAL,0,0);
	pane_files->SetBasicStyle(attr);
	pane_usrspc = new wxRichTextCtrl(panel, 103, "");
	pane_usrspc->SetBasicStyle(attr);
	pane_files_sizer = new wxBoxSizer(wxVERTICAL);

	passwd_sizer = new wxBoxSizer( wxHORIZONTAL ) ;
	pswd_text= new wxStaticText(panel, wxID_ANY, wxT("Password"));
	usr_enter = new wxTextCtrl(panel,wxEVT_TEXT_ENTER, wxEmptyString,
                             wxDefaultPosition,  wxDefaultSize,
                             wxTE_PASSWORD,  wxDefaultValidator,
                             "密码");
	passwd_sizer->Add(pswd_text, 1,wxALL | wxEXPAND, 0);
	passwd_sizer->Add(usr_enter,1,wxALL | wxEXPAND,0);
	pane_files_sizer->Add(tree_ctrl,2, wxALL | wxEXPAND, size_border_hor);
	pane_files_sizer->Add(passwd_sizer,-1,wxALL|wxALIGN_LEFT,size_border_hor);
	pane_files_sizer->Add(pane_files,3, wxALL | wxEXPAND, size_border_hor);

	pane_sizer->Add(pane_files_sizer, 2, wxALL | wxEXPAND, size_border_hor);
	pane_sizer->Add(pane_usrspc, 3, wxALL | wxEXPAND, size_border_ver);
	panel->SetSizer(pane_sizer);
	d_target =new Tree_Ctrl::DnDFile(pane_files);
	pane_files->SetDropTarget(d_target);
	maintain_theme();

	pane_files->WriteText(_T("请将需要加密的文件拖入此窗口\n"));
	// Panel Text
	tree_ctrl->set_d_target(d_target);
	menu_bar->Append(menu_file, wxT("&File"));

	this->SetMenuBar(menu_bar);
	this->Centre(wxBOTH);
	this->SetFocus();

}

void cMain::create_dec_tree(){
	long style = wxTR_DEFAULT_STYLE |
#ifndef NO_VARIABLE_HEIGHT
                 wxTR_HAS_VARIABLE_ROW_HEIGHT |
#endif
                 wxTR_EDIT_LABELS | wxTR_HIDE_ROOT | wxTR_HAS_BUTTONS;

    tree_creator(style | wxSIMPLE_BORDER);


}

/*
	Creates a tree with style.

*/
void cMain::tree_creator(long style){
	tree_ctrl = new Tree_Ctrl( panel ,Dec_Tree,
                                wxDefaultPosition, wxDefaultSize,
                                style);

	// GetMenuBar()->Enable(TreeTest_SelectRoot, !(style & wxTR_HIDE_ROOT));

    this->Resize();
	
}



void cMain::Resize()
{
    wxSize size = GetClientSize();
    tree_ctrl->SetSize(0, 0, size.x, size.y
// #if wxUSE_LOG
//                                             *2/3);
//     txt->SetSize(0, 2*size.y/3, size.x, size.y/3
// #endif
    );
}



void cMain::c_about(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("pdm\n"
//                 "Personal Data Management, encrypted data managemenent \n Yi Yang",
                 "个人数据管理, 加密系统。 \n \t由杨译个人开发，所加密数据由用户所有。如密码丢失，其pdm数据将永久丢失。",
                 "2021",
                 wxOK | wxICON_INFORMATION, this);
}

void cMain::stc_quit(wxCommandEvent& WXUNUSED(event)) {

	bool veto = Close();
}

void cMain::on_close(wxCloseEvent& event){
	if (event.CanVeto()) {
	    int answer = wxMessageBox(_T("退出!"), _T("请确认加密文件已保存"),wxYES_NO| wxCANCEL,panel);
		this->SetFocus();
	    if (answer != wxYES) {
			event.Veto();
			return;
		}
	}
	Destroy();
}



void cMain::maintain_theme(){
	attr.SetTextColour(wxTEXT_ATTR_TEXT_COLOUR);
	// Color Settings
	panel->SetBackgroundColour(wxTheColourDatabase->Find("DIM GREY"));
	panel->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
	txt->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
	pswd_text->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));

	tree_ctrl->SetBackgroundColour(wxTheColourDatabase->Find("DARK GREY"));
	usr_enter->SetBackgroundColour(wxTheColourDatabase->Find("DARK GREY"));
	usr_enter->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
	tree_ctrl->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
	pane_files->SetBackgroundColour(wxTheColourDatabase->Find("DARK GREY"));

	pane_usrspc->SetBackgroundColour(wxTheColourDatabase->Find("DARK GREY"));

	pane_usrspc->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));

}
void cMain::cMainOnFile(wxUpdateUIEvent & event) {
//    std::cout<<"on File \n"<<std::endl;
    if(d_target->has_update_files) tree_ctrl->update_current();
    d_target->has_update_files=0;
}

void cMain::stc_open(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog* OpenDialog = new wxFileDialog(
		this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
		_("*"),
			wxFD_OPEN, wxDefaultPosition, wxSize(300,500));

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		CurrentDocPath = OpenDialog->GetPath();
		std::cout << "doc dir: " << CurrentDocPath << std::endl;
		wxMessageBox(CurrentDocPath, CurrentDocPath, wxOK);
//		maintain_theme();
		pane_usrspc->LoadFile(CurrentDocPath); //Opens that file
//		maintain_theme();
//		OnFont();

	}
}

void cMain::OnFont()
{
	attr.SetTextColour(*wxWHITE);
	pane_usrspc->SetBasicStyle( attr);
	pane_files->SetBasicStyle(attr);
}

void cMain::stc_save(wxCommandEvent& WXUNUSED(event)) {
    wxRichTextBuffer bff = pane_usrspc->GetBuffer();
    wxMemoryOutputStream stream;

    bff.SaveFile(stream,wxRICHTEXT_TYPE_TEXT);
    char* data = new char[stream.GetSize() + 1];
    const char* pswd_data = usr_enter->GetValue().mb_str(wxConvUTF8).data();
    stream.CopyTo(data, stream.GetSize());
    data[stream.GetSize()]='\0';
    std::cout<<"Captured text of size "<<stream.GetSize()<<": "<<data<<std::endl;
    std::cout<<"Password: "<<pswd_data<<std::endl;
    tree_ctrl->d_target->m_pOwner->WriteText(data);

}

cMain::~cMain()
{
}

// ADDED FROM WXWIDGETS MANUAL, Event handle 

