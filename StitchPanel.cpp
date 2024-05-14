//
// Created by ghisl on 08/05/2024.
//

#include "StitchPanel.h"
#include "MainFrame.h"

StitchPanel::StitchPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {

    wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Stitcher Frame"));

    wxBoxSizer* vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->AddStretchSpacer();
    vBoxSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    vBoxSizer->AddStretchSpacer();

    SetSizer(vBoxSizer);

    SetBackgroundColour(*wxWHITE);

    dynamic_cast<MainFrame*>(GetParent())->loadImageIOMenu();

}
