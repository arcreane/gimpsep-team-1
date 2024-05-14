//
// Created by ghisl on 08/05/2024.
//

#include "MainFrame.h"
#include "MenuPanel.h"
#include "MyMenuBar.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, wxT("GimpIsep"), wxDefaultPosition,
                                 wxSize(1200, 700),wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)  {

    contentPanel = new MenuPanel(this);
    vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->Add(contentPanel, 1, wxEXPAND | wxALL, 5);
    SetSizer(vBoxSizer);

    menuBar = new MyMenuBar(contentPanel);

    SetBackgroundColour(*wxWHITE);

}

void MainFrame::SetContentPanel(wxPanel* panel) {
    if (contentPanel) {
        contentPanel->Destroy();
    }
    contentPanel = panel;
    vBoxSizer->Add(contentPanel, 1, wxEXPAND | wxALL, 5);
    Layout();
}

void MainFrame::loadImageIOMenu() {
    SetMenuBar(menuBar);
}

void MainFrame::hideImageIOMenu() {
    SetMenuBar(nullptr);
}


