﻿#pragma once
#include "cMain.h"
#include "cApp.h"
#include "Tree_Ctrl.h"
#include <wx/richtext/richtextbuffer.h>
#include <vector>
#include "wx/numdlg.h"
#include <wx/dir.h>
#include <wx/arrstr.h>
#include <iostream>
#include "wx/artprov.h"
#include "wx/image.h"
#include "wx/treectrl.h"
#include "wx/math.h"
#include "wx/mstream.h"
#include "wx/wupdlock.h"
#include <wx/tokenzr.h>
#include "cc20_multi.cpp"
#include "id.h"


//std::vector<wxString> current_files={};
// TESTING ONLY -- Adapted from offical manual


// END TESTING ONLY

#define MENU_LINK(name) EVT_MENU(Tree_##name, MyFrame::On##name)

	wxDECLARE_APP(cApp);
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	EVT_UPDATE_UI(wxEVT_DROP_FILES,cMain::cMainOnFile)
  EVT_MENU(wxID_SAVE, cMain::stc_save)
  EVT_MENU((wxStandardID)window::id::FOCUS_PSWD, cMain::stc_pswd_focus)
  EVT_MENU((wxStandardID)window::id::FOCUS_USRSPC, cMain::stc_usrspc_focus)
  EVT_MENU(wxID_OPEN, cMain::stc_open)
  EVT_MENU(wxID_NEW, cMain::stc_new)
	EVT_MENU(wxID_EXIT, cMain::stc_quit)
	EVT_MENU((wxStandardID)window::id::PDM_ABOUT,cMain::c_about)
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
	menu_file = new wxMenu;
  menu_view = new wxMenu;
  menu_pdm  = new wxMenu;
  view_pswd_focus=new wxMenuItem(menu_view,window::id::FOCUS_PSWD, wxString(wxT("To &Password\tCtrl+p")), "Set Focus to password entry without using mouse", wxITEM_NORMAL);
  view_usrspc_focus=new wxMenuItem(menu_view,window::id::FOCUS_USRSPC, wxString(wxT("To &Editor\tCtrl+l")), "Set Focus to editor without using mouse", wxITEM_NORMAL);
	file_open = new wxMenuItem(menu_file, wxID_OPEN, wxString(wxT("&Open\tCtrl+o")), wxEmptyString, wxITEM_NORMAL);
	file_save = new wxMenuItem(menu_file, wxID_SAVE, wxString(wxT("&Save")) + wxT('\t') + wxT("Ctrl+s"), wxEmptyString, wxITEM_NORMAL);
	file_save_as = new wxMenuItem(menu_file, wxID_ANY, wxString(wxT("&Save As")), wxEmptyString, wxITEM_NORMAL);
	file_new = new wxMenuItem(menu_file, wxID_NEW, wxString(wxT("&New\tCtrl+n")), wxEmptyString, wxITEM_NORMAL);
	pdm_about = new wxMenuItem(menu_file, window::id::PDM_ABOUT,wxString(wxT("&About")),"About PDM",wxITEM_NORMAL);
	file_quit = new wxMenuItem(menu_file,wxID_EXIT);

	menu_file->Append(file_open);
	menu_file->Append(file_new);
	menu_file->Append(file_save);
	menu_file->AppendSeparator();
	menu_file->Append(file_save_as);
	menu_pdm->Append(pdm_about);
	menu_file->Append(file_quit);
	menu_view->Append(view_pswd_focus);
	menu_view->Append(view_usrspc_focus);
	// Menu event

	// Menu event

	attr.SetTextColour(*wxWHITE);

	panel = new wxPanel(this);

	panel->SetMinSize(GetBestSize());
	pane_sizer = new wxBoxSizer(4);
	SetMenuBar(menu_bar);
	pane_sizer->SetMinSize(GetBestSize());

	txt = new wxStaticText(panel, wxID_ANY, wxT("Encrypted Files"));

	pane_files = new wxRichTextCtrl(panel, wxEVT_DROP_FILES, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
	this->create_dec_tree();


	pane_files->SetScrollPos(wxVERTICAL,0,0);
	pane_files->SetBasicStyle(attr);
	pane_usrspc = new wxRichTextCtrl(panel, 103, "");
	pane_usrspc->SetBasicStyle(attr);
	pane_files_sizer = new wxBoxSizer(wxVERTICAL);
	pane_usrspc_sizer = new wxBoxSizer(wxVERTICAL);
	file_text = new wxStaticText(panel, wxID_ANY, "New_File");
	passwd_sizer = new wxBoxSizer( wxHORIZONTAL ) ;
	pswd_text= new wxStaticText(panel, wxID_ANY, _("Password"));
	usr_enter = new wxTextCtrl(panel,wxEVT_TEXT_ENTER, wxEmptyString,
                             wxDefaultPosition,  wxDefaultSize,
                             wxTE_PASSWORD,  wxDefaultValidator,
                             "密码");
	passwd_sizer->Add(pswd_text, 1,wxALL | wxEXPAND, 0);
	passwd_sizer->Add(usr_enter,1,wxALL | wxEXPAND,0);
	pane_files_sizer->Add(txt,-1, wxEXPAND, size_border_hor);
	pane_files_sizer->Add(tree_ctrl,2, wxALL | wxEXPAND, size_border_hor);
	pane_files_sizer->Add(passwd_sizer,-1,wxALL|wxALIGN_LEFT,size_border_hor);
	pane_files_sizer->Add(pane_files,3, wxALL | wxEXPAND, size_border_hor);
	pane_usrspc_sizer->Add(file_text);
	pane_usrspc_sizer->Add(pane_usrspc,-1, wxALL|wxEXPAND, size_border_hor);

	pane_sizer->Add(pane_files_sizer, 2, wxALL | wxEXPAND, size_border_hor);
	pane_sizer->Add(pane_usrspc_sizer, 3, wxALL | wxEXPAND, size_border_ver);
	panel->SetSizer(pane_sizer);

	d_target =new Tree_Ctrl::DnDFile(pane_files);
	pane_files->SetDropTarget(d_target);
	maintain_theme();

	pane_files->WriteText(_T("请将需要加密的文件拖入此窗口\n"));
	// Panel Text
	tree_ctrl->set_d_target(d_target);
	menu_bar->Append(menu_pdm, wxT("&PDM"));
	menu_bar->Append(menu_file, wxT("&File"));
	menu_bar->Append(menu_view, wxT("&View"));
  tree_ctrl->set_parent(this);
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
    this->Resize();
	
}



