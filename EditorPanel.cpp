#include "EditorPanel.h"
#include "Manipulation.h"
#include <opencv2/imgproc/imgproc.hpp>

// Initializes the elements of the graphical interface and organizes the different panels
EditorPanel::EditorPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {

    auto* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // Creates the menu panel and sets its background color
    menuPanel = new wxPanel(this, wxID_ANY);
    menuPanel->SetBackgroundColour(*wxLIGHT_GREY);

    // Creates the grid of buttons in the menu panel
    auto buttonGrid = createButtonGrid();

    // Creates the sub-menu panel within the menu panel
    subMenuPanel = new wxPanel(menuPanel, wxID_ANY);
    subMenuPanel->SetBackgroundColour(*wxWHITE);

    // Organizes the layout of the menu panel with the button grid and sub-menu panel
    menuPanelSizer = new wxBoxSizer(wxVERTICAL);
    menuPanelSizer->Add(buttonGrid, 1, wxEXPAND | wxALL, 1);
    menuPanelSizer->Add(subMenuPanel, 1, wxEXPAND | wxALL, 2);
    menuPanel->SetSizer(menuPanelSizer);

    // Creates the image panel and set its background color
    imagePanel = new wxPanel(this, wxID_ANY);
    imagePanel->SetBackgroundColour(*wxWHITE);
    imagePanSizer = new wxBoxSizer(wxVERTICAL);
    imagePanel->SetSizer(imagePanSizer);
    imageBitmap = nullptr;

    // Adding the menu and image panels to the main sizer
    mainSizer->Add(menuPanel, 3, wxEXPAND | wxALL, 1);
    mainSizer->Add(imagePanel, 9, wxEXPAND | wxALL, 1);

    // Sets the sizer for the main panel and define size hints
    SetSizer(mainSizer);
    SetSizeHints(wxDefaultSize, wxDefaultSize, wxSize(1200, 600));

}

// Loads an image and saves it to get back the original settings
void EditorPanel::setImage(const cv::Mat& inImage) {
    mainImage = inImage;
    originalImage = inImage.clone(); // Saves the original image
    displayMainImageToPanel();
}

cv::Mat EditorPanel::getImage() {
    return mainImage;
}

void EditorPanel::displayMainImageToPanel() {
    wxImage image = cvMatToWxImage(mainImage); // Converts the OpenCV image to wxImage

    int width = imagePanel->GetSize().GetWidth();
    int height = imagePanel->GetSize().GetHeight();
    double scaleFactor = std::min((double)width / image.GetWidth(), (double)height / image.GetHeight());
    int newWidth = image.GetWidth() * scaleFactor;
    int newHeight = image.GetHeight() * scaleFactor;
    image.Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH); // Rescales the image to fit into the panel

    if (imageBitmap) {
        imagePanSizer->Detach(imageBitmap);
        delete imageBitmap;
        imageBitmap = nullptr;
    }

    // Creates and adds a wxStaticBitmap to display the image
    imageBitmap = new wxStaticBitmap(imagePanel, wxID_ANY, wxBitmap(image));
    imagePanSizer->Add(imageBitmap, 1, wxALL | wxEXPAND, 5);
    imagePanel->Layout();
}


wxGridSizer * EditorPanel::createButtonGrid() {

    auto* buttonGrid = new wxGridSizer(3, 3, 1, 1);

    auto* button1 = new wxButton(menuPanel, wxID_ANY, wxT("Lighten/Darken"));

    int button1ID = 1;
    buttonGrid->Add(button1, 0, wxEXPAND | wxALL, 3);
    button1->Bind(wxEVT_BUTTON, [this, button1ID](wxCommandEvent& event) { onButtonClicked(event, button1ID); });

    auto* button2 = new wxButton(menuPanel, wxID_ANY, wxT("Erode/Dilate"));
    int button2ID = 2;
    buttonGrid->Add(button2, 0, wxEXPAND | wxALL, 3);
    button2->Bind(wxEVT_BUTTON, [this, button2ID](wxCommandEvent& event) { onButtonClicked(event, button2ID); });

    auto* button3 = new wxButton(menuPanel, wxID_ANY, wxT("Resize"));

    int button3ID = 3;
    buttonGrid->Add(button3, 0, wxEXPAND | wxALL, 3);
    button3->Bind(wxEVT_BUTTON, [this, button3ID](wxCommandEvent& event) { onButtonClicked(event, button3ID); });

    auto* button4 = new wxButton(menuPanel, wxID_ANY, wxT("Canny edge\ndetection"));
    int button4ID = 4;
    buttonGrid->Add(button4, 0, wxEXPAND | wxALL, 3);
    button4->Bind(wxEVT_BUTTON, [this, button4ID](wxCommandEvent& event) { onButtonClicked(event, button4ID); });

    auto* button5 = new wxButton(menuPanel, wxID_ANY, wxT("Grayscale"));
    int button5ID = 5;
    buttonGrid->Add(button5, 0, wxEXPAND | wxALL, 3);
    button5->Bind(wxEVT_BUTTON, [this, button5ID](wxCommandEvent& event) { onButtonClicked(event, button5ID); });

    auto* button6 = new wxButton(menuPanel, wxID_ANY, wxT("Black and white"));
    int button6ID = 6;
    buttonGrid->Add(button6, 0, wxEXPAND | wxALL, 3);
    button6->Bind(wxEVT_BUTTON, [this, button6ID](wxCommandEvent& event) { onButtonClicked(event, button6ID); });

    return buttonGrid;
}

// Returns the principal image that is in the editor panel
cv::Mat EditorPanel::getImage() {
    return mainImage;
}

// Handles button clicks by destroying the sub-menus and creating new ones when a button is clicked
void EditorPanel::onButtonClicked(wxCommandEvent &event, int buttonId) {
    if (subMenuPanel == nullptr) {
        return;
    }
    subMenuPanel->DestroyChildren(); // Clear existing sub-menu content

    // Creates the appropriate sub-menu based on the button clicked
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
            createGrayscaleSubmenu();
            break;
      case 6:
            createBlackWhiteSubmenu();
            break;
        default:
            wxMessageBox("Invalid button ID");
            break;
    }
}

// Creates the sub-menu for adjusting brightness
void EditorPanel::createLightenDarkenSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren(); // Clears existing sub-menu content


  // Creates UI elements for the brightness adjustment sub-menu
    auto* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Adjust Brightness (-100 to +100)"));
    auto* brightnessSlider = new wxSlider(subMenuPanel, wxID_ANY, 0, -100, 100, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(brightnessSlider, 0, wxEXPAND | wxALL, 5);

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


    brightnessSlider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, [this, brightnessSlider](wxCommandEvent& event) {
        int value = brightnessSlider->GetValue();
        onApplyLightenDarken(value);

    });

    confirmButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        originalImage = mainImage.clone(); // Confirms changes
        wxMessageBox("Your modifications have been updated", "Confirmation", wxOK | wxICON_INFORMATION);
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Reverts changes
        displayMainImageToPanel();
    });
}

// Creates the sub-menu for erode/dilate operations
void EditorPanel::createErodeDilateSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren(); // Clears existing sub-menu content


  // Create UI elements for the erode/dilate sub-menu
    auto* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Erode / Dilate Options"));
    auto* erodeRadioButton = new wxRadioButton(subMenuPanel, wxID_ANY, wxT("Erode"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    auto* dilateRadioButton = new wxRadioButton(subMenuPanel, wxID_ANY, wxT("Dilate"));
    auto* kernelSizeSlider = new wxSlider(subMenuPanel, wxID_ANY, 3, 1, 21, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    auto* sizer = new wxBoxSizer(wxVERTICAL);
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

    // Binds slider and radio buttons for real-time update
    auto updateFunction = [this, erodeRadioButton, dilateRadioButton, kernelSizeSlider](wxCommandEvent& event) {
        this->onApplyErodeDilate(erodeRadioButton->GetValue(), dilateRadioButton->GetValue(), kernelSizeSlider->GetValue());
    };

    kernelSizeSlider->Bind(wxEVT_SCROLL_THUMBTRACK, updateFunction);
    erodeRadioButton->Bind(wxEVT_RADIOBUTTON, updateFunction);
    dilateRadioButton->Bind(wxEVT_RADIOBUTTON, updateFunction);

    confirmButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        originalImage = mainImage.clone(); // Confirms changes
        wxMessageBox("Your modifications have been updated", "Confirmation", wxOK | wxICON_INFORMATION);
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Reverts changes
        displayMainImageToPanel();
    });
}

// Creates the sub-menu for resizing the image
void EditorPanel::createResizeSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren(); // Clears existing sub-menu content


    // Creates UI elements for the resize sub-menu
    wxStaticText* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Resize Options"));
    wxRadioButton* factorRadioButton = new wxRadioButton(subMenuPanel, wxID_ANY, wxT("Resize by Factor"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP);
    wxRadioButton* dimensionsRadioButton = new wxRadioButton(subMenuPanel, wxID_ANY, wxT("Resize by Dimensions"));

    wxSlider* factorSlider = new wxSlider(subMenuPanel, wxID_ANY, 1, 1, 10, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);
    wxTextCtrl* widthCtrl = new wxTextCtrl(subMenuPanel, wxID_ANY);
    wxTextCtrl* heightCtrl = new wxTextCtrl(subMenuPanel, wxID_ANY);

    wxButton* confirmButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Confirm"));
    wxButton* cancelButton = new wxButton(subMenuPanel, wxID_ANY, wxT("Cancel"));

    auto* sizer = new wxBoxSizer(wxVERTICAL);
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

    // Binds events for the factor slider and text controls for real-time updates
    factorSlider->Bind(wxEVT_SCROLL_THUMBTRACK, [this, factorRadioButton, factorSlider](wxScrollEvent& event) {
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

    // Binds confirm and cancel buttons
    confirmButton->Bind(wxEVT_BUTTON, [this, factorRadioButton, factorSlider, widthCtrl, heightCtrl](wxCommandEvent& event) {
        onApplyResize(factorRadioButton->GetValue(), factorSlider->GetValue(), widthCtrl->GetValue(), heightCtrl->GetValue());
        originalImage = mainImage.clone(); // Confirms changes
        wxMessageBox("Your modifications have been updated", "Confirmation", wxOK | wxICON_INFORMATION);
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Reverts changes
        displayMainImageToPanel();
    });
}

// Creates the sub-menu for Canny edge detection
void EditorPanel::createCannySubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren(); // Clears existing sub-menu content

    // Creates UI elements for the Canny edge detection sub-menu
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

    // Binds events for real-time update
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
        wxMessageBox("Your modifications have been updated", "Confirmation", wxOK | wxICON_INFORMATION);
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Reverts changes
        displayMainImageToPanel();
    });
}

// Creates the sub-menu for black and white conversion
void EditorPanel::createBlackWhiteSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren(); // Clears existing sub-menu content

    // Creates UI elements for the black and white conversion sub-menu
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

    // Binds events for the black and white slider
    bwSlider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, [this, bwSlider, sliderValueDisplay](wxCommandEvent& event) {
        int value = bwSlider->GetValue();
        sliderValueDisplay->SetLabel(wxString::Format(wxT("%d"), value));
        onApplyBlackWhite(value); // Apply changes in real-time
    });

    confirmButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        originalImage = mainImage.clone(); // Confirm changes
        wxMessageBox("Your modifications have been updated", "Confirmation", wxOK | wxICON_INFORMATION);
    });

    cancelButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) {
        mainImage = originalImage.clone(); // Revert changes
        displayMainImageToPanel();
    });
}


void EditorPanel::createGrayscaleSubmenu() {
    if (subMenuPanel == nullptr) {
        return;
    }

    subMenuPanel->DestroyChildren();

    auto* text = new wxStaticText(subMenuPanel, wxID_ANY, wxT("Set number or grayscale levels (2-21)"));
    auto* grayscaleSlider = new wxSlider(subMenuPanel, wxID_ANY, 12, 2, 22, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_LABELS);

    auto* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(text, 0, wxALL, 5);
    sizer->Add(grayscaleSlider, 0, wxEXPAND | wxALL, 5);

    subMenuPanel->SetSizer(sizer);
    subMenuPanel->Layout();

    grayscaleSlider->Bind(wxEVT_COMMAND_SLIDER_UPDATED, [this, grayscaleSlider](wxCommandEvent& event) {
        int value = grayscaleSlider->GetValue();
        onApplyGrayscale(value);
    });
}


// Adjusts the luminosity of the image
void EditorPanel::onApplyLightenDarken(int adjustment) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    cv::Mat tempImage = originalImage.clone();

    cv::Mat tempImageYCrCb;
    cv::cvtColor(tempImage, tempImageYCrCb, cv::COLOR_BGR2YCrCb); // Converts the image to YCrCb color space

    // Adjusts the Y channel (brightness) by the specified adjustment value
    for (int i = 0; i < tempImageYCrCb.rows; i++) {
        for (int j = 0; j < tempImageYCrCb.cols; j++) {
            uchar& Y = tempImageYCrCb.at<cv::Vec3b>(i, j)[0];
            int newY = Y + adjustment;
            Y = cv::saturate_cast<uchar>(newY);
        }
    }

    cv::cvtColor(tempImageYCrCb, mainImage, cv::COLOR_YCrCb2BGR); // Converts the image back to BGR color space

    displayMainImageToPanel(); // Updates the displayed image
}

