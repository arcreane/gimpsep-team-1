//
// Created by ghisl on 08/05/2024.
//

#include "MainFrame.h"
#include "MenuPanel.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, wxT("GimpIsep"), wxDefaultPosition,
                                 wxSize(1200, 700),wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)  {

    menuBar = new wxMenuBar();

    auto* menu = new wxMenu();
    menu->Append(wxID_ANY, wxT("Submenu"));
    menuBar->Append(menu, wxT("Menu"));
    SetMenuBar(menuBar);

    contentPanel = new MenuPanel(this);
    vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->Add(contentPanel, 1, wxEXPAND | wxALL, 5);
    SetSizer(vBoxSizer);

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


