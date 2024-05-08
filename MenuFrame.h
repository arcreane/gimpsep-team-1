//
// Created by ghisl on 08/05/2024.
//

#ifndef GIMPSEP_TEAM_1_MENUFRAME_H
#define GIMPSEP_TEAM_1_MENUFRAME_H

#include <wx/wx.h>

class MenuFrame : public wxFrame
{
public:
    MenuFrame();
private:
    wxButton* gotoEditorButton;
    wxButton* gotoStitcherButton;
    void OnGotoEditorButtonClicked(wxCommandEvent &event);
    void OnGotoStitcherButtonClicked(wxCommandEvent &event);
};


#endif //GIMPSEP_TEAM_1_MENUFRAME_H
