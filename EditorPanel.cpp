#include "EditorPanel.h"
#include "MainFrame.h"
#include "Manipulation.h"
#include <opencv2/imgproc/imgproc.hpp>

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
    originalImage = inImage.clone();
    displayMainImageToPanel();
}

void EditorPanel::displayMainImageToPanel() {
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

    wxButton* button1 = new wxButton(menuPanel, wxID_ANY, wxT("Lighten/Darken"));
    int button1ID = 1;
    buttonGrid->Add(button1, 0, wxALL, 5);
    button1->Bind(wxEVT_BUTTON, [this, button1ID](wxCommandEvent& event) { onButtonClicked(event, button1ID); });

    wxButton* button2 = new wxButton(menuPanel, wxID_ANY, wxT("Erode/Dilate"));
    int button2ID = 2;
    buttonGrid->Add(button2, 0, wxALL, 5);
    button2->Bind(wxEVT_BUTTON, [this, button2ID](wxCommandEvent& event) { onButtonClicked(event, button2ID); });

    wxButton* button3 = new wxButton(menuPanel, wxID_ANY, wxT("Resize"));
    int button3ID = 3;
    buttonGrid->Add(button3, 0, wxALL, 5);
    button3->Bind(wxEVT_BUTTON, [this, button3ID](wxCommandEvent& event) { onButtonClicked(event, button3ID); });

    wxButton* button4 = new wxButton(menuPanel, wxID_ANY, wxT("Canny"));
    int button4ID = 4;
    buttonGrid->Add(button4, 0, wxALL, 5);
    button4->Bind(wxEVT_BUTTON, [this, button4ID](wxCommandEvent& event) { onButtonClicked(event, button4ID); });

    wxButton* button5 = new wxButton(menuPanel, wxID_ANY, wxT("Black and white"));
    int button5ID = 5;
    buttonGrid->Add(button5, 0, wxALL, 5);
    button5->Bind(wxEVT_BUTTON, [this, button5ID](wxCommandEvent& event) { onButtonClicked(event, button5ID); });

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
            createLightenDarkenSubmenu();
            break;
        case 2:
            createErodeDilateSubmenu();
            break;
        case 3:
            createResizeSubmenu();
            break;
        case 4:
            createCannySubmenu();
            break;
        case 5:
            createBlackWhiteSubmenu();
            break;
        default:
            wxMessageBox("Invalid button ID");
            break;
    }
}

void EditorPanel::createLightenDarkenSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Adjust Brightness (-100 to +100)"));
    wxSlider* brightnessSlider = new wxSlider(subMenuPanel, wxID_ANY, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
    wxStaticText* sliderValueDisplay = new wxStaticText(subMenuPanel, wxID_ANY, wxT("0"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

    wxButton* confirmButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Confirm"));
    wxButton* cancelButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Cancel"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(brightnessSlider, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sliderValueDisplay, 0, wxALL | wxEXPAND, 5);
    sizer->Add(confirmButton, 0, wxALL | wxEXPAND, 5);
    sizer->Add(cancelButton, 0, wxALL | wxEXPAND, 5);

    subMenuPanel->SetSizer(sizer);
    subMenuPanel->Layout();

    brightnessSlider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, [this, brightnessSlider, sliderValueDisplay](wxCommandEvent& event) {
        int value = brightnessSlider->GetValue();
        sliderValueDisplay->SetLabel(wxString::Format(wxT("%d"), value));
        onApplyLightenDarken(value);
    });

    confirmButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        originalImage = mainImage.clone(); // Confirm changes
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Revert changes
        displayMainImageToPanel();
    });
}

void EditorPanel::createErodeDilateSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Erode / Dilate Options"));
    wxRadioButton* erodeRadioButton = new wxRadioButton(subMenuPanel, wxID_ANY, wxT("Erode"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxRadioButton* dilateRadioButton = new wxRadioButton(subMenuPanel, wxID_ANY, wxT("Dilate"));
    wxSlider* kernelSizeSlider = new wxSlider(subMenuPanel, wxID_ANY, 3, 1, 21, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    wxButton* confirmButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Confirm"));
    wxButton* cancelButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Cancel"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(erodeRadioButton, 0, wxALL, 5);
    sizer->Add(dilateRadioButton, 0, wxALL, 5);
    sizer->Add(kernelSizeSlider, 0, wxEXPAND | wxALL, 5);
    sizer->Add(confirmButton, 0, wxALL | wxEXPAND, 5);
    sizer->Add(cancelButton, 0, wxALL | wxEXPAND, 5);

    subMenuPanel->SetSizer(sizer);
    subMenuPanel->Layout();

    // Bind slider and radio buttons for real-time update
    auto updateFunction = [this, erodeRadioButton, dilateRadioButton, kernelSizeSlider](wxCommandEvent& event) {
        this->onApplyErodeDilate(erodeRadioButton->GetValue(), dilateRadioButton->GetValue(), kernelSizeSlider->GetValue());
    };

    kernelSizeSlider->Bind(wxEVT_SCROLL_THUMBTRACK, updateFunction);
    erodeRadioButton->Bind(wxEVT_RADIOBUTTON, updateFunction);
    dilateRadioButton->Bind(wxEVT_RADIOBUTTON, updateFunction);

    confirmButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        originalImage = mainImage.clone(); // Confirm changes
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Revert changes
        displayMainImageToPanel();
    });
}

void EditorPanel::createResizeSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Resize Options"));
    wxRadioButton* factorRadioButton = new wxRadioButton(subMenuPanel, wxID_ANY, wxT("Resize by Factor"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxRadioButton* dimensionsRadioButton = new wxRadioButton(subMenuPanel, wxID_ANY, wxT("Resize by Dimensions"));

    wxSlider* factorSlider = new wxSlider(subMenuPanel, wxID_ANY, 1, 1, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
    wxTextCtrl* widthCtrl = new wxTextCtrl(subMenuPanel, wxID_ANY);
    wxTextCtrl* heightCtrl = new wxTextCtrl(subMenuPanel, wxID_ANY);

    wxButton* confirmButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Confirm"));
    wxButton* cancelButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Cancel"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(factorRadioButton, 0, wxALL, 5);
    sizer->Add(factorSlider, 0, wxEXPAND | wxALL, 5);
    sizer->Add(dimensionsRadioButton, 0, wxALL, 5);
    sizer->Add(new wxStaticText(subMenuPanel, wxID_ANY, wxT("Width")), 0, wxALL, 5);
    sizer->Add(widthCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(new wxStaticText(subMenuPanel, wxID_ANY, wxT("Height")), 0, wxALL, 5);
    sizer->Add(heightCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(confirmButton, 0, wxALL | wxEXPAND, 5);
    sizer->Add(cancelButton, 0, wxALL | wxEXPAND, 5);

    subMenuPanel->SetSizer(sizer);
    subMenuPanel->Layout();
    menuPanel->Layout();

    factorSlider->Bind(wxEVT_SCROLL_THUMBTRACK, [this, factorRadioButton, widthCtrl, heightCtrl, factorSlider](wxScrollEvent& event) {
        if (factorRadioButton->GetValue()) {
            onApplyResize(true, factorSlider->GetValue(), "", "");
        }
    });

    widthCtrl->Bind(wxEVT_TEXT, [this, factorRadioButton, widthCtrl, heightCtrl](wxCommandEvent& event) {
        if (!factorRadioButton->GetValue()) {
            onApplyResize(false, 1, widthCtrl->GetValue(), heightCtrl->GetValue());
        }
    });

    heightCtrl->Bind(wxEVT_TEXT, [this, factorRadioButton, widthCtrl, heightCtrl](wxCommandEvent& event) {
        if (!factorRadioButton->GetValue()) {
            onApplyResize(false, 1, widthCtrl->GetValue(), heightCtrl->GetValue());
        }
    });

    confirmButton->Bind(wxEVT_BUTTON, [this, factorRadioButton, factorSlider, widthCtrl, heightCtrl](wxCommandEvent& event) {
        onApplyResize(factorRadioButton->GetValue(), factorSlider->GetValue(), widthCtrl->GetValue(), heightCtrl->GetValue());
        originalImage = mainImage.clone(); // Confirm changes
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Revert changes
        displayMainImageToPanel();
    });
}

void EditorPanel::createCannySubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Canny Edge Detection"));
    wxTextCtrl* lowThresholdCtrl = new wxTextCtrl(subMenuPanel, wxID_ANY);
    wxTextCtrl* highThresholdCtrl = new wxTextCtrl(subMenuPanel, wxID_ANY);
    wxSlider* kernelSizeSlider = new wxSlider(subMenuPanel, wxID_ANY, 3, 1, 7, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    wxButton* confirmButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Confirm"));
    wxButton* cancelButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Cancel"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(new wxStaticText(subMenuPanel, wxID_ANY, wxT("Low Threshold")), 0, wxALL, 5);
    sizer->Add(lowThresholdCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(new wxStaticText(subMenuPanel, wxID_ANY, wxT("High Threshold")), 0, wxALL, 5);
    sizer->Add(highThresholdCtrl, 0, wxEXPAND | wxALL, 5);
    sizer->Add(new wxStaticText(subMenuPanel, wxID_ANY, wxT("Kernel Size (Only odd values)")), 0, wxALL, 5);
    sizer->Add(kernelSizeSlider, 0, wxEXPAND | wxALL, 5);
    sizer->Add(confirmButton, 0, wxALL | wxEXPAND, 5);
    sizer->Add(cancelButton, 0, wxALL | wxEXPAND, 5);

    subMenuPanel->SetSizer(sizer);
    subMenuPanel->Layout();
    menuPanel->Layout();

    // Bind events for real-time update
    lowThresholdCtrl->Bind(wxEVT_TEXT, [this, lowThresholdCtrl, highThresholdCtrl, kernelSizeSlider](wxCommandEvent& event) {
        onApplyCanny(wxAtol(lowThresholdCtrl->GetValue()), wxAtol(highThresholdCtrl->GetValue()), kernelSizeSlider->GetValue());
    });

    highThresholdCtrl->Bind(wxEVT_TEXT, [this, lowThresholdCtrl, highThresholdCtrl, kernelSizeSlider](wxCommandEvent& event) {
        onApplyCanny(wxAtol(lowThresholdCtrl->GetValue()), wxAtol(highThresholdCtrl->GetValue()), kernelSizeSlider->GetValue());
    });

    kernelSizeSlider->Bind(wxEVT_SCROLL_THUMBTRACK, [this, lowThresholdCtrl, highThresholdCtrl, kernelSizeSlider](wxScrollEvent& event) {
        onApplyCanny(wxAtol(lowThresholdCtrl->GetValue()), wxAtol(highThresholdCtrl->GetValue()), kernelSizeSlider->GetValue());
    });

    confirmButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        originalImage = mainImage.clone(); // Confirm changes
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Revert changes
        displayMainImageToPanel();
    });
}

void EditorPanel::createBlackWhiteSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren();

    wxStaticText* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Adjust Black and White Intensity (0 to 255)"));
    wxSlider* bwSlider = new wxSlider(subMenuPanel, wxID_ANY, 128, 0, 255, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
    wxStaticText* sliderValueDisplay = new wxStaticText(subMenuPanel, wxID_ANY, wxT("128"), wxDefaultPosition, wxDefaultSize, wxALIGN_CENTER_HORIZONTAL);

    wxButton* confirmButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Confirm"));
    wxButton* cancelButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Cancel"));

    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(bwSlider, 0, wxEXPAND | wxALL, 5);
    sizer->Add(sliderValueDisplay, 0, wxALL | wxEXPAND, 5);
    sizer->Add(confirmButton, 0, wxALL | wxEXPAND, 5);
    sizer->Add(cancelButton, 0, wxALL | wxEXPAND, 5);

    subMenuPanel->SetSizer(sizer);
    subMenuPanel->Layout();

    bwSlider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, [this, bwSlider, sliderValueDisplay](wxCommandEvent& event) {
        int value = bwSlider->GetValue();
        sliderValueDisplay->SetLabel(wxString::Format(wxT("%d"), value));
        onApplyBlackWhite(value);
    });

    confirmButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        originalImage = mainImage.clone(); // Confirm changes
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Revert changes
        displayMainImageToPanel();
    });
}

void EditorPanel::onApplyLightenDarken(int adjustment) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    cv::Mat tempImage = originalImage.clone();

    cv::Mat tempImageYCrCb;
    cv::cvtColor(tempImage, tempImageYCrCb, cv::COLOR_BGR2YCrCb);

    for (int i = 0; i < tempImageYCrCb.rows; i++) {
        for (int j = 0; j < tempImageYCrCb.cols; j++) {
            uchar& Y = tempImageYCrCb.at<cv::Vec3b>(i, j)[0];
            int newY = Y + adjustment;
            Y = cv::saturate_cast<uchar>(newY);
        }
    }

    cv::cvtColor(tempImageYCrCb, mainImage, cv::COLOR_YCrCb2BGR);

    displayMainImageToPanel();
}

void EditorPanel::onApplyErodeDilate(bool erode, bool dilate, int kernelSize) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    if (kernelSize % 2 == 0) kernelSize += 1;

    cv::Mat result;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));

    if (erode) {
        cv::erode(originalImage, result, element);
    } else if (dilate) {
        cv::dilate(originalImage, result, element);
    }

    mainImage = result;
    displayMainImageToPanel();
}

void EditorPanel::onApplyResize(bool resizeByFactor, int factor, const wxString& widthStr, const wxString& heightStr) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    if ((resizeByFactor && factor == 1) || (!resizeByFactor && (wxAtoi(widthStr) == 0 || wxAtoi(heightStr) == 0))) {
        // Reset to original image if no resizing is needed
        mainImage = originalImage.clone();
        displayMainImageToPanel();
        return;
    }

    cv::Mat resizedImage;
    if (resizeByFactor) {
        cv::resize(originalImage, resizedImage, cv::Size(), factor, factor);
    } else {
        int width = wxAtoi(widthStr);
        int height = wxAtoi(heightStr);
        if (width <= 0 || height <= 0) {
            wxMessageBox("Invalid dimensions.");
            return;
        }
        cv::resize(originalImage, resizedImage, cv::Size(width, height));
    }

    mainImage = resizedImage;
    displayMainImageToPanel();
}

void EditorPanel::onApplyCanny(long lowThreshold, long highThreshold, int kernelSize) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    if (kernelSize % 2 == 0) {
        kernelSize += 1;
    }

    if (lowThreshold == 0 && highThreshold == 0) {
        mainImage = originalImage.clone();
        displayMainImageToPanel();
    } else {
        cv::Mat edges;
        cv::Canny(mainImage, edges, lowThreshold, highThreshold, kernelSize);
        mainImage = edges;
        displayMainImageToPanel();
    }
}

void EditorPanel::onApplyBlackWhite(int intensity) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }
    cv::Mat grayImage;
    cv::cvtColor(mainImage, grayImage, cv::COLOR_BGR2GRAY);
    cv::Mat bwImage;
    cv::threshold(grayImage, bwImage, intensity, 255, cv::THRESH_BINARY);

    cv::cvtColor(bwImage, mainImage, cv::COLOR_GRAY2BGR);
    displayMainImageToPanel();
}

void EditorPanel::onApplyOpenCVFunction() {
    // Soit faire cv::Mat newImage = cv::AppliquerFonction(parametres)
    // mainImage = newImage;
    // Soit mainImage = cv::AppliquerFonction(params)
    displayMainImageToPanel();
}
