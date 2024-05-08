//
// Created by ghisl on 08/05/2024.
//

#ifndef GIMPSEP_TEAM_1_MAINFRAME_H
#define GIMPSEP_TEAM_1_MAINFRAME_H

#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
    MainFrame();
    void SetContentPanel(wxPanel* panel);
private:
    wxPanel* contentPanel;

};


#endif //GIMPSEP_TEAM_1_MAINFRAME_H
