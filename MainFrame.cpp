//
// Created by ghisl on 08/05/2024.
//

#include "MainFrame.h"

#include <utility>
#include "MenuPanel.h"
#include "MyMenuBar.h"

MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, wxT("GimpIsep"), wxDefaultPosition,
                                 wxSize(1200, 700),wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)  {

    menuBar = new MyMenuBar(contentPanel);

    vBoxSizer = new wxBoxSizer(wxVERTICAL);

    menuPanel = new MenuPanel(this);
    editorPanel = new EditorPanel(this);
    stitchPanel = new StitchPanel(this);

    contentPanel = menuPanel;
    vBoxSizer->Add(contentPanel, 1, wxEXPAND | wxALL, 5);

    SetSizer(vBoxSizer);
    loadEditorPanel();
    loadStitcherPanel();
    loadMenuPanel();

    SetBackgroundColour(*wxWHITE);

}

int MainFrame::getCurrentPanel() {
    if (contentPanel == menuPanel) {
        return 0;
    } else if (contentPanel == editorPanel) {
        return 1;
    } else if (contentPanel == stitchPanel) {
        return 2;
    } else {
        wxASSERT(false);
        return -1;
    }
}

void MainFrame::SetContentPanel(wxPanel* newPanel) {
    if (contentPanel != nullptr) {
        vBoxSizer->Detach(contentPanel);
        contentPanel->Hide();
    }
    if (newPanel != contentPanel) {
        contentPanel = newPanel;
        vBoxSizer->Add(contentPanel, 1, wxEXPAND | wxALL, 5);
    }
    contentPanel->Show();
    Layout();
}

void MainFrame::loadImageIOMenu() {
    SetMenuBar(menuBar);
}

void MainFrame::hideImageIOMenu() {
    SetMenuBar(nullptr);
}

void MainFrame::loadMenuPanel() {
    SetContentPanel(menuPanel);
    hideImageIOMenu();
}

void MainFrame::loadEditorPanel() {
    SetContentPanel(editorPanel);
    loadImageIOMenu();
}

void MainFrame::loadStitcherPanel() {
    SetContentPanel(stitchPanel);
    loadMenuPanel();
}

void MainFrame::setEditorPanelImage(cv::Mat inImage) {
    editorPanel->setImage(std::move(inImage));
    editorPanel->displayMainImageToPanel();

}

cv::Mat MainFrame::getEditedImage() {
    return editorPanel->getImage();
}


