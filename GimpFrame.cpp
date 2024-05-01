//
// Created by ghisl on 01/05/2024.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "GimpFrame.h"
#include "Manipulation.h"

GimpFrame::GimpFrame(const wxString &title) : wxFrame(nullptr,
                                                      wxID_ANY, title,
                                                      wxDefaultPosition,
                                                      wxSize(1200, 600),
                                                      wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER) {

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
    rightPanSizer = new wxBoxSizer(wxVERTICAL);
    rightPanel->SetSizer(rightPanSizer);
    bitmap = nullptr;

    mainSizer->Add(leftPanel, 3, wxEXPAND | wxALL, 5);
    mainSizer->Add(rightPanel, 9, wxEXPAND | wxALL, 5);

    SetSizer(mainSizer);
    SetSizeHints(wxDefaultSize, wxDefaultSize, wxSize(1200, 600));

    wxMenuBar* menuBar = new wxMenuBar();
    wxMenu* fileMenu = new wxMenu();
    fileMenu->Append(wxID_ANY, "Load Image", "Load an image from file");
    menuBar->Append(fileMenu, "File");

    Bind(wxEVT_MENU, &GimpFrame::OnLoadImage, this, wxID_ANY);

    SetMenuBar(menuBar);

}

void GimpFrame::OnLoadImage(wxCommandEvent& event) {
    wxFileDialog dialog(this, "Select an image file", "", "", "Image files (*.jpg, *.jpeg, *.png)|*.jpg;*.jpeg;*.png", wxFD_OPEN);
    if (dialog.ShowModal() == wxID_OK) {
        mainImageMat = cv::imread(dialog.GetPath().ToStdString(), cv::IMREAD_COLOR);
        if (mainImageMat.empty()) {
            wxMessageBox("Failed to load image", "Error", wxOK | wxICON_ERROR);
            return;
        }
        displayImageMatToSizer();
    }
}

void GimpFrame::displayImageMatToSizer(){
    wxImage image = MatToWxImage(mainImageMat);

    int width = rightPanel->GetSize().GetWidth();
    int height = rightPanel->GetSize().GetHeight();
    double scaleFactor = std::min((double)width / image.GetWidth(), (double)height / image.GetHeight());
    int newWidth = image.GetWidth() * scaleFactor;
    int newHeight = image.GetHeight() * scaleFactor;
    image.Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);

    if (bitmap) {
        rightPanSizer->Detach(bitmap);
        delete bitmap;
        bitmap = nullptr;
    }

    bitmap = new wxStaticBitmap(rightPanel, wxID_ANY, wxBitmap(image));
    rightPanSizer->Add(bitmap, 1, wxALL | wxEXPAND, 5);

    rightPanel->Layout();
}

wxGridSizer* GimpFrame::CreateButtonGrid() {

    wxGridSizer* buttonGrid = new wxGridSizer(3, 3, 1, 1);

    wxButton* button1 = new wxButton(leftPanel, wxID_ANY, wxT("1"));
    int button1ID = 1;
    buttonGrid->Add(button1, 0, wxALL, 5);
    button1->Bind(wxEVT_BUTTON, [this, button1ID](wxCommandEvent& event) { OnButtonClicked(event, button1ID); });


    wxButton* button2 = new wxButton(leftPanel, wxID_ANY, wxT("2"));
    int button2ID = 2;
    buttonGrid->Add(button2, 0, wxALL, 5);
    button2->Bind(wxEVT_BUTTON, [this, button2ID](wxCommandEvent& event) { OnButtonClicked(event, button2ID); });


    wxButton* button3 = new wxButton(leftPanel, wxID_ANY, wxT("3"));
    int button3ID = 3;
    buttonGrid->Add(button3, 0, wxALL, 5);
    button3->Bind(wxEVT_BUTTON, [this, button3ID](wxCommandEvent& event) { OnButtonClicked(event, button3ID); });


    return buttonGrid;
}


