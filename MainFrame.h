#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    wxPanel* lateralPanel;
    wxBoxSizer* lateralSizer;

    void OnButton1Click(wxCommandEvent& event);
    void OnButton2Click(wxCommandEvent& event);
    void OnButton3Click(wxCommandEvent& event);

    wxBoxSizer* CreateLayout1();
    wxBoxSizer* CreateLayout2();
    wxBoxSizer* CreateLayout3();

    void ShowLayout(wxBoxSizer* layout);
    void HideLayout();
};

#endif // MAIN_FRAME_H
