//
// Created by ghisl on 08/05/2024.
//

#ifndef GIMPSEP_TEAM_1_MAINFRAME_H
#define GIMPSEP_TEAM_1_MAINFRAME_H

#include <wx/wx.h>
#include "MenuPanel.h"
#include "EditorPanel.h"
#include "StitchPanel.h"

class MainFrame : public wxFrame
{
public:
    MainFrame();
    void loadImageIOMenu();
    void hideImageIOMenu();
    void loadMenuPanel();
    void loadEditorPanel();
    void loadStitcherPanel();
private:
    wxMenuBar* menuBar;
    MenuPanel* menuPanel;
    EditorPanel* editorPanel;
    StitchPanel* stitchPanel;
    wxPanel* contentPanel;
    wxBoxSizer* vBoxSizer;

    void SetContentPanel(wxPanel* panel);
};


#endif //GIMPSEP_TEAM_1_MAINFRAME_H
