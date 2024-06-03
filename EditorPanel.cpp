//
// Created by ghisl on 08/05/2024.
//

#include "EditorPanel.h"
#include "MainFrame.h"
#include "Manipulation.h"

EditorPanel::EditorPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    menuPanel = new wxPanel(this, wxID_ANY);
    menuPanel->SetBackgroundColour(*wxLIGHT_GREY);

    auto buttonGrid = createButtonGrid();

    subMenuPanel = new wxPanel(menuPanel, wxID_ANY);
    subMenuPanel->SetBackgroundColour(*wxWHITE);

    menuPanelSizer = new wxBoxSizer(wxVERTICAL);
    menuPanelSizer->Add(buttonGrid, 1, wxEXPAND | wxALL, 1);
    menuPanelSizer->Add(subMenuPanel, 1, wxEXPAND | wxALL, 2);

    menuPanel->SetSizer(menuPanelSizer);

    imagePanel = new wxPanel(this, wxID_ANY);
    imagePanel->SetBackgroundColour(*wxWHITE);
    imagePanSizer = new wxBoxSizer(wxVERTICAL);
    imagePanel->SetSizer(imagePanSizer);
    imageBitmap = nullptr;

    mainSizer->Add(menuPanel, 3, wxEXPAND | wxALL, 1);
    mainSizer->Add(imagePanel, 9, wxEXPAND | wxALL, 1);

    SetSizer(mainSizer);
    SetSizeHints(wxDefaultSize, wxDefaultSize, wxSize(1200, 600));

    SetBackgroundColour(*wxWHITE);

}

void EditorPanel::setImage(cv::Mat inImage) {
    mainImage = inImage;
}

void EditorPanel::displayMainImageToPanel(){
    wxImage image = cvMatToWxImage(mainImage);

    int width = imagePanel->GetSize().GetWidth();
    int height = imagePanel->GetSize().GetHeight();
    double scaleFactor = std::min((double)width / image.GetWidth(), (double)height / image.GetHeight());
    int newWidth = image.GetWidth() * scaleFactor;
    int newHeight = image.GetHeight() * scaleFactor;
    image.Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);

    if (imageBitmap) {
        imagePanSizer->Detach(imageBitmap);
        delete imageBitmap;
        imageBitmap = nullptr;
    }

    imageBitmap = new wxStaticBitmap(imagePanel, wxID_ANY, wxBitmap(image));
    imagePanSizer->Add(imageBitmap, 1, wxALL | wxEXPAND, 5);
    imagePanel->Layout();
}

wxGridSizer * EditorPanel::createButtonGrid() {
    wxGridSizer* buttonGrid = new wxGridSizer(3, 3, 1, 1);

    wxButton* button1 = new wxButton(menuPanel, wxID_ANY, wxT("Lighten"));
    int button1ID = 1;
    buttonGrid->Add(button1, 0, wxALL, 5);
    button1->Bind(wxEVT_BUTTON, [this, button1ID](wxCommandEvent& event) { onButtonClicked(event, button1ID); });

    wxButton* button2 = new wxButton(menuPanel, wxID_ANY, wxT("2"));
    int button2ID = 2;
    buttonGrid->Add(button2, 0, wxALL, 5);
    button2->Bind(wxEVT_BUTTON, [this, button2ID](wxCommandEvent& event) { onButtonClicked(event, button2ID); });


    wxButton* button3 = new wxButton(menuPanel, wxID_ANY, wxT("3"));
    int button3ID = 3;
    buttonGrid->Add(button3, 0, wxALL, 5);
    button3->Bind(wxEVT_BUTTON, [this, button3ID](wxCommandEvent& event) { onButtonClicked(event, button3ID); });


    return buttonGrid;
}

cv::Mat EditorPanel::getImage() {
    return mainImage;
}

void EditorPanel::onButtonClicked(wxCommandEvent &event, int buttonId) {
    if (subMenuPanel == nullptr) {
        return;
    }
    subMenuPanel->DestroyChildren();

    switch (buttonId) {
        case 1:
            createLightenSubmenu();
            break;
        case 2:
            createSubmenu2();
            break;
        case 3:
            createSubmenu3();
            break;
            //Mettre les 9 autres cas
        default:
            wxMessageBox("Invalid button ID");
            break;
    }
}

void EditorPanel::createLightenSubmenu() {
    wxButton* applyButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Lighten"));
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(applyButton, 0, wxALL, 5);

    applyButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        onApplyLighten();
    });

    /*
    OU POUR PASSER DES COMPOSANTS DU SUBMENU
     applyButton->Bind(wxEVT_BUTTON, [this, lightenCheckBox, darkenCheckBox, factorSlider](wxCommandEvent& event) {
        OnApplyLightenDarken(lightenCheckBox->IsChecked(), darkenCheckBox->IsChecked(), factorSlider->GetValue());
    });
    */

}

void EditorPanel::createSubmenu2() {

}

void EditorPanel::createSubmenu3() {

}

void EditorPanel::onApplyLighten() {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    int channels = mainImage.channels();
    int rows = mainImage.rows;
    int cols = mainImage.cols * channels;

    for (int i = 0; i < rows; i++) {
        uchar* data = mainImage.ptr<uchar>(i);
        for (int j = 0; j < cols; j++) {
            data[j] = std::min(255, data[j] + 5);
        }
    }
    displayMainImageToPanel();

}

void EditorPanel::onApplyOpenCVFunction() {
    //Soit faire cv::Mat newImage = cv::AppliquerFonction(parametres)
    //mainImage = newImage;
    //Soit mainImage = cv::AppliquerFonction(params)
    displayMainImageToPanel();
}
