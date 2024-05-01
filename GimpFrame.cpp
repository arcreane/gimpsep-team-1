//
// Created by ghisl on 01/05/2024.
//

#include "GimpFrame.h"

GimpFrame::GimpFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title,  wxDefaultPosition, wxSize(800, 600)) {

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    leftPanel = new wxPanel(this, wxID_ANY);
    leftPanel->SetBackgroundColour(*wxLIGHT_GREY);

    wxGridSizer* buttonGrid = CreateButtonGrid();

    submenuPanel = new wxPanel(leftPanel, wxID_ANY);
    submenuPanel->SetBackgroundColour(*wxWHITE);

    wxBoxSizer* leftPanelSizer = new wxBoxSizer(wxVERTICAL);
    leftPanelSizer->Add(buttonGrid, 1, wxEXPAND | wxALL, 5);
    leftPanelSizer->Add(submenuPanel, 1, wxEXPAND | wxALL, 5);

    leftPanel->SetSizer(leftPanelSizer);

    rightPanel = new wxPanel(this, wxID_ANY);
    rightPanel->SetBackgroundColour(*wxWHITE);

    mainSizer->Add(leftPanel, 3, wxEXPAND | wxALL, 5);
    mainSizer->Add(rightPanel, 7, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    SetSizeHints(wxDefaultSize, wxDefaultSize, wxSize(800, 600));

}

wxGridSizer* GimpFrame::CreateButtonGrid() {

    wxGridSizer* buttonGrid = new wxGridSizer(3, 3, 1, 1);

    wxButton* button1 = new wxButton(leftPanel, wxID_ANY, wxT("1"));
    buttonGrid->Add(button1, 0, wxALL, 5);
    button1->Bind(wxEVT_BUTTON, &GimpFrame::OnButton1Clicked, this);

    wxButton* button2 = new wxButton(leftPanel, wxID_ANY, wxT("2"));
    buttonGrid->Add(button2, 0, wxALL, 5);
    button2->Bind(wxEVT_BUTTON, &GimpFrame::OnButton2Clicked, this);

    wxButton* button3 = new wxButton(leftPanel, wxID_ANY, wxT("3"));
    buttonGrid->Add(button3, 0, wxALL, 5);
    button3->Bind(wxEVT_BUTTON, &GimpFrame::OnButton3Clicked, this);

    return buttonGrid;
}


void GimpFrame::CreateSubmenu1() {
    if (submenuPanel == nullptr) {
        return;
    }

    submenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(submenuPanel, wxID_ANY, wxT("This is submenu 1"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);

    submenuPanel->SetSizer(sizer);
    submenuPanel->Layout();
}

void GimpFrame::CreateSubmenu2() {
    if (submenuPanel == nullptr) {
        return;
    }

    submenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(submenuPanel, wxID_ANY, wxT("This is submenu 2"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);

    submenuPanel->SetSizer(sizer);
    submenuPanel->Layout();
}

void GimpFrame::CreateSubmenu3() {
    if (submenuPanel == nullptr) {
        return;
    }

    submenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(submenuPanel, wxID_ANY, wxT("This is submenu 3"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);

    submenuPanel->SetSizer(sizer);
    submenuPanel->Layout();
}


void GimpFrame::OnButton1Clicked(wxCommandEvent& event) {
    CreateSubmenu1();
}

void GimpFrame::OnButton2Clicked(wxCommandEvent& event) {
    CreateSubmenu2();
}

void GimpFrame::OnButton3Clicked(wxCommandEvent& event) {
    CreateSubmenu3();
}