void cMain::Resize()
{
    wxSize size = GetClientSize();
    tree_ctrl->SetSize(0, 0, size.x, size.y
    );
}

void cMain::open_enc_file(wxString infile) {
//  wxString file_content;
  char* data ;
  char* outstr;

  buffer = usr_enter->GetValue().ToUTF8();
  pswd_data = buffer.data();
  if (usr_enter->GetValue().size()==0){
    wxMessageBox("Need password", "Enter a password first, then save.", wxOK);
    usr_enter->SetFocus();
    return;
  }
  wxFile file1( infile+".pdm", wxFile::read );
  size_t file_len = file1.Length();

  if(data!= nullptr){
    delete[] data;
  }
  data = new char[file_len+1];
  file1.Read(data,file_len);
  file1.Close();
  std::printf("OF file %s \nsize: %lld\n",data,file_len);

  if(outstr!= nullptr){
    delete[] outstr;
  }
  outstr=new char[file_len+1];

  CurrentFileName=pton(infile);
  CurrentDocPath=infile;
  CurrentFileSize=file_len;
  CurrentFileNameEnc=CurrentFileName+".pdm";
  CurrentDocPathEnc=CurrentDocPath+".pdm";
  tree_ctrl->d_target->m_pOwner->WriteText(_("Open: "));
  tree_ctrl->d_target->m_pOwner->WriteText(infile+"\n");
  file_text->SetLabel(CurrentFileName);

  DE = 1;
  cmd_enc((uint8_t*)data,(size_t)CurrentFileSize,(uint8_t*)outstr,((std::string)pswd_data));
  pane_usrspc->Clear();
  pane_usrspc->WriteText(outstr);
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

void cMain::stc_new(wxCommandEvent& event) {
  CurrentDocPath="";
  CurrentFileName="";
  CurrentFileSize=0;
  file_text->SetLabel(_("New_File"));
  pane_usrspc->Clear();
  stc_usrspc_focus(event);

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
	file_text->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
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
	  CurrentFileName= OpenDialog->GetFilename();
		CurrentDocPath = OpenDialog->GetPath();
		std::cout << "doc dir: " << CurrentDocPath << std::endl;
		pane_usrspc->LoadFile(CurrentDocPath,wxFILE); //Opens that file
    file_text->SetLabel(CurrentFileName);
	}
}

