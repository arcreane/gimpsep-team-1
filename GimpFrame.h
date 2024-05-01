//
// Created by ghisl on 01/05/2024.
//

#ifndef GIMPSEP_TEAM_1_GIMPFRAME_H
#define GIMPSEP_TEAM_1_GIMPFRAME_H

#include "wx/wx.h"

class GimpFrame : public wxFrame {
public:
    explicit GimpFrame(const wxString& title);
private:
    wxPanel* leftPanel;
    wxPanel* rightPanel;
    wxPanel* submenuPanel;

    wxGridSizer* CreateButtonGrid();

    void OnButton1Clicked(wxCommandEvent& event);
    void OnButton2Clicked(wxCommandEvent& event);
    void OnButton3Clicked(wxCommandEvent& event);

    void CreateSubmenu1();
    void CreateSubmenu2();
    void CreateSubmenu3();


};


#endif //GIMPSEP_TEAM_1_GIMPFRAME_H
