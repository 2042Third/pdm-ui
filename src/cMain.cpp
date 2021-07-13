#pragma once
#include "cMain.h"
#include "cApp.h"
#include <wx/richtext/richtextbuffer.h>

// Tree added includes
#include "wx/colordlg.h"
#include "wx/numdlg.h"
#include <wx/dir.h>
#include <wx/busyinfo.h>
#include <wx/arrstr.h>

#include "wx/artprov.h"
#include "wx/image.h"
#include "wx/imaglist.h"
#include "wx/treectrl.h"
#include "wx/math.h"
#include "wx/renderer.h"
#include "wx/wupdlock.h"
#include "wx/dnd.h"


// TESTING ONLY -- Adapted from offical manual
static const int NUM_CHILDREN_PER_LEVEL = 5;
static const int NUM_LEVELS = 2;

// END TESTING ONLY

#define MENU_LINK(name) EVT_MENU(Tree_##name, MyFrame::On##name)

	wxDECLARE_APP(cApp);
wxBEGIN_EVENT_TABLE(cMain, wxFrame)
	//EVT_TEXT_ENTER(10001,OnKeyEnter)
	
	// EVT_DROP_FILES(wxEVT_DROP_FILES,cMain::OnDropFiles) 
	EVT_MENU(wxID_SAVE, cMain::stc_save)
	EVT_MENU(wxID_OPEN, cMain::stc_open)
	EVT_MENU(wxID_EXIT, cMain::stc_quit)
	EVT_MENU(wxID_ABOUT,cMain::c_about)
	EVT_CLOSE(cMain::on_close)
wxEND_EVENT_TABLE()

wxBEGIN_EVENT_TABLE(Tree_Ctrl, wxTreeCtrl)
	EVT_TREE_ITEM_EXPANDED(Dec_Tree, Tree_Ctrl::OnItemExpanded)
    EVT_TREE_ITEM_EXPANDING(Dec_Tree, Tree_Ctrl::OnItemExpanding)
    EVT_TREE_ITEM_COLLAPSED(Dec_Tree, Tree_Ctrl::OnItemCollapsed)
    EVT_TREE_ITEM_COLLAPSING(Dec_Tree, Tree_Ctrl::OnItemCollapsing)
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
	Bind(wxEVT_MENU, &cMain::stc_save, this, wxID_SAVE);
	Bind(wxEVT_MENU, &cMain::stc_open, this, wxID_OPEN);
	Bind(wxEVT_MENU, &cMain::stc_quit, this, wxID_EXIT);

	// maintain_theme();
	// Panel start
	panel = new wxPanel(this);
	panel->SetMinSize(GetBestSize());
	pane_sizer = new wxBoxSizer(4);
	SetMenuBar(menu_bar);
	pane_sizer->SetMinSize(GetBestSize());

	// maintain_theme();

	txt = new wxStaticText(panel, wxID_ANY, wxT("Decrypted Files"));

	pane_files = new wxRichTextCtrl(panel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
	create_dec_tree();
	pane_usrspc = new wxRichTextCtrl(panel, 103, "");
	pane_files_sizer = new wxBoxSizer(wxVERTICAL);

	maintain_theme();
	
	pane_files_sizer->Add(tree_ctrl,1, wxALL | wxEXPAND, size_border_hor);
	pane_files_sizer->Add(pane_files,1, wxALL | wxEXPAND, size_border_hor);

	pane_sizer->Add(pane_files_sizer, 1, wxALL | wxEXPAND, size_border_hor);
	pane_sizer->Add(pane_usrspc, 2, wxALL | wxEXPAND, size_border_ver);
	panel->SetSizer(pane_sizer);
	// pane_files->DragAcceptFiles(true);
	pane_files->SetDropTarget(new DnDFile(pane_files));
	maintain_theme();
	pane_files->WriteText(_T("请将需要加密的文件拖入此窗口"));
	// Panel Text
	// pane_files->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(cMain::OnDropFiles), NULL, this);

	menu_bar->Append(menu_file, wxT("&File"));

	this->SetMenuBar(menu_bar);
	this->Centre(wxBOTH);

}

