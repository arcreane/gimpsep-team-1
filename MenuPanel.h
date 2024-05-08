//
// Created by ghisl on 08/05/2024.
//

#ifndef GIMPSEP_TEAM_1_MENUPANEL_H
#define GIMPSEP_TEAM_1_MENUPANEL_H

#include <wx/wx.h>

class MenuPanel : public wxPanel {
public:
    explicit MenuPanel(wxWindow* parent);

private:
    wxButton* gotoEditorButton;
    wxButton* gotoStitcherButton;
    void OnGotoEditorButtonClicked(wxCommandEvent &event);
    void OnGotoStitcherButtonClicked(wxCommandEvent &event);

};


#endif //GIMPSEP_TEAM_1_MENUPANEL_H
