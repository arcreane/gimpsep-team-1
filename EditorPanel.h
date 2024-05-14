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
    wxPanel* menuPanel;
    wxPanel* imagePanel;
    wxPanel* subMenuPanel;
    wxBoxSizer* imagePanSizer;
    wxBoxSizer* menuPanelSizer;
    wxStaticBitmap* imageBitmap;
    wxGridSizer * createButtonGrid();
public:
    explicit EditorPanel(wxWindow* parent);
    void setImage(cv::Mat);
    cv::Mat getImage();

    void displayMainImageToPanel();

    void onButtonClicked(wxCommandEvent &event, int id);

    void createLightenSubmenu();
    void createSubmenu2();
    void createSubmenu3();

    void onApplyLighten();
    void onApplyOpenCVFunction();
};


#endif //GIMPSEP_TEAM_1_EDITORPANEL_H