//
// Created by ghisl on 01/05/2024.
//

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include "GimpFrame.h"
#include "Manipulation.h"

GimpFrame::GimpFrame(const wxString &title) : wxFrame(nullptr, wxID_ANY, title,  wxDefaultPosition, wxSize(1200, 600)) {

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