// Applies the erode or dilate operation to the image
void EditorPanel::onApplyErodeDilate(bool erode, bool dilate, int kernelSize) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    if (kernelSize % 2 == 0) kernelSize += 1; // Ensures the kernel size is odd

    cv::Mat result;
    cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(kernelSize, kernelSize));

    // Performs the appropriate morphological operation
    if (erode) {
        cv::erode(originalImage, result, element);
    } else if (dilate) {
        cv::dilate(originalImage, result, element);
    }

    mainImage = result;
    displayMainImageToPanel(); // Updates the displayed image
}

// Applies the resize operation to the image
void EditorPanel::onApplyResize(bool resizeByFactor, int factor, const wxString& widthStr, const wxString& heightStr) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    // Checks if resizing is needed; if not, reset to the original image
    if ((resizeByFactor && factor == 1) || (!resizeByFactor && (wxAtoi(widthStr) == 0 || wxAtoi(heightStr) == 0))) {
        mainImage = originalImage.clone();
        displayMainImageToPanel();
        return;
    }

    cv::Mat resizedImage;
    if (resizeByFactor) {
        cv::resize(originalImage, resizedImage, cv::Size(), factor, factor); // Resize by factor
    } else {
        int width = wxAtoi(widthStr);
        int height = wxAtoi(heightStr);
        if (width <= 0 || height <= 0) {
            wxMessageBox("Invalid dimensions.");
            return;
        }
        cv::resize(originalImage, resizedImage, cv::Size(width, height)); // Resize by specified dimensions
    }

    mainImage = resizedImage;
    displayMainImageToPanel(); // Updates the displayed image
}

// Applies Canny edge detection to the image
void EditorPanel::onApplyCanny(long lowThreshold, long highThreshold, int kernelSize) {
    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }

    if (kernelSize % 2 == 0) {
        kernelSize += 1; // Ensures the kernel size is odd
    }

    if (lowThreshold == 0 && highThreshold == 0) {
        mainImage = originalImage.clone();
        displayMainImageToPanel();
    } else {
        cv::Mat edges;
        cv::Canny(mainImage, edges, lowThreshold, highThreshold, kernelSize); // Apply Canny edge detection
        mainImage = edges;
        displayMainImageToPanel(); // Updates the displayed image
    }
}


void EditorPanel::onApplyGrayscale(int numLevels) {

    mainImage = originalImage.clone();
    int levelRange = 255 / numLevels;

    for (int i = 0; i < mainImage.rows; i++) {
        for (int j = 0; j < mainImage.cols; j++) {
            auto& pixel = mainImage.at<cv::Vec3b>(i, j);
            uchar Y = (pixel[0] + pixel[1] + pixel[2]) / 3;
            int newY = (Y / levelRange) * levelRange;
            if (newY > 255) {
                newY = 255;
            }
            pixel[0] = pixel[1] = pixel[2] = cv::saturate_cast<uchar>(newY);
        }
    }

    displayMainImageToPanel();
}


// Converts the image to black and white based on the specified intensity
void EditorPanel::onApplyBlackWhite(int intensity) {

    if (mainImage.empty()) {
        wxMessageBox("No image loaded.");
        return;
    }
    cv::Mat grayImage;
    cv::cvtColor(originalImage, grayImage, cv::COLOR_BGR2GRAY); // Converts to grayscale
    cv::Mat bwImage;
    cv::threshold(grayImage, bwImage, intensity, 255, cv::THRESH_BINARY); // Apply threshold to create black and white image

    cv::cvtColor(bwImage, mainImage, cv::COLOR_GRAY2BGR); // Converts back to BGR for display
    displayMainImageToPanel(); // Updates the displayed image
}

// Updates the displayed image after applying an OpenCV function
void EditorPanel::onApplyOpenCVFunction() {
    // Either use cv::Mat newImage = cv::AppliquerFonction(parametres)
    // mainImage = newImage;
    // Or mainImage = cv::AppliquerFonction(params)
    displayMainImageToPanel();
}

