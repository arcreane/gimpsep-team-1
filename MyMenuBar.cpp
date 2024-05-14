//
// Created by ghisl on 14/05/2024.
//

#include "MyMenuBar.h"

MyMenuBar::MyMenuBar(wxPanel* panel) {

    contentPanel = panel;
    auto* menuMenu = new wxMenu();
    menuMenu->Append(0, wxT("Main menu"));
    auto* lsMenu = new wxMenu();
    lsMenu->Append(1, wxT("Load Image"));
    lsMenu->Append(2, wxT("Save Image"));

    this->Append(menuMenu, wxT("Menu"));
    this->Append(lsMenu, wxT("Image IO"));

}
