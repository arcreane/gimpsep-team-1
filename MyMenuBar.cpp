//
// Created by ghisl on 14/05/2024.
//

#include "MyMenuBar.h"
#include "MainFrame.h"
#include "MenuPanel.h"

MyMenuBar::MyMenuBar(wxPanel* panel) {

    contentPanel = panel;
    auto* menuMenu = new wxMenu();
    menuMenu->Append(0, wxT("Main menu"));
    auto* lsMenu = new wxMenu();
    lsMenu->Append(1, wxT("Load Image"));
    lsMenu->Append(2, wxT("Save Image"));

    this->Append(menuMenu, wxT("Menu"));
    this->Append(lsMenu, wxT("Image IO"));

    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onMainMenuClk, this, 0);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onLoadImageClk, this, 1);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onSaveImageClk, this, 2);

}

void MyMenuBar::onMainMenuClk(wxCommandEvent &event) {

    dynamic_cast<MainFrame*>(GetParent())->loadMenuPanel();
}

void MyMenuBar::onLoadImageClk(wxCommandEvent &event) {

}

void MyMenuBar::onSaveImageClk(wxCommandEvent &event) {

}
