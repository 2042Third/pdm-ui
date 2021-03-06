#include <wx/wxprec.h>
#ifndef WX_PRECOMP
// Include your minimal set of headers here, or wx.h
#include <wx/wx.h>
#endif

#include <wx/arrstr.h>
#include <wx/dir.h>
#include <wx/busyinfo.h>

class MyFrame : public wxFrame
{
public:
    MyFrame()
        : wxFrame(NULL, wxID_ANY, _("Test"))
    {
        wxBoxSizer* pSizer = new wxBoxSizer(wxVERTICAL);

        wxTextCtrl* dropTarget = new wxTextCtrl(this, wxID_ANY, _("Drop files onto me!"), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY);
        dropTarget->DragAcceptFiles(true);
        pSizer->Add(dropTarget, 1, wxEXPAND, 0);

        SetSizer(pSizer);
        Layout();
        Centre();

        dropTarget->Connect(wxEVT_DROP_FILES, wxDropFilesEventHandler(MyFrame::OnDropFiles), NULL, this);

    } 
private:
    void OnDropFiles(wxDropFilesEvent& event)   
    {
        if (event.GetNumberOfFiles() > 0) {

            wxString* dropped = event.GetFiles();
            wxASSERT(dropped);

            wxBusyCursor busyCursor;
            wxWindowDisabler disabler;    
            wxBusyInfo busyInfo(_("Adding files, wait please..."));

            wxString name;
            wxArrayString files;

            for (int i = 0; i < event.GetNumberOfFiles(); i++) {
                name = dropped[i];
                if (wxFileExists(name))
                    files.push_back(name);
                else if (wxDirExists(name))
                    wxDir::GetAllFiles(name, &files);                                    
            }

            wxTextCtrl* textCtrl = dynamic_cast<wxTextCtrl*>(event.GetEventObject());
            wxASSERT(textCtrl);
            textCtrl->Clear();
            for (size_t i = 0; i < files.size(); i++) {
                *textCtrl << files[i] << wxT('\n');
            }
        }
    }

};


/**** MyApp ****/
class MyApp : public wxApp
{
public: 
    virtual bool OnInit()
    {
        if (!wxApp::OnInit())
            return false;         

        MyFrame* frame = new MyFrame();
        frame ->Show();

        return true;
    }

};

IMPLEMENT_APP(MyApp)