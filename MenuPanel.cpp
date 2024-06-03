//
// Created by ghisl on 08/05/2024.
//

#include "MenuPanel.h"
#include "MainFrame.h"

MenuPanel::MenuPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {

    wxFont font(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);
    auto* title = new wxStaticText(this, wxID_ANY, wxT("Choose an option:"));
    title->SetFont(font);

    editorBtn = new wxButton(this, wxID_ANY, wxT("Editor"));
    stitcherBtn = new wxButton(this, wxID_ANY, wxT("Stitcher"));
    editorBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MenuPanel::onEditorBtnClk, this);
    stitcherBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MenuPanel::onStitcherBtnClk, this);

    auto* hBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    hBoxSizer->Add(editorBtn, 0, wxALL, 5);
    hBoxSizer->Add(stitcherBtn, 0, wxALL, 5);

    auto* vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->AddStretchSpacer();
    vBoxSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    vBoxSizer->AddSpacer(15);
    vBoxSizer->Add(hBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);
    vBoxSizer->AddStretchSpacer();

    SetSizer(vBoxSizer);
}

void MenuPanel::onEditorBtnClk(wxCommandEvent& event) {
    dynamic_cast<MainFrame*>(GetParent())->loadEditorPanel();
}

void MenuPanel::onStitcherBtnClk(wxCommandEvent& event) {
    dynamic_cast<MainFrame*>(GetParent())->loadStitcherPanel();
}
