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
    int getCurrentPanel();
    void loadImageIOMenu();
    void hideImageIOMenu();
    void loadMenuPanel();
    void loadEditorPanel();
    void loadStitcherPanel();
    void setEditorPanelImage(cv::Mat);
    cv::Mat getEditedImage();
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