void cMain::OnFont()
{
	attr.SetTextColour(*wxWHITE);
	pane_usrspc->SetBasicStyle( attr);
	pane_files->SetBasicStyle(attr);
}

void cMain::stc_pswd_focus(wxCommandEvent& event) {
  usr_enter->SetFocus();
}
void cMain::stc_usrspc_focus(wxCommandEvent& event) {
  pane_usrspc->SetFocus();
}

void cMain::stc_save(wxCommandEvent& event) {
  wxRichTextBuffer bff = pane_usrspc->GetBuffer();
  wxMemoryOutputStream stream;
  bff.SaveFile(stream,wxRICHTEXT_TYPE_TEXT);
  data = new char[stream.GetSize() + 1];

  buffer = usr_enter->GetValue().ToUTF8();
  pswd_data = buffer.data();
  stream.CopyTo(data, stream.GetSize());
  data[stream.GetSize()]='\0';
  if (usr_enter->GetValue().size()==0){
    wxMessageBox("Need password", "Enter a password first, then save.", wxOK);
    stc_pswd_focus(event);
    return;
  }
  std::cout<<"Captured text of size "<<stream.GetSize()<<": "<<data<<std::endl;
  std::cout<<"Password: "<<pswd_data<<std::endl;
  CurrentFileSize=stream.GetSize()+12;
  if(outstr!= nullptr){
    delete[] outstr;
  }
  outstr = new char[stream.GetSize()+13];
  DE = 0;
  cmd_enc((uint8_t*)data,(size_t)stream.GetSize(),(uint8_t*)outstr,((std::string)pswd_data));

  if (CurrentFileName.size()!=0){
    std::cout << "doc dir: " << CurrentDocPath << std::endl;

    CurrentFileNameEnc=CurrentFileName+".pdm";
    CurrentDocPathEnc=CurrentDocPath+".pdm";

    wxFile file( CurrentDocPathEnc, wxFile::write );
    if( file.IsOpened() )
    {
      file.Write(outstr,CurrentFileSize);
      std::cout<<file.Length()<<std::endl;
      file.Close();
    }
    tree_ctrl->d_target->m_pOwner->WriteText(_("Successfully saved "));
    tree_ctrl->d_target->m_pOwner->WriteText(CurrentFileName+" as "+CurrentFileNameEnc+"\n");
    file_text->SetLabel(CurrentFileName);
    tree_ctrl->addFileToTree(CurrentDocPath);
    return;
  }

  wxFileDialog* OpenDialog = new wxFileDialog(
      this, _("Save file"), wxEmptyString, CurrentFileName,
      _("*"),
      wxFD_SAVE, wxDefaultPosition, wxSize(300,500));
  if (OpenDialog->ShowModal() == wxID_OK ) // if the user click "Open" instead of "cancel"
    {
    CurrentFileName= OpenDialog->GetFilename();
    CurrentDocPath = OpenDialog->GetPath();
    std::cout << "doc dir: " << CurrentDocPath << std::endl;

    CurrentFileNameEnc=CurrentFileName+".pdm";
    CurrentDocPathEnc=CurrentDocPath+".pdm";

    wxFile file( CurrentDocPathEnc, wxFile::write );
    if( file.IsOpened() )
    {
      file.Write(outstr,CurrentFileSize);
      file.Close();
    }
    tree_ctrl->d_target->m_pOwner->WriteText(_("Successfully saved "));
    tree_ctrl->d_target->m_pOwner->WriteText(CurrentFileName+" as "+CurrentFileNameEnc+"\n");
    file_text->SetLabel(CurrentFileName);
    tree_ctrl->addFileToTree(CurrentDocPath);
    }


}
wxString cMain::pton(wxString& a){
  return wxFileNameFromPath(a);
}
cMain::~cMain()
{
  if(outstr!= nullptr){
    delete[] outstr;
  }
  if(data!= nullptr){
    delete[] data;
  }
}

// ADDED FROM WXWIDGETS MANUAL, Event handle 

