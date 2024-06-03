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
    cv::Mat originalImage;
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

    void createLightenDarkenSubmenu();
    void createErodeDilateSubmenu();
    void createResizeSubmenu();
    void createCannySubmenu();

    void onApplyLightenDarken(int adjustment);
    void onApplyOpenCVFunction();
    void onApplyErodeDilate(bool erode, bool dilate, int kernelSize);
    void onApplyResize(bool resizeByFactor, int factor, const wxString& widthStr, const wxString& heightStr);
    void onApplyCanny(long lowThreshold, long highThreshold, int kernelSize);
};


#endif //GIMPSEP_TEAM_1_EDITORPANEL_H
