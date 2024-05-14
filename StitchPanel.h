//
// Created by ghisl on 08/05/2024.
//

#ifndef GIMPSEP_TEAM_1_STITCHPANEL_H
#define GIMPSEP_TEAM_1_STITCHPANEL_H


#include <wx/wx.h>
#include <opencv2/opencv.hpp>

class StitchPanel : public wxPanel {
    std::vector<cv::Mat> images;
public:
    explicit StitchPanel(wxWindow* parent);
};


#endif //GIMPSEP_TEAM_1_STITCHPANEL_H
