#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include <wx/image.h>

class MainFrame : public wxFrame
{
public:
    MainFrame(const wxString& title);

private:
    wxImage image;
    wxStaticBitmap* bitmap;
    wxBoxSizer* mainSizer;
    wxPanel* lateralPanel;
    wxBoxSizer* buttonSizer;
    wxBoxSizer* textSizer;
    wxStaticText* infoText;

    void OnButtonClick(wxCommandEvent& event);
};

#endif // MAIN_FRAME_H
