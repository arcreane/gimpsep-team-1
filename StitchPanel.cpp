
#include "StitchPanel.h"
#include "MainFrame.h"
#include "Manipulation.h"

StitchPanel::StitchPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {

    vBoxSizer = new wxBoxSizer(wxVERTICAL);
    buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    imagesPanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1050, 600));
    imagesSizer = new wxGridSizer(2, 3, 0, 0);
    vBoxSizer->Add(imagesPanel, 1, wxEXPAND | wxALL, 5);
    stitchButton = new wxButton(this, wxID_ANY, wxT("Stitch images"));
    stitchButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) { stitchImages();});
    removeButton = new wxButton(this, wxID_ANY, wxT("Clear image"));
    buttonSizer->AddStretchSpacer();
    buttonSizer->Add(stitchButton, 0, wxALIGN_CENTER | wxALL, 5);
    buttonSizer->Add(removeButton, 0, wxALIGN_CENTER | wxALL, 5);
    buttonSizer->AddStretchSpacer();
    vBoxSizer->Add(buttonSizer, 0, wxEXPAND | wxALL, 5);
    removeButton->Bind(wxEVT_BUTTON, [this](wxCommandEvent& event) { removeLastImage(); });
    vBoxSizer->Layout();
    SetSizer(vBoxSizer);

}

void StitchPanel::addImageToList(const cv::Mat& inImage) {
    if(images.size() < 6){
        images.push_back(inImage);
        displayImageList();
    } else {
        wxMessageBox(wxT("Stitching limited to 6 images."), wxT("Too many images"));
    }
}

void StitchPanel::displayImageList() {
    imagesPanel->DestroyChildren();
    imagesSizer->Clear(true);

    for (const auto& image : images) {
        double scaleFactor = std::min(350.0 / image.cols, 300.0 / image.rows);
        int newWidth = scaleFactor * image.cols;
        int newHeight = scaleFactor * image.rows;

        cv::Mat scaledImage;
        cv::resize(image, scaledImage, cv::Size(newWidth, newHeight));

        wxImage wximage = cvMatToWxImage(scaledImage);
        wxBitmap bitmap(wximage);
        imagesSizer->Add(new wxStaticBitmap(imagesPanel, wxID_ANY, bitmap), 0, wxALIGN_CENTER | wxALL, 0);
    }

    imagesPanel->SetSizer(imagesSizer);
    imagesSizer->Layout();
    refreshDisplay();
}

void StitchPanel::stitchImages() {

    cv::Ptr<cv::Stitcher> stitcher = cv::Stitcher::create(cv::Stitcher::PANORAMA);
    cv::Stitcher::Status status = stitcher->stitch(images, stitchedImage);

    if (status == cv::Stitcher::OK) {
        imagesPanel->DestroyChildren();
        images.clear();

        double scaleFactor = std::min(1050.0 / stitchedImage.cols, 600.0 / stitchedImage.rows);
        int newWidth = scaleFactor * stitchedImage.cols;
        int newHeight = scaleFactor * stitchedImage.rows;
        cv::Mat scaledResult;
        cv::resize(stitchedImage, scaledResult, cv::Size(newWidth, newHeight));

        wxBitmap bitmap(cvMatToWxImage(scaledResult));
        auto* stitchedBitmap = new wxStaticBitmap(imagesPanel, wxID_ANY, bitmap);
        auto* stitchedImageSizer = new wxBoxSizer(wxVERTICAL);
        stitchedImageSizer->Add(stitchedBitmap, 1, wxEXPAND);
        imagesPanel->SetSizer(stitchedImageSizer);
        stitchedImageSizer->Fit(imagesPanel);
        stitchedImageSizer->SetSizeHints(imagesPanel);
        imagesPanel->Layout();
        refreshDisplay();
    } else {
        wxString message(wxT("An error occured. Please try again with 2-6 images sharing similarities."));
        wxMessageBox(message, wxT("Stitching error"));
    }
}

void StitchPanel::refreshDisplay() {
    dynamic_cast<MainFrame*>(GetParent())->loadMenuPanel();
    dynamic_cast<MainFrame*>(GetParent())->loadStitcherPanel();
}

void StitchPanel::removeLastImage() {
    if(!stitchedImage.empty()){
        stitchedImage.release();
        imagesPanel->DestroyChildren();
        imagesSizer = new wxGridSizer(2, 3, 0, 0);
        imagesPanel->SetSizer(imagesSizer);
        imagesSizer->Layout();
    }
    if (!images.empty()) {
        images.pop_back();
    }
    displayImageList();
}

cv::Mat StitchPanel::getStitchedImage() {
    return stitchedImage;
}
