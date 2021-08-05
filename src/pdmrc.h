//
// Created by MikeYang on 8/4/21.
//

#ifndef PDM_PDMRC_H
#define PDM_PDMRC_H
#include <wx/artprov.h>
#include <wx/filename.h>

#ifndef WX_PRECOMP
	#include "wx/wx.h"
#endif
#include "wx/xml/xml.h"


class pdmrc {
public:
  explicit pdmrc(wxFrame* a){parent=a;}
  ~pdmrc()= default;
  void init_rc();
  void read_rc();
  void save_rc();
  void load_rc();

private:
  wxFrame* parent;
  wxXmlNode* doc_rc{};
  wxXmlNode* root_rc{};
  wxXmlNode* load_rc1{};
  wxXmlNode* load_rc2{};
  wxXmlDocument* loader{};
  wxXmlNode* child_time{};
  wxFileName* pdm_dir = new wxFileName();
  wxXmlNode* tree_rc=new wxXmlNode(wxXML_ELEMENT_NODE,"tree-rc");
  wxXmlDocument* pdm_rc{};
  const wxString sp= wxFileName::GetPathSeparators();
  const wxString file_name = "pdm_rc.conf";
};


#endif //PDM_PDMRC_H
