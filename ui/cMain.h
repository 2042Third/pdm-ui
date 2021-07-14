#pragma once
//#include "cApp.h"
#include <wx/artprov.h>
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif
//#include "pdmFile.h"
//#include "cApp.h"
#include "Tree_Ctrl.h"
#include <vector>
#include "cc20_multi.h"
#include <iostream>
#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/menu.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/frame.h>
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
#include "wx/richtext/richtextctrl.h"
//#include <wx/richtext/richtextbuffer.h>
// #define wxUSE_DRAG_AND_DROP = 1




class cMain : public wxFrame
{

public:
	//cMain(const wxString& title, const wxSize& size);
	cMain(wxWindow* parent,
		wxWindowID id,
		const wxString& title,
		const wxPoint& pos = wxDefaultPosition,
		const wxSize& size = wxDefaultSize,
		long style = wxDEFAULT_FRAME_STYLE,
		const wxString& name = wxASCII_STR(wxFrameNameStr));
	~cMain();

	// Encryption
	wxTextCtrl* key_entry = nullptr;
	// File 
	wxTextCtrl* MainEditBox;
	wxString CurrentDocPath;
	// Var
	wxSize size = wxGetDisplaySize();
	wxRichTextAttr attr;


	// Menu&Bar
	wxMenuBar* menu_bar;
	wxMenu* menu_file;
	wxMenuItem* file_open;
	wxMenuItem* file_save;
	wxMenuItem* file_about;
	wxMenuItem* file_save_as;
	wxMenuItem* file_quit;

	// Window handle
	wxPanel* panel = nullptr;
	wxStaticBox* pane_auth = nullptr;
	wxRichTextCtrl* pane_files = nullptr;
	wxRichTextCtrl* pane_usrspc = nullptr;
	wxBoxSizer* pane_sizer = nullptr;
	wxBoxSizer* pane_files_sizer = nullptr;
	wxStaticText* txt = nullptr;

	 
	// Border Val
	int size_border_ver = 7;
	int size_border_hor = 20;


	// Event handle
	void stc_quit(wxCommandEvent& event);
	void stc_open(wxCommandEvent& event);
	void stc_save(wxCommandEvent& event);
	void on_close(wxCloseEvent& event);
	void c_about(wxCommandEvent& event);
	//void OnSaveAs(wxCommandEvent& event);
	void OneKeyEnter(wxCommandEvent& event);
	void OnFont();
	void Resize();
	void cMainOnFile(wxUpdateUIEvent &event);
	
	// Decrypted tree
	void create_dec_tree();
private:
    wxTreeItemId root_man;
    long style = wxTR_DEFAULT_STYLE |
                      #ifndef NO_VARIABLE_HEIGHT
                      wxTR_HAS_VARIABLE_ROW_HEIGHT |
                      #endif
                      wxTR_EDIT_LABELS;
	wxString fileText = _T("请将需要加密的文件拖入此窗口");
	// void OnDropFiles(wxDropFilesEvent& event);
	void maintain_theme();
    Tree_Ctrl::DnDFile *d_target;

	// Decrypted tree control
	void tree_creator(long style);

	Tree_Ctrl * tree_ctrl;
	wxDECLARE_EVENT_TABLE();
	
	//wxRichTextCtrl* m_richTextCtrl;
};




