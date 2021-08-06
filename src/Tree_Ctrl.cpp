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
#include "id.h"
#include "wx/treectrl.h"
#include "wx/math.h"
#include "wx/wupdlock.h"

static const int NUM_CHILDREN_PER_LEVEL = 5;
static const int NUM_LEVELS = 2;

wxIMPLEMENT_DYNAMIC_CLASS(Tree_Ctrl, wxTreeCtrl);
#define MENU_LINK(name) EVT_MENU(name, Tree_Ctrl::On##name)


wxBEGIN_EVENT_TABLE(Tree_Ctrl, wxTreeCtrl)
  EVT_TREE_ITEM_ACTIVATED(Dec_Tree, Tree_Ctrl::OnItemActivated)
  EVT_TREE_ITEM_EXPANDED(Dec_Tree, Tree_Ctrl::OnItemExpanded)
  EVT_TREE_ITEM_EXPANDING(Dec_Tree, Tree_Ctrl::OnItemExpanding)
  EVT_TREE_ITEM_COLLAPSED(Dec_Tree, Tree_Ctrl::OnItemCollapsed)
  EVT_TREE_ITEM_COLLAPSING(Dec_Tree, Tree_Ctrl::OnItemCollapsing)
  EVT_TREE_ITEM_GETTOOLTIP(Dec_Tree, Tree_Ctrl::ShowTreeTooltip)
  EVT_TREE_ITEM_MENU(Dec_Tree, Tree_Ctrl::OnItemMenu)
  MENU_LINK(ItemMenuDelete)
  MENU_LINK(ItemMenuOpenEnc)
  MENU_LINK(ItemMenuOpenPlain)
  MENU_LINK(ItemMenuDir)
  EVT_MENU_HIGHLIGHT_ALL(Tree_Ctrl::OnMenuHighlight)
  EVT_MENU_CLOSE(Tree_Ctrl::OnMenuClose)
wxEND_EVENT_TABLE()



