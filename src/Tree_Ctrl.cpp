//
// Created by MikeYang on 7/14/21.
//
//#pragma once
#include "cMain.h"
#include "cApp.h"
#include "Tree_Ctrl.h"
#include <vector>
// Tree added includes
#include <wx/dir.h>
#include <wx/arrstr.h>
#include <iostream>
#include <functional>
#include "wx/image.h"
#include "wx/treectrl.h"
#include "wx/math.h"
#include "wx/wupdlock.h"

static const int NUM_CHILDREN_PER_LEVEL = 5;
static const int NUM_LEVELS = 2;

wxIMPLEMENT_DYNAMIC_CLASS(Tree_Ctrl, wxTreeCtrl);
wxBEGIN_EVENT_TABLE(Tree_Ctrl, wxTreeCtrl)
  EVT_TREE_ITEM_ACTIVATED(Dec_Tree, Tree_Ctrl::OnItemActivated)
  EVT_TREE_ITEM_EXPANDED(Dec_Tree, Tree_Ctrl::OnItemExpanded)
  EVT_TREE_ITEM_EXPANDING(Dec_Tree, Tree_Ctrl::OnItemExpanding)
  EVT_TREE_ITEM_COLLAPSED(Dec_Tree, Tree_Ctrl::OnItemCollapsed)
  EVT_TREE_ITEM_COLLAPSING(Dec_Tree, Tree_Ctrl::OnItemCollapsing)
wxEND_EVENT_TABLE()

Tree_Ctrl::Tree_Ctrl(wxWindow *parent, const wxWindowID id,
                     const wxPoint& pos, const wxSize& size,
                     long style)
        : wxTreeCtrl(parent, id, pos, size, style,wxDefaultValidator, "Decrypted Files")
{

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
                image = imageSel = -1;
            wxTreeItemId id = AppendItem(idParent, str, image, imageSel,
                                         new Tree_Data(str));
                SetItemState(id, 0);
            AddItemsRecursively(id, numChildren, depth - 1, n + 1);
        }
    }
    //else: done!
}

void Tree_Ctrl::update_current(){
    wxArrayString current_files =d_target->get_current();
    size_t nFiles = current_files.GetCount();
    std::cout<<"Able to get to drop"<<std::endl;

//    DeleteChildren(rootId);
    if (current_files.GetCount()==0) return;
    for (unsigned int i =0;i<nFiles;i++){
      if(wxFileExists(current_files[i]))addFileToTree(current_files[i]);
      else{
        d_target->m_pOwner->WriteText(current_files[i]);
        d_target->m_pOwner->WriteText(" is folder or doesn't exist, not added.\n");
      }
    }

}

void Tree_Ctrl::addFileToTree(const wxString& tree_str) {
//    if(tree_str.empty()) return;

    size_t a = hasher((char*)tree_str.mb_str().data());
    size_t b = hasher((char*)tree_str.mb_str().data());
    std::cout<<"Hashing "<<a<<std::endl;
    tree_pair = tree_eles.insert(std::pair<size_t,size_t>(a, b));
    if( tree_pair.second==false)return;
    std::cout<<"Adding "<<tree_str<<std::endl;
    wxTreeItemId id = AppendItem(rootId, tree_str,-1,
                                 -1,new Tree_Data(tree_str));
        SetItemState(id, 0);

}

void Tree_Ctrl::AddTestItemsToTree(	size_t children_count,
                                       size_t depth)
{
    wxTreeItemId rootId = AddRoot("Root",-1,-1,
                                  new Tree_Data("Root item"));

    AddItemsRecursively(rootId, children_count, depth, 0);



}

void Tree_Ctrl::OnItemCollapsing(wxTreeEvent& event)
{

    wxTreeItemId itemId = event.GetItem();
}


void Tree_Ctrl::OnItemActivated(wxTreeEvent& event){
  std::printf("[tree::OnItemActivated] Starting operation\n");

  wxTreeItemId itemId = event.GetItem();
  auto *item = (Tree_Data *)GetItemData(itemId);
  wxString file_name_copy = item->m_desc;

  if(wxFileExists(file_name_copy+".pdm"))((cMain*)parent)->open_enc_file(file_name_copy);
  else ((cMain*)parent)->open_file(file_name_copy);

}

bool Tree_Ctrl::DnDFile::OnDropFiles(wxCoord, wxCoord, const wxArrayString& filenames){
    checked=1;
    size_t nFiles = filenames.GetCount();
    wxString str;

    str.Printf( "%d files dropped", (int)nFiles);

    if (m_pOwner != nullptr)
      m_pOwner->WriteText(_T("\n"));
    if (nFiles > 0) {
        for (size_t i = 0; i < filenames.GetCount(); i++) {
            cur_files.push_back(filenames[i]);
        }
        has_update_files=1;
    }
    return true;
}


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

