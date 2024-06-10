//
// Created by ghisl on 08/05/2024.
//

#include "MenuPanel.h"
#include "MainFrame.h"

// Constructor for MenuPanel, initializes the menu interface
MenuPanel::MenuPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {

    // Creates a font object for the title
    wxFont font(20, wxFONTFAMILY_DEFAULT, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL);

    // Creates and sets the title text
    auto* title = new wxStaticText(this, wxID_ANY, wxT("Gimpsep T1"));
    title->SetFont(font);

    // Creates buttons for editing an image and stitching images
    editorBtn = new wxButton(this, wxID_ANY, wxT("Edit an image"));
    stitcherBtn = new wxButton(this, wxID_ANY, wxT("Stitch images"));

    // Binds button click events to their respective handlers
    editorBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MenuPanel::onEditorBtnClk, this);
    stitcherBtn->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &MenuPanel::onStitcherBtnClk, this);

    // Creates a horizontal box sizer and add the buttons to it
    auto* hBoxSizer = new wxBoxSizer(wxHORIZONTAL);
    hBoxSizer->Add(editorBtn, 0, wxALL, 5);
    hBoxSizer->Add(stitcherBtn, 0, wxALL, 5);

    // Create a vertical box sizer and add the title and button sizer to it
    auto* vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->AddStretchSpacer(); // Add a stretchable space above the title
    vBoxSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    vBoxSizer->AddSpacer(15); // Add some space between the title and buttons
    vBoxSizer->Add(hBoxSizer, 0, wxALIGN_CENTER_HORIZONTAL | wxALIGN_CENTER_VERTICAL, 0);
    vBoxSizer->AddStretchSpacer(); // Add a stretchable space below the buttons

    // Sets the sizer for the panel
    SetSizer(vBoxSizer);
}

// Event handler for the "Edit an image" button click
void MenuPanel::onEditorBtnClk(wxCommandEvent& event) {
    dynamic_cast<MainFrame*>(GetParent())->loadEditorPanel();
}

// Event handler for the "Stitch images" button click
void MenuPanel::onStitcherBtnClk(wxCommandEvent& event) {
    dynamic_cast<MainFrame*>(GetParent())->loadStitcherPanel();
}

