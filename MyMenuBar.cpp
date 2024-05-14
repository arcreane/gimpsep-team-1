//
// Created by ghisl on 14/05/2024.
//

#include "MyMenuBar.h"
#include "MainFrame.h"

MyMenuBar::MyMenuBar(wxPanel* panel) {

    contentPanel = panel;
    auto* menuMenu = new wxMenu();
    menuMenu->Append(0, wxT("Main menu"));
    auto* lsMenu = new wxMenu();
    lsMenu->Append(1, wxT("Load Image"));
    lsMenu->Append(2, wxT("Save Image"));

    this->Append(menuMenu, wxT("Menu"));
    this->Append(lsMenu, wxT("Image IO"));

    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onMainMenuClk, this, 0);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onLoadImageClk, this, 1);
    this->Bind(wxEVT_COMMAND_MENU_SELECTED, &MyMenuBar::onSaveImageClk, this, 2);

}

void MyMenuBar::onMainMenuClk(wxCommandEvent &event) {
    dynamic_cast<MainFrame*>(GetParent())->loadMenuPanel();
}

void MyMenuBar::onLoadImageClk(wxCommandEvent &event) {

    cv::Mat image;
    wxFileDialog dialog(this, "Select an image file", "", "", "Image files (*.jpg, *.jpeg, *.png)|*.jpg;*.jpeg;*.png", wxFD_OPEN);

    switch (dynamic_cast<MainFrame*>(GetParent())->getCurrentPanel()) {
        case 0:
            wxMessageBox("You may not load an image here.", "Invalid location", wxOK | wxICON_INFORMATION);
            break;
        case 1:
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
            //Add image to stitcher list
            if (dialog.ShowModal() == wxID_OK) {
                image = cv::imread(dialog.GetPath().ToStdString(), cv::IMREAD_COLOR);
                if (image.empty()) {
                    wxMessageBox("Failed to load image", "Error", wxOK | wxICON_ERROR);
                    return;
                }
            }
            break;
    }
}

void MyMenuBar::onSaveImageClk(wxCommandEvent &event) {

    wxFileDialog dialog(this, "Save image as", "", "", "Image files (*.jpg, *.jpeg, *.png)|*.jpg;*.jpeg;*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    cv::Mat image;

    switch (dynamic_cast<MainFrame*>(GetParent())->getCurrentPanel()) {
        case 0:
            wxMessageBox("You may not load an image here.", "Invalid location", wxOK | wxICON_INFORMATION);
            break;
        case 1:
            image = dynamic_cast<MainFrame*>(GetParent())->getEditedImage();
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
                size_t lastDotPos = filePath.find_last_of('.');
                if (lastDotPos != std::string::npos && lastDotPos < filePath.size() - 1) {
                    filePath.replace(lastDotPos + 1, filePath.size() - lastDotPos - 1, fileExt);
                } else {
                    filePath += "." + fileExt;
                }
                cv::imwrite(filePath, image, {cv::IMWRITE_JPEG_QUALITY, 95});
            }
        case 2:
            //Save stitched image

            break;
    }


}