void cMain::create_dec_tree(){
	long style = wxTR_DEFAULT_STYLE |
#ifndef NO_VARIABLE_HEIGHT
                 wxTR_HAS_VARIABLE_ROW_HEIGHT |
#endif
                 wxTR_EDIT_LABELS;

    tree_creator(style | wxSIMPLE_BORDER);

    // // as we don't know what wxTR_DEFAULT_STYLE could contain, test for
    // // everything
    // // wxMenuBar *mbar = GetMenuBar();
    // menu_bar->Check(TreeTest_TogHideRoot, (style & wxTR_HIDE_ROOT) != 0);
    // menu_bar->Check(TreeTest_TogEdit, (style & wxTR_EDIT_LABELS) != 0);
    // menu_bar->Check(TreeTest_TogBorder, (style & wxTR_ROW_LINES) != 0);
    // menu_bar->Check(TreeTest_TogFullHighlight, (style & wxTR_FULL_ROW_HIGHLIGHT) != 0);
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

void Tree_Ctrl::AddItemsRecursively(const wxTreeItemId& idParent,
                                     size_t numChildren,
                                     size_t depth,
                                     size_t folder)
{
    if ( depth > 0 )
    {
        bool hasChildren = depth > 1;

        wxString str;
        for ( size_t n = 0; n < numChildren; n++ )
        {
            // at depth 1 elements won't have any more children
            if ( hasChildren )
                str.Printf("%s child %u", "Folder", unsigned(n + 1));
            else
                str.Printf("%s child %u.%u", "File", unsigned(folder), unsigned(n + 1));

            // here we pass to AppendItem() normal and selected item images (we
            // suppose that selected image follows the normal one in the enum)
            int image, imageSel;
            if ( wxGetApp().ShowImages() )
            {
                image = depth == 1 ? Tree_Ctrl_Icon_Files : Tree_Ctrl_Icon_Folder;
                imageSel = image + 1;
            }
            else
            {
                image = imageSel = -1;
            }
            wxTreeItemId id = AppendItem(idParent, str, image, imageSel,
                                         new Tree_Data(str));

            if ( wxGetApp().ShowStates() )
                SetItemState(id, 0);

            // and now we also set the expanded one (only for the folders)
            // if ( hasChildren && wxGetApp().ShowImages() )
            // {
            //     SetItemImage(id, TreeCtrlIcon_FolderOpened,
            //                  wxTreeItemIcon_Expanded);
            // }

            AddItemsRecursively(id, numChildren, depth - 1, n + 1);
        }
    }
    //else: done!
}

void Tree_Ctrl::AddTestItemsToTree(	size_t children_count,
									size_t depth)
{
	int image = wxGetApp().ShowImages() ? Tree_Ctrl::Tree_Ctrl_Icon_Files : -1;
	wxTreeItemId rootId = AddRoot("Root",
                                  image, image,
                                  new Tree_Data("Root item"));

	AddItemsRecursively(rootId, children_count, depth, 0);

	// // set some colours/fonts for testing
    // if ( !HasFlag(wxTR_HIDE_ROOT) )
    //     SetItemFont(rootId, *wxITALIC_FONT);

    // wxTreeItemIdValue cookie;
    // wxTreeItemId id = GetFirstChild(rootId, cookie);
    // SetItemTextColour(id, *wxBLUE);

    // id = GetNextChild(rootId, cookie);
    // if ( id )
    //     id = GetNextChild(rootId, cookie);
    // if ( id )
    // {
    //     SetItemTextColour(id, *wxRED);
    //     SetItemBackgroundColour(id, *wxLIGHT_GREY);
    // }

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

wxIMPLEMENT_DYNAMIC_CLASS(Tree_Ctrl, wxTreeCtrl);

Tree_Ctrl::Tree_Ctrl(wxWindow *parent, const wxWindowID id,
                       const wxPoint& pos, const wxSize& size,
                       long style)
          			: wxTreeCtrl(parent, id, pos, size, style,wxDefaultValidator, "Decrypted Files")
{
    // m_reverseSort = false;

    // CreateImageList();
    // CreateStateImageList();

    // Add some items to the tree
    AddTestItemsToTree(NUM_CHILDREN_PER_LEVEL, NUM_LEVELS);
}

void cMain::c_about(wxCommandEvent& WXUNUSED(event))
{
    wxMessageBox("pdm\n"
                 "Personal Data Management, encrypted data management \n Yi Yang",
                 "2021",
                 wxOK | wxICON_INFORMATION, this);
}

void cMain::stc_quit(wxCommandEvent& WXUNUSED(event)) {

	bool veto = Close();
}

void cMain::on_close(wxCloseEvent& event){
	if (event.CanVeto()) {
		int answer = wxMessageBox(_T("退出!"), _T("请确认加密文件已保存"),wxYES_NO);
		if (answer != wxYES) {

			event.Veto();
			return;
		}
	}

	Destroy();
}

void Tree_Ctrl::OnItemCollapsing(wxTreeEvent& event)
{
    // wxLogMessage("OnItemCollapsing");

    // for testing, prevent the user from collapsing the first child folder
    wxTreeItemId itemId = event.GetItem();
    // if ( IsTestItem(itemId) )
    // {
    //     wxMessageBox("You can't collapse this item.");

    //     event.Veto();
    // }
}

void cMain::maintain_theme(){
	attr.SetTextColour(*wxWHITE);
	// Color Settings
		panel->SetBackgroundColour(wxTheColourDatabase->Find("DARK GREY"));
	panel->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
	txt->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));

	tree_ctrl->SetBackgroundColour(wxTheColourDatabase->Find("DARK GREY"));
	tree_ctrl->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
	if(1){pane_files->SetBackgroundColour(wxTheColourDatabase->Find("DARK GREY"));
	pane_files->SetDefaultStyle(attr);}
	if(1){pane_usrspc->SetBackgroundColour(wxTheColourDatabase->Find("DARK GREY"));
	pane_usrspc->SetDefaultStyle(attr);}
	pane_files->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
	//pane_usrspc->SetForegroundColour(wxTheColourDatabase->Find("WHITE"));
	
	//OnFont();
}

