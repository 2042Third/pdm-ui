#pragma once
//#include "cApp.h"
#include <wx/artprov.h>
#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif
//#include "pdmFile.h"
//#include "cApp.h"
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


class Tree_Data : public wxTreeItemData
{
public:
    Tree_Data(const wxString& desc) : m_desc(desc) { }

    void ShowInfo(wxTreeCtrl *tree);
    wxString const& GetDesc() const { return m_desc; }

private:
    wxString m_desc;
};

class Tree_Ctrl : public wxTreeCtrl
{
public:
	enum{
		Tree_Ctrl_Icon_Files,
		Tree_Ctrl_Icon_Folder
	};

	Tree_Ctrl(){m_alternateImages = false; m_alternateStates = false;}
	Tree_Ctrl (wxWindow * parent, const wxWindowID id,
               const wxPoint& pos, const wxSize& size,
               long style);
	~Tree_Ctrl(){}


    void OnItemExpanded(wxTreeEvent& event);
    void OnItemExpanding(wxTreeEvent& event);
    void OnItemCollapsed(wxTreeEvent& event);
    void OnItemCollapsing(wxTreeEvent& event);

	void SetAlternateImages(bool show) { m_alternateImages = show; }
    bool AlternateImages() const { return m_alternateImages; }

    void SetAlternateStates(bool show) { m_alternateStates = show; }
    bool AlternateStates() const { return m_alternateStates; }

private:
	void AddTestItemsToTree(size_t numChildren,
                                    size_t depth);
	void AddItemsRecursively(const wxTreeItemId& idParent,
                                     size_t numChildren,
                                     size_t depth,
                                     size_t folder);
	void LogEvent(const wxString& name, const wxTreeEvent& event);
	wxDECLARE_DYNAMIC_CLASS(Tree_Ctrl);
    wxDECLARE_EVENT_TABLE();
	
	bool         m_alternateImages;
    bool         m_alternateStates;
};

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
	wxPanel* pane_files = nullptr;
	wxRichTextCtrl* pane_usrspc = nullptr;
	wxBoxSizer* pane_sizer = nullptr;
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
	
	
	// Decrypted tree
	void create_dec_tree();
private:
	

	// Decrypted tree control
	void tree_creator(long style);

	Tree_Ctrl * tree_ctrl;
	wxDECLARE_EVENT_TABLE();
	
	//wxRichTextCtrl* m_richTextCtrl;
};

enum{
	Dec_Tree = 1000
};

