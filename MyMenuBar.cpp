//
// Created by ghisl on 14/05/2024.
//

#include "MyMenuBar.h"
#include "MainFrame.h"

// Constructor for MyMenuBar, initializes the menu bar with specified menus and menu items
MyMenuBar::MyMenuBar(wxPanel* panel) {

    contentPanel = panel;

    // Creates a menu for navigation
    auto* menuMenu = new wxMenu();
    menuMenu->Append(0, wxT("Go to menu"));

    // Creates a menu for image input/output operations
    auto* lsMenu = new wxMenu();
    lsMenu->Append(1, wxT("Load Image"));
    lsMenu->Append(2, wxT("Save Image"));

    // Appends the created menus to the menu bar
    this->Append(menuMenu, wxT("Menu"));
    this->Append(lsMenu, wxT("Image IO"));

    // Binds menu item selections to their respective event handler functions
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onMainMenuClk, this, 0);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onLoadImageClk, this, 1);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onSaveImageClk, this, 2);
}

// Event handler for "Go to menu" selection
void MyMenuBar::onMainMenuClk(wxCommandEvent &event) {
    // Load the main menu panel
    dynamic_cast<MainFrame*>(GetParent())->loadMenuPanel();
}

// Event handler for "Load Image" selection
void MyMenuBar::onLoadImageClk(wxCommandEvent &event) {

    cv::Mat image;
    // Opens a file dialog to select an image file
    wxFileDialog dialog(this, "Select an image file", "", "", "Image files (*.jpg, *.jpeg, *.png)|*.jpg;*.jpeg;*.png", wxFD_OPEN);

    // Checks which panel is currently active
    switch (dynamic_cast<MainFrame*>(GetParent())->getCurrentPanel()) {
        case 0:
            wxMessageBox("You may not load an image here.", "Invalid location", wxOK | wxICON_INFORMATION);
            break;
        case 1:
            // If the editor panel is active, load the image into the editor
            if (dialog.ShowModal() == wxID_OK) {
                image = cv::imread(dialog.GetPath().ToStdString(), cv::IMREAD_COLOR);
                if (image.empty()) {
                    wxMessageBox("Failed to load image", "Error", wxOK | wxICON_ERROR);
                    return;
                }
                dynamic_cast<MainFrame*>(GetParent())->setEditorPanelImage(image);
            }
            break;
        case 2:
            // If the stitcher panel is active, add the image to the stitcher list
            if (dialog.ShowModal() == wxID_OK) {
                image = cv::imread(dialog.GetPath().ToStdString(), cv::IMREAD_COLOR);
                if (image.empty()) {
                    wxMessageBox("Failed to load image", "Error", wxOK | wxICON_ERROR);
                    return;
                }
                dynamic_cast<MainFrame*>(GetParent())->addImageToStitcherList(image);
            }
            break;
    }
}

// Event handler for "Save Image" selection
void MyMenuBar::onSaveImageClk(wxCommandEvent &event) {

    // Opens a file dialog to specify the save location and file name
    wxFileDialog dialog(this, "Save image as", "", "stitched.jpg", "Image files (*.jpg, *.png)|*.jpg;*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    cv::Mat image;

    // Checks which panel is currently active and get the corresponding image
    switch (dynamic_cast<MainFrame*>(GetParent())->getCurrentPanel()) {
        case 0:
            wxMessageBox("You may not load an image here.", "Invalid location", wxOK | wxICON_INFORMATION);
            return;
        case 1:
            image = dynamic_cast<MainFrame*>(GetParent())->getEditedImage();
            break;
        case 2:
            image = dynamic_cast<MainFrame*>(GetParent())->getStitchedImage();
            if(image.empty()){
                wxMessageBox("Please stitch images first.", "No image.", wxICON_INFORMATION);
                return;
            }
            break;
    }

    // If an image is available, proceed with the save operation
    if(!image.empty()){
        if (dialog.ShowModal() == wxID_OK) {
            std::string filePath = dialog.GetPath().ToStdString();
            int filterIndex = dialog.GetFilterIndex();
            std::string fileExt;
            switch (filterIndex) {
                case 0:
                    fileExt = "jpg";
                    break;
                case 1:
                    fileExt = "png";
                    break;
                default:
                    fileExt = "jpg";
                    break;
            }
            // Adjusts the file extension if necessary
            size_t lastDotPos = filePath.find_last_of('.');
            if (lastDotPos != std::string::npos && lastDotPos < filePath.size() - 1) {
                filePath.replace(lastDotPos + 1, filePath.size() - lastDotPos - 1, fileExt);
            } else {
                filePath += "." + fileExt;
            }
            // Saves the image using OpenCV
            cv::imwrite(filePath, image, {cv::IMWRITE_JPEG_QUALITY, 95});
        }
    }
}
