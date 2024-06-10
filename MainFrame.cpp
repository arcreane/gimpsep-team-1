//
// Created by ghisl on 08/05/2024.
//

#include "MainFrame.h"
#include "MenuPanel.h"
#include "MyMenuBar.h"

// Constructor for MainFrame, initializes the main application window
MainFrame::MainFrame() : wxFrame(nullptr, wxID_ANY, wxT("GimpIsep"), wxDefaultPosition,
                                 wxSize(1200, 700), wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)  {

    // Initializes the menu bar and content panels
    menuBar = new MyMenuBar(contentPanel);

    vBoxSizer = new wxBoxSizer(wxVERTICAL);

    // Creates the different panels for the application
    menuPanel = new MenuPanel(this);
    editorPanel = new EditorPanel(this);
    stitchPanel = new StitchPanel(this);

    // Set the initial content panel to the menu panel
    contentPanel = menuPanel;
    vBoxSizer->Add(contentPanel, 1, wxEXPAND | wxALL, 5);

    SetSizer(vBoxSizer);

    // Loads the different panels initially
    loadEditorPanel();
    loadStitcherPanel();
    loadMenuPanel();

    SetBackgroundColour(*wxWHITE);
}

// Returns an identifier for the currently displayed panel
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

// Sets the content panel to the specified panel
void MainFrame::setContentPanel(wxPanel* panel) {
    if (contentPanel != nullptr) {
        vBoxSizer->Detach(contentPanel);
        contentPanel->Hide();
    }
    if (panel != contentPanel) {
        contentPanel = panel;
        vBoxSizer->Add(contentPanel, 1, wxEXPAND | wxALL, 5);
    }
    contentPanel->Show();
    Layout();
}

// Shows the image input/output menu
void MainFrame::loadImageIOMenu() {
    SetMenuBar(menuBar);
}

// Hides the image input/output menu
void MainFrame::hideImageIOMenu() {
    SetMenuBar(nullptr);
}

// Loads the menu panel
void MainFrame::loadMenuPanel() {
    setContentPanel(menuPanel);
    hideImageIOMenu();
}

// Loads the editor panel
void MainFrame::loadEditorPanel() {
    setContentPanel(editorPanel);
    loadImageIOMenu();
}

// Loads the stitcher panel
void MainFrame::loadStitcherPanel() {
    setContentPanel(stitchPanel);
    loadImageIOMenu();
}

// Sets and displays the image in the editor panel
void MainFrame::setEditorPanelImage(cv::Mat inImage) {
    editorPanel->setImage(std::move(inImage));
    editorPanel->displayMainImageToPanel();
}

// Returns the edited image from the editor panel
cv::Mat MainFrame::getEditedImage() {
    return editorPanel->getImage();
}

// Adds an image to the stitcher list in the stitcher panel
void MainFrame::addImageToStitcherList(const cv::Mat& inImage) {
    stitchPanel->addImageToList(inImage);
    stitchPanel->displayImageList();
}

// Returns the stitched image from the stitcher panel
cv::Mat MainFrame::getStitchedImage() {
    return stitchPanel->getStitchedImage();
}
