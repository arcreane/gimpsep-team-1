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
    wxButton* editorBtn;
    wxButton* stitcherBtn;
    void onEditorBtnClk(wxCommandEvent &event);
    void onStitcherBtnClk(wxCommandEvent &event);
};


#endif //GIMPSEP_TEAM_1_MENUPANEL_H