void GimpFrame::CreateSubmenu1() {
    if (submenuPanel == nullptr) {
        return;
    }

    submenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(submenuPanel, wxID_ANY, wxT("Lighten / Darken submenu"));
    wxCheckBox* lightenCheckBox = new wxCheckBox(submenuPanel, wxID_ANY, wxT("Lighten"));
    wxCheckBox* darkenCheckBox = new wxCheckBox(submenuPanel, wxID_ANY, wxT("Darken"));
    wxSlider* factorSlider = new wxSlider(submenuPanel, wxID_ANY, 0, 0, 255);
    wxStaticText* factorLabel = new wxStaticText(submenuPanel, wxID_ANY, wxT("0"));
    wxButton* applyButton = new wxButton(submenuPanel, wxID_ANY, wxT("Apply"));
    wxButton* decreaseButton = new wxButton(submenuPanel, wxID_ANY, wxT("-"));
    wxButton* increaseButton = new wxButton(submenuPanel, wxID_ANY, wxT("+"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(lightenCheckBox, 0, wxALL, 5);
    sizer->Add(darkenCheckBox, 0, wxALL, 5);
    wxBoxSizer* factorSizer = new wxBoxSizer(wxHORIZONTAL);
    const int buttonWidth = 30;
    decreaseButton->SetMinSize(wxSize(buttonWidth, -1));
    increaseButton->SetMinSize(wxSize(buttonWidth, -1));
    factorSizer->Add(decreaseButton, 1, wxALL, 2);
    factorSizer->Add(factorSlider, 6, wxALL, 2);
    factorSizer->Add(increaseButton, 1, wxALL, 2);
    factorSizer->Add(factorLabel, 2, wxALL, 5);
    sizer->Add(factorSizer, 0, wxEXPAND, 0);
    sizer->Add(applyButton, 0, wxALL, 5);

    submenuPanel->SetSizer(sizer);
    submenuPanel->Layout();

    factorSlider->Bind(wxEVT_SCROLL_CHANGED, [factorLabel](wxScrollEvent& event) {
        factorLabel->SetLabel(wxString::Format(wxT("%d"), event.GetPosition()));
    });

    decreaseButton->Bind(wxEVT_BUTTON, [factorSlider, factorLabel](wxCommandEvent& event) {
        int value = factorSlider->GetValue();
        if (value > 0) {
            factorSlider->SetValue(value - 1);
        }
        factorLabel->SetLabel(wxString::Format(wxT("%d"), factorSlider->GetValue()));
    });

    increaseButton->Bind(wxEVT_BUTTON, [factorSlider, factorLabel](wxCommandEvent& event) {
        int value = factorSlider->GetValue();
        if (value < 255) {
            factorSlider->SetValue(value + 1);
        }
        factorLabel->SetLabel(wxString::Format(wxT("%d"), factorSlider->GetValue()));
    });

    applyButton->Bind(wxEVT_BUTTON, [this, lightenCheckBox, darkenCheckBox, factorSlider](wxCommandEvent& event) {
        OnApplyLightenDarken(lightenCheckBox->IsChecked(), darkenCheckBox->IsChecked(), factorSlider->GetValue());
    });
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

void GimpFrame::OnApplyLightenDarken(bool lighten, bool darken, int factor) {

    if (mainImageMat.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    if (!lighten && !darken) {
        wxMessageBox("Please select either Lighten or Darken.");
        return;
    }

    if (lighten && darken) {
        wxMessageBox("Please select only one of Lighten or Darken.");
        return;
    }

    int channels = mainImageMat.channels();
    int rows = mainImageMat.rows;
    int cols = mainImageMat.cols * channels;

    if (lighten) {
        for (int i = 0; i < rows; i++) {
            uchar* data = mainImageMat.ptr<uchar>(i);
            for (int j = 0; j < cols; j++) {
                data[j] = std::min(255, data[j] + factor);
            }
        }
    } else if (darken) {
        for (int i = 0; i < rows; i++) {
            uchar* data = mainImageMat.ptr<uchar>(i);
            for (int j = 0; j < cols; j++) {
                data[j] = std::max(0, data[j] + factor);
            }
        }
    }
    displayImageMatToSizer();
}


void GimpFrame::OnButtonClicked(wxCommandEvent& event, int buttonId) {
    switch (buttonId) {
        case 1:
            CreateSubmenu1();
            break;
        case 2:
            CreateSubmenu2();
            break;
        case 3:
            CreateSubmenu3();
            break;
        default:
            wxMessageBox("Invalid button ID");
            break;
    }
}