//
// Created by ghisl on 08/05/2024.
//

#ifndef GIMPSEP_TEAM_1_EDITORPANEL_H
#define GIMPSEP_TEAM_1_EDITORPANEL_H

#include "wx/wx.h"
#include "opencv2/opencv.hpp"

class EditorPanel : public wxPanel
{
    cv::Mat mainImage;
public:
    explicit EditorPanel(wxWindow* parent);
};


#endif //GIMPSEP_TEAM_1_EDITORPANEL_H
