//
// Created by MikeYang on 8/4/21.
//

#include "pdmrc.h"
#include "wx/xml/xml.h"
#include "cMain.h"
#include "wx/time.h"
#include "wx/utils.h"
#include <wx/filefn.h>
#include <wx/dir.h>
#include <wx/filename.h>

void pdmrc::init_rc(){
  pdm_rc=new wxXmlDocument();
  pdm_dir=new wxFileName();
  loader=new wxXmlDocument();
  //Reserved, uncomment if need different documentation
//  doc_rc = new wxXmlNode(wxXML_ELEMENT_NODE,"pdm",wxNow());
  root_rc = new wxXmlNode(nullptr,wxXML_ELEMENT_NODE,"pdm",wxEmptyString,new wxXmlAttribute("class","pdm"));
  child_time = (new wxXmlNode(wxXML_ELEMENT_NODE,"logs"));
  child_time->AddChild(new wxXmlNode(wxXML_TEXT_NODE,"",wxNow()));
  root_rc->AddChild(child_time);
//  root_rc->GetChildren()->GetNext()->AddChild(new wxXmlNode(wxXML_ENTITY_REF_NODE,"time",wxNow()));
//  pdm_rc->SetDocumentNode(doc_rc);
  pdm_rc->SetRoot(root_rc);
}

void pdmrc::load_rc(){
//  wxString directory_name=pdm_dir->GetCwd()+sp+".pdmrc";
  wxString directory_name=file_dirtry;
  if(!wxFileExists(directory_name+sp+file_name)){
    ((cMain*)parent)->write_log("No configuration files");
    return;
  }
  std::cout<<"rc1 "<<directory_name+sp+file_name<<std::endl;
  loader=new wxXmlDocument();

  loader->Load(directory_name+sp+file_name);
  if(!loader->IsOk()){
    ((cMain*)parent)->write_log("Configuration file loading failure");
    return;
  }
  load_rc2=loader->GetRoot()->GetChildren();

  std::cout<<"Reading config file "<<directory_name+sp+file_name<<std::endl;
  while(load_rc2){
    if(load_rc2->GetName()=="logs"){
      load_rc1=load_rc2->GetChildren();
      std::cout<<"last save loaded from "+
      load_rc1->GetContent()<<std::endl;
      ((cMain*)parent)->write_log("last save loaded from "+
      load_rc1->GetContent());
    }
    else if(load_rc2->GetName()=="tree-rc"&& load_rc2->GetChildren()){
      load_rc1=load_rc2->GetChildren();
      std::cout<<"loading file \""+
      load_rc1->GetChildren()->GetContent()<<"\""<<std::endl;
      while(load_rc1){
        ((cMain*)parent)->tree_ctrl->
        addFileToTree(load_rc1->GetChildren()->GetContent());
        load_rc1=load_rc1->GetNext();
      }
    }
    load_rc2=load_rc2->GetNext();
  }
}

void pdmrc::read_rc(){
  delete tree_rc;
  tree_rc=new wxXmlNode(wxXML_ELEMENT_NODE,"tree-rc");
  int count=0;
  wxXmlNode*tree_file_rc=NULL;
  wxXmlNode*tree_str1_rc=NULL;
  wxXmlNode*tree_str2_rc=NULL;

  for (auto const& fl:((cMain*)parent)->tree_ctrl->tree_eles){
//    std::cout<<"Read and writing config "
//          <<fl.second<<std::endl;
    tree_file_rc=new wxXmlNode(wxXML_ELEMENT_NODE,"f"+std::to_string(count));
    if(tree_str1_rc)tree_str1_rc->SetNext(tree_file_rc);
    tree_str2_rc=new wxXmlNode(wxXML_TEXT_NODE,"",fl.second);
    tree_file_rc->AddChild(tree_str2_rc);
    tree_rc->AddChild(tree_file_rc);
    count++;
    tree_str1_rc=tree_file_rc;
  }
}

void pdmrc::save_rc() {
  pdm_dir=new wxFileName();
  wxString directory_name=file_dirtry;
  if(tree_rc!= nullptr)
    root_rc->AddChild(tree_rc);
  pdm_dir->Mkdir(directory_name,0777,wxPATH_MKDIR_FULL);
//  if(((cMain*)parent)->DEBUG_OUT_PDM)
//    ((cMain*)parent)->write_log("Saving config to "+directory_name+sp+file_name);
  pdm_rc->Save(directory_name+sp+file_name);
}