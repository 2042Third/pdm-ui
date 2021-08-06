//
// Created by MikeYang on 7/14/21.
//

#ifndef PDM_TREE_CTRL_H
#define PDM_TREE_CTRL_H
#include <wx/treectrl.h>
#include <wx/richtext/richtextbuffer.h>
#include "wx/dnd.h"
#pragma once
//#include "cApp.h"
#include <wx/artprov.h>
#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif
//#include "pdmFile.h"
//#include "cApp.h
#include <map>
#include <vector>
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




class Tree_Ctrl : public wxTreeCtrl
{
public:
    enum{
        Tree_Ctrl_Icon_Files,
        Tree_Ctrl_Icon_Folder
    };
    void update_current();

    Tree_Ctrl(){m_alternateImages = false; m_alternateStates = false;}
    Tree_Ctrl (wxWindow * parent, const wxWindowID id,
               const wxPoint& pos, const wxSize& size,
               long style);
    ~Tree_Ctrl(){}

    void OnUpdateUI(wxUpdateUIEvent&event);
    void OnItemExpanded(wxTreeEvent& event);
    void OnItemExpanding(wxTreeEvent& event);
    void OnItemCollapsed(wxTreeEvent& event);
    void OnItemCollapsing(wxTreeEvent& event);
    void addFileToTree(const wxString& tree_str);
    void OnItemActivated(wxTreeEvent& event);
    void OnMenuHighlight(wxMenuEvent& event);
    void ShowTreeTooltip(wxTreeEvent& event);
    void OnItemMenu(wxTreeEvent& event);
    void OnItemMenuDelete(wxCommandEvent& event);
    void OnItemMenuOpenEnc(wxCommandEvent& event);
    void OnItemMenuOpenPlain(wxCommandEvent& event);
    void OnItemMenuDir(wxCommandEvent& event);
    void OnMenuClose(wxMenuEvent& event);
    wxString name_by_event(wxTreeEvent& event);
    wxString name_by_event(wxCommandEvent& event);
    wxString get_tooltip(wxTreeEvent& event);
    void clear_files_tree();
    void reload_tree_from_eles();
    void OnGetToolTip(wxCommandEvent& event) ;
    void SetAlternateImages(bool show) { m_alternateImages = show; }
    bool AlternateImages() const { return m_alternateImages; }
    void SetAlternateStates(bool show) { m_alternateStates = show; }
    bool AlternateStates() const { return m_alternateStates; }
    class DnDFile : public wxFileDropTarget
    {
    public:
        DnDFile(wxRichTextCtrl *pOwner = NULL) { m_pOwner = pOwner; }
        int checked=0;
        bool OnDropFiles(wxCoord x, wxCoord y,
                         const wxArrayString& filenames) ;
        bool file_droped(const wxArrayString& filenames) ;
        wxArrayString get_current(){return cur_files;};
        wxArrayString cur_files;
        int has_update_files=0;

        wxRichTextCtrl *m_pOwner;
    };
    wxMenu* menu;
    wxMenuItem* item_delete;
    wxMenuItem* item_open_enc;
    wxMenuItem* item_open_plain;
    wxMenuItem* item_menu_str;
    wxMenuItem* item_menu_dir;
    size_t get_item_hash(wxString a);
    void set_parent(wxFrame* a){parent=a;}
    void set_d_target(Tree_Ctrl::DnDFile* d){d_target=d;}
    Tree_Ctrl::DnDFile * d_target;
    wxFrame * parent;
    std::map<size_t,std::string> tree_eles;
    std::hash<std::string> hasher;
    std::pair<std::map<size_t,std::string>::iterator,bool> tree_pair;
private:

    wxTreeItemId rootId= AddRoot("Root",-1,-1,
                                 new Tree_Data("Root item"));
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
    class Tree_Data : public wxTreeItemData
    {
    public:
        Tree_Data(const wxString& desc) : m_desc(desc) { }

        void ShowInfo(wxTreeCtrl *tree);
        wxString const& GetDesc() const { return m_desc; }
//    private:
        wxString m_desc;
    };
};

enum{
    TreeTest_ToggleSel,
    File_Drop=1001,
    Dec_Tree  = 1000,
    ItemMenuDelete,
    ItemMenuOpenEnc,
    ItemMenuOpenPlain,
    ItemMenuDir,
    ItemMenuStr
};
#endif //PDM_TREE_CTRL_H