void cMain::stc_open(wxCommandEvent& WXUNUSED(event)) {
	wxFileDialog* OpenDialog = new wxFileDialog(
		this, _("Choose a file to open"), wxEmptyString, wxEmptyString,
		_("Text files (*.txt)|*.txt|C++ Source Files (*.cpp, *.cxx)|*.cpp;*.cxx|C Source files(*.c) | *.c | C header files(*.h) | *.h| All | *"),
			wxFD_OPEN, wxDefaultPosition, wxSize(300,500));

	// Creates a "open file" dialog with 4 file types
	if (OpenDialog->ShowModal() == wxID_OK) // if the user click "Open" instead of "cancel"
	{
		CurrentDocPath = OpenDialog->GetPath();
		std::cout << "doc dir: " << CurrentDocPath << std::endl;
		wxMessageBox(CurrentDocPath, CurrentDocPath, wxOK);
		maintain_theme();
		pane_usrspc->LoadFile(CurrentDocPath); //Opens that file
		maintain_theme();
		OnFont();

	}
}
// void cMain::OnDropFiles(wxDropFilesEvent& event)   
//     {
//         if (event.GetNumberOfFiles() > 0) {

//             wxString* dropped = event.GetFiles();
//             wxASSERT(dropped);

//             wxBusyCursor busyCursor;
//             wxWindowDisabler disabler;    
//             wxBusyInfo busyInfo(_("Adding files, wait please..."));

//             wxString name;
//             wxArrayString files;

//             for (int i = 0; i < event.GetNumberOfFiles(); i++) {
//                 name = dropped[i];
//                 if (wxFileExists(name))
//                     files.push_back(name);
//                 else if (wxDirExists(name))
//                     wxDir::GetAllFiles(name, &files);                                    
//             }

//             wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
//             wxASSERT(textCtrl);
//             textCtrl->Clear();
//             for (size_t i = 0; i < files.size(); i++) {
//                 *textCtrl << files[i] << wxT('\n');
//             }
//         }
//     }

bool DnDFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames)
{
    size_t nFiles = filenames.GetCount();
    wxString str;
    str.Printf( "%d files dropped", (int)nFiles);

    if (m_pOwner != NULL)
    {
    	m_pOwner->WriteText(_T("请将需要加密的文件拖入此窗口"));
        // m_pOwner->Append(str);
        for ( size_t n = 0; n < nFiles; n++ )
    		m_pOwner->WriteText(filenames[n]);
            // m_pOwner->Append(filenames[n]);
    }

    return true;
}

void cMain::OnFont()
{
	wxRichTextRange range;
	if (pane_usrspc->HasSelection())
		range = pane_usrspc->GetSelectionRange();
	else
		range = wxRichTextRange(0, pane_usrspc->GetLastPosition() + 1);

	wxRichTextAttr attr;
	attr.SetTextColour(*wxWHITE);

	pane_usrspc->SetStyleEx(range, attr);
	pane_files->SetStyleEx(range,attr);
}

void cMain::stc_save(wxCommandEvent& WXUNUSED(event)) {

}

cMain::~cMain()
{
}

// ADDED FROM WXWIDGETS MANUAL, Event handle 

void Tree_Ctrl::LogEvent(const wxString& name, const wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    wxString text;
    if ( item.IsOk() )
        text << '"' << GetItemText(item).c_str() << '"';
    else
        text = "invalid item";
    // wxLogMessage("%s(%s)", name, text);
}

// ADDED FROM WXWIDGETS MANUAL, for tree
// avoid repetition
#define TREE_EVENT_HANDLER(name)                                 \
void Tree_Ctrl::name(wxTreeEvent& event)                        \
{                                                                \
    LogEvent(#name, event);                                  \
    event.Skip();                                                \
}

TREE_EVENT_HANDLER(OnItemExpanded)
TREE_EVENT_HANDLER(OnItemExpanding)
TREE_EVENT_HANDLER(OnItemCollapsed)

#undef TREE_EVENT_HANDLER