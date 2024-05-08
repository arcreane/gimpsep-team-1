//
// Created by ghisl on 08/05/2024.
//

#include "MenuFrame.h"
#include "EditorFrame.h"
#include "StitchFrame.h"

MenuFrame::MenuFrame() : wxFrame(nullptr, wxID_ANY, wxT("GimpIsep"),wxDefaultPosition,
                                 wxSize(1200, 700),wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)  {

    wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Choose an option:"));

    gotoEditorButton = new wxButton(this, wxID_ANY, wxT("Editor"));
    gotoStitcherButton = new wxButton(this, wxID_ANY, wxT("Stitcher"));
    gotoEditorButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MenuFrame::OnGotoEditorButtonClicked, this);
    gotoStitcherButton->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MenuFrame::OnGotoStitcherButtonClicked, this);

    wxBoxSizer* hBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    hBoxSizer->Add(gotoEditorButton, 0, wxALL, 5);
    hBoxSizer->Add(gotoStitcherButton, 0, wxALL, 5);

    wxBoxSizer* vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->AddStretchSpacer();
    vBoxSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    vBoxSizer->Add(hBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);
    vBoxSizer->AddStretchSpacer();

    SetSizer(vBoxSizer);

    SetBackgroundColour(*wxWHITE);

}

void MenuFrame::OnGotoEditorButtonClicked(wxCommandEvent& event) {
    auto* editorFrame = new EditorFrame();
    editorFrame->Show();
    this->Close();
}

void MenuFrame::OnGotoStitcherButtonClicked(wxCommandEvent& event) {
    auto* stitcherFrame = new StitchFrame();
    stitcherFrame->Show();
    this->Close();
}
