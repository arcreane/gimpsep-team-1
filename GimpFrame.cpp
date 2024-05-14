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
    submenuPanel->SetMinSize(wxSize(300, 400));

    wxBoxSizer* leftPanelSizer = new wxBoxSizer(wxVERTICAL);
    leftPanelSizer->Add(buttonGrid, 0, wxEXPAND | wxALL, 5);
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
    wxWindowID loadImageID = wxNewId();
    fileMenu->Append(loadImageID, "Load Image", "Load an image from file");
    wxWindowID saveImageID = wxNewId();
    fileMenu->Append(saveImageID, "Save Image", "Save the modified image to a file");
    menuBar->Append(fileMenu, "File");

    Bind(wxEVT_MENU, &GimpFrame::OnLoadImage, this, loadImageID);
    Bind(wxEVT_MENU, &GimpFrame::OnSaveImage, this, saveImageID);

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

void GimpFrame::OnSaveImage(wxCommandEvent& event) {
    wxFileDialog dialog(this, "Save image as", "", "", "Image files (*.jpg, *.jpeg, *.png)|*.jpg;*.jpeg;*.png", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
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

        cv::imwrite(filePath, mainImageMat, {cv::IMWRITE_JPEG_QUALITY, 95});
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

    wxButton* button1 = new wxButton(leftPanel, wxID_ANY, wxT("Lighten/Darken"));
    int button1ID = 1;
    buttonGrid->Add(button1, 0, wxALL, 5);
    button1->Bind(wxEVT_BUTTON, [this, button1ID](wxCommandEvent& event) { OnButtonClicked(event, button1ID); });


    wxButton* button2 = new wxButton(leftPanel, wxID_ANY, wxT("Erode/Dilate"));
    int button2ID = 2;
    buttonGrid->Add(button2, 0, wxALL, 5);
    button2->Bind(wxEVT_BUTTON, [this, button2ID](wxCommandEvent& event) { OnButtonClicked(event, button2ID); });


    wxButton* button3 = new wxButton(leftPanel, wxID_ANY, wxT("Resize"));
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

    wxStaticText* text = new wxStaticText(submenuPanel, wxID_ANY, wxT("Erode / Dilate Options"));
    wxCheckBox* erodeCheckBox = new wxCheckBox(submenuPanel, wxID_ANY, wxT("Erode"));
    wxCheckBox* dilateCheckBox = new wxCheckBox(submenuPanel, wxID_ANY, wxT("Dilate"));
    wxSlider* kernelSizeSlider = new wxSlider(submenuPanel, wxID_ANY, 3, 1, 21, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
    wxButton* applyButton = new wxButton(submenuPanel, wxID_ANY, wxT("Apply"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(erodeCheckBox, 0, wxALL, 5);
    sizer->Add(dilateCheckBox, 0, wxALL, 5);
    sizer->Add(kernelSizeSlider, 0, wxEXPAND | wxALL, 5);
    sizer->Add(applyButton, 0, wxALL, 5);

    submenuPanel->SetSizer(sizer);
    submenuPanel->Layout();

    applyButton->Bind(wxEVT_BUTTON, [this, erodeCheckBox, dilateCheckBox, kernelSizeSlider](wxCommandEvent& event) {
        OnApplyErodeDilate(erodeCheckBox->IsChecked(), dilateCheckBox->IsChecked(), kernelSizeSlider->GetValue());
    });
}

void GimpFrame::CreateSubmenu3() {
    if (submenuPanel == nullptr) {
        return;
    }

    submenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(submenuPanel, wxID_ANY, wxT("Resize Options"));
    wxRadioButton* factorRadioButton = new wxRadioButton(submenuPanel, wxID_ANY, wxT("Resize by Factor"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxRadioButton* dimensionsRadioButton = new wxRadioButton(submenuPanel, wxID_ANY, wxT("Resize by Dimensions"));

    wxSlider* factorSlider = new wxSlider(submenuPanel, wxID_ANY, 1, 1, 10);
    wxStaticText* factorLabel = new wxStaticText(submenuPanel, wxID_ANY, wxT("1"));

    wxTextCtrl* widthCtrl = new wxTextCtrl(submenuPanel, wxID_ANY);
    wxTextCtrl* heightCtrl = new wxTextCtrl(submenuPanel, wxID_ANY);

    wxButton* applyButton = new wxButton(submenuPanel, wxID_ANY, wxT("Apply"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(factorRadioButton, 0, wxALL, 5);
    sizer->Add(factorSlider, 0, wxEXPAND | wxALL, 5);
    sizer->Add(factorLabel, 0, wxALL, 5);
    sizer->Add(dimensionsRadioButton, 0, wxALL, 5);
    sizer->Add(new wxStaticText(submenuPanel, wxID_ANY, wxT("Width")), 0, wxALL, 5);
    sizer->Add(widthCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(new wxStaticText(submenuPanel, wxID_ANY, wxT("Height")), 0, wxALL, 5);
    sizer->Add(heightCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(applyButton, 0, wxALL, 5);

    submenuPanel->SetSizer(sizer);
    submenuPanel->Layout();
    leftPanel->Layout();

    factorSlider->Bind(wxEVT_SCROLL_CHANGED, [factorLabel](wxScrollEvent& event) {
        factorLabel->SetLabel(wxString::Format(wxT("%d"), event.GetPosition()));
    });

    applyButton->Bind(wxEVT_BUTTON, [this, factorRadioButton, factorSlider, widthCtrl, heightCtrl](wxCommandEvent& event) {
        OnApplyResize(factorRadioButton->GetValue(), factorSlider->GetValue(), widthCtrl->GetValue(), heightCtrl->GetValue());
    });
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
                data[j] = std::max(0, data[j] - factor);
            }
        }
    }
    displayImageMatToSizer();
}

void GimpFrame::OnApplyErodeDilate(bool erode, bool dilate, int kernelSize) {
    if (mainImageMat.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    if (!erode && !dilate) {
        wxMessageBox("Please select either Erode or Dilate.");
        return;
    }

    if (erode && dilate) {
        wxMessageBox("Please select only one of Erode or Dilate.");
        return;
    }

    // Ensure the kernel size is odd
    if (kernelSize % 2 == 0) {
        kernelSize += 1;
    }

    cv::Mat result;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));

    if (erode) {
        cv::erode(mainImageMat, result, element);
    } else if (dilate) {
        cv::dilate(mainImageMat, result, element);
    }

    mainImageMat = result;
    displayImageMatToSizer();
}

void GimpFrame::OnApplyResize(bool resizeByFactor, int factor, const wxString& widthStr, const wxString& heightStr) {
    if (mainImageMat.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    cv::Mat resizedImage;
    if (resizeByFactor) {
        cv::resize(mainImageMat, resizedImage, cv::Size(), factor, factor);
    } else {
        int width = wxAtoi(widthStr);
        int height = wxAtoi(heightStr);
        if (width <= 0 || height <= 0) {
            wxMessageBox("Invalid dimensions.");
            return;
        }
        cv::resize(mainImageMat, resizedImage, cv::Size(width, height));
    }

    mainImageMat = resizedImage;
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
