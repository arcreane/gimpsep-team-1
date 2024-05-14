//
// Created by ghisl on 01/05/2024.
//

#ifndef GIMPSEP_TEAM_1_GIMPFRAME_H
#define GIMPSEP_TEAM_1_GIMPFRAME_H

#include <opencv2/core/mat.hpp>
#include "wx/wx.h"

class GimpFrame : public wxFrame {
public:
    explicit GimpFrame(const wxString& title);
private:
    wxPanel* leftPanel;
    wxPanel* rightPanel;
    wxBoxSizer* rightPanSizer;
    wxPanel* submenuPanel;

    wxStaticBitmap* bitmap;
    cv::Mat mainImageMat;

    void OnLoadImage(wxCommandEvent& event);
    void displayImageMatToSizer();

    wxGridSizer* CreateButtonGrid();

    void OnButtonClicked(wxCommandEvent& event, int buttonId);

    void CreateSubmenu1();
    void CreateSubmenu2();
    void CreateSubmenu3();


    void OnApplyLightenDarken(bool lighten, bool darken, int factor);
    void OnApplyErodeDilate(bool erode, bool dilate, int kernelSize);
    void OnApplyResize(bool resizeByFactor, int factor, const wxString& widthStr, const wxString& heightStr);


    void OnSaveImage(wxCommandEvent &event);
};


#endif //GIMPSEP_TEAM_1_GIMPFRAME_H
