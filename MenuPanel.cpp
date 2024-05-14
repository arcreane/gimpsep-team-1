//
// Created by ghisl on 08/05/2024.
//

#include "MenuPanel.h"
#include "MainFrame.h"

MenuPanel::MenuPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {
    wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Choose an option:"));

    gotoEditorButton = new wxButton(this, wxID_ANY, wxT("Editor"));
    gotoStitcherButton = new wxButton(this, wxID_ANY, wxT("Stitcher"));
    gotoEditorButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MenuPanel::OnGotoEditorButtonClicked, this);
    gotoStitcherButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MenuPanel::OnGotoStitcherButtonClicked, this);

    wxBoxSizer* hBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    hBoxSizer->Add(gotoEditorButton, 0, wxALL, 5);
    hBoxSizer->Add(gotoStitcherButton, 0, wxALL, 5);

    wxBoxSizer* vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->AddStretchSpacer();
    vBoxSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    vBoxSizer->Add(hBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);
    vBoxSizer->AddStretchSpacer();

    SetSizer(vBoxSizer);
}

void MenuPanel::OnGotoEditorButtonClicked(wxCommandEvent& event) {
    dynamic_cast<MainFrame*>(GetParent())->loadEditorPanel();
}

void MenuPanel::OnGotoStitcherButtonClicked(wxCommandEvent& event) {
    dynamic_cast<MainFrame*>(GetParent())->loadStitcherPanel();
}