Tree_Ctrl::Tree_Ctrl(wxWindow *parent, const wxWindowID id,
                     const wxPoint& pos, const wxSize& size,
                     long style)
        : wxTreeCtrl(parent, id, pos, size, style,wxDefaultValidator, "Decrypted Files")
{
  menu =new wxMenu("");

  item_delete=new wxMenuItem(menu,ItemMenuDelete,"&Delete","Removes the file from list, but doesn't delete from your system");
  item_open_enc=new wxMenuItem(menu,ItemMenuOpenEnc,"Open &Encrypted", "Opens Encrypted file if exists");
  item_open_plain=new wxMenuItem(menu,ItemMenuOpenPlain,"Open &Plain Text", "Opens Plain text file if exists");
  item_menu_str=new wxMenuItem(menu,ItemMenuStr,"File info","File info");
  item_menu_dir=new wxMenuItem(menu,ItemMenuDir,"Open in &Folder","Opens selected file in a folder");
  item_menu_str->Enable(false);
  menu->Append(item_menu_str);
  menu->Append(item_menu_dir);
  menu->AppendSeparator();
  menu->Append(item_delete);
  menu->AppendSeparator();
  menu->Append(item_open_enc);
  menu->Append(item_open_plain);
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

void Tree_Ctrl::clear_files_tree() {
  tree_eles.clear();
  reload_tree_from_eles();
}
void Tree_Ctrl::reload_tree_from_eles() {
  this->DeleteAllItems();
  for(auto const& fl:tree_eles){
    addFileToTree(fl.second);
  }
}

void Tree_Ctrl::addFileToTree(const wxString& tree_str) {
    int item_enc=0;
    wxString filename = tree_str;
    if(((cMain*)parent)->check_extend(tree_str)) {
      filename=((cMain*)parent)->extend_off(tree_str);
    }
    if(wxFileExists(filename+".pdm"))
      item_enc=1;
    else if(wxFileExists(filename))
      item_enc=0;
    else
      item_enc=-1;
    size_t a = hasher((char*)filename.mb_str().data());
    tree_pair = tree_eles.insert(std::pair<size_t,std::string>(a, (char*)filename.mb_str().data()));
    if( tree_pair.second==false)return;
    wxTreeItemId id = AppendItem(rootId, wxFileNameFromPath(filename),-1,
                                 -1,new Tree_Data(filename));
    SetItemTextColour(id,wxTheColourDatabase->Find("LIGHT GREY"));
    if(item_enc==1)
      SetItemTextColour(id,wxTheColourDatabase->Find("WHITE"));
    else if(item_enc==-1)
      SetItemTextColour(id,wxTheColourDatabase->Find("RED"));
    wxString tip;
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

wxString Tree_Ctrl::name_by_event(wxTreeEvent& event){
  wxTreeItemId itemId = event.GetItem();
  auto *item = (Tree_Data *)GetItemData(itemId);
  wxString file_name_copy = item->m_desc;
  return file_name_copy;
}
wxString Tree_Ctrl::name_by_event(wxCommandEvent& event){
  wxTreeItemId itemId = this->GetFocusedItem();
  auto *item = (Tree_Data *)GetItemData(itemId);
  wxString file_name_copy = item->m_desc;
  return file_name_copy;
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

size_t Tree_Ctrl::get_item_hash(wxString a) {
  return hasher((char*)a.mb_str().data());
}

void Tree_Ctrl::ShowTreeTooltip (wxTreeEvent& event){
  wxString tip;
  std::cout<<"";
  if(wxFileExists(name_by_event(event)+".pdm")){
    tip=wxString::Format( "[Encrypted] %s",name_by_event(event));
    if(wxFileExists(name_by_event(event)))
      tip=wxString::Format( "[Both] %s",name_by_event(event));
  }
  else
    tip=wxString::Format( "[Not Encrypted] %s",name_by_event(event)) ;
  event.SetToolTip(tip);
//  ((cMain*)parent)->SetStatusText(tip,0);

}

wxString Tree_Ctrl::get_tooltip (wxTreeEvent& event){
  wxString tip;
  std::cout<<"";
  if(wxFileExists(name_by_event(event)+".pdm")){
    tip=wxString::Format( "[Encrypted] %s",name_by_event(event));
    if(wxFileExists(name_by_event(event)))
      tip=wxString::Format( "[Both] %s",name_by_event(event));
  }
  else
    tip=wxString::Format( "[Not Encrypted] %s",name_by_event(event)) ;
//  event.SetToolTip(tip);
  return tip;
}

void Tree_Ctrl::OnItemMenu(wxTreeEvent& event){
  wxTreeItemId itemId = event.GetItem();
  wxCHECK_RET( itemId.IsOk(), "should have a valid item" );
  wxPoint clientpt = event.GetPoint();
  wxString title;
  if ( itemId.IsOk() )
  {
    title << "Menu for " << GetItemText(itemId);
  }
  else
  {
    title = "Menu for no particular item";
  }
  menu->SetTitle(title);
  item_menu_str->SetItemLabel(get_tooltip(event));
  PopupMenu(menu, clientpt);
}

void Tree_Ctrl::OnMenuClose(wxMenuEvent &event) {
  ((cMain*)parent)->SetStatusText("pdm",0);

}

void Tree_Ctrl::OnMenuHighlight(wxMenuEvent& event)
  {
    wxString msg;


    if ( event.GetEventType() == wxEVT_MENU_HIGHLIGHT )
    {
      switch(event.GetId()){
        case ItemMenuDelete:
          ((cMain*)parent)->SetStatusText("Removes the file from the list",0);
          break;
        case ItemMenuOpenPlain:
          ((cMain*)parent)->SetStatusText("If exists, open plain file",0);
          break;
        case ItemMenuOpenEnc:
          ((cMain*)parent)->SetStatusText("If exists, open encrypted file",0);
          break;
        case ItemMenuStr:
          ((cMain*)parent)->SetStatusText("File info",0);
          break;
        case ItemMenuDir:
          ((cMain*)parent)->SetStatusText("Opens the file in a separate folder",0);
          break;
      }
    }
    msg << ".";
  }
void Tree_Ctrl::OnItemMenuDelete(wxCommandEvent& event){
  wxTreeItemId itemId = this->GetFocusedItem();
//  this->DeleteChildren(itemId);//Doesn't refresh
  tree_eles.erase(get_item_hash(name_by_event(event)));
  this->Delete(itemId);

}
void Tree_Ctrl::OnItemMenuOpenEnc(wxCommandEvent &event) {
  ((cMain*)parent)->open_enc_file(name_by_event(event));
}
void Tree_Ctrl::OnItemMenuOpenPlain(wxCommandEvent& event){
  ((cMain*)parent)->open_file(name_by_event(event));

}

void Tree_Ctrl::OnGetToolTip(wxCommandEvent& event){
  ((cMain*)parent)->SetStatusText(event.GetString(),0);
}
void Tree_Ctrl::OnItemMenuDir(wxCommandEvent& event){
  wxString directory_open = name_by_event(event);

  #ifdef __WXMSW__
    wxExecute(_("explorer.exe "+directory_open));
  #endif//WXMSW
  #ifdef __WXMAC__
    wxExecute(_("open -R "+directory_open));
  #endif//__WXMAC__
  #ifdef __WXGTK__
    wxExecute(_("nautilus "+directory_open));
    ((cMain*)parent)->write_log("The Linux distribution information is not clear");
    ((cMain*)parent)->write_log("If nothing happens, you may manually goto \""+
    directory_open+"\" directly.");
  #endif//__WXGTK__
}

void Tree_Ctrl::LogEvent(const wxString& name, const wxTreeEvent& event)
{
    wxTreeItemId item = event.GetItem();
    wxString text;
    if ( item.IsOk() )
        text << '"' << GetItemText(item).c_str() << '"';
    else
        text = "invalid item";
}
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

