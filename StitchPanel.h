//
// Created by ghisl on 08/05/2024.
//

#ifndef GIMPSEP_TEAM_1_STITCHPANEL_H
#define GIMPSEP_TEAM_1_STITCHPANEL_H


#include <wx/wx.h>
#include <opencv2/opencv.hpp>

class StitchPanel : public wxPanel {
    std::vector<cv::Mat> images;
    cv::Mat stitchedImage;
    wxButton* stitchButton;
    wxButton* removeButton;
    wxBoxSizer* buttonSizer;
    wxBoxSizer* vBoxSizer;
    wxPanel* imagesPanel;
    wxGridSizer* imagesSizer;
    void stitchImages();
public:
    explicit StitchPanel(wxWindow* parent);
    void addImageToList(const cv::Mat&);
    void displayImageList();
    void refreshDisplay();

    void removeLastImage();

    cv::Mat getStitchedImage();
};


#endif //GIMPSEP_TEAM_1_STITCHPANEL_H
