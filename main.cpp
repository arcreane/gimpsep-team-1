#include <wx/wx.h>
#include <opencv2/opencv.hpp>
#include <fstream>

wxImage MatToWxImage(const cv::Mat& mat)
{
    wxImage img(mat.cols, mat.rows);

    if (mat.channels() == 3)
    {
        for (int y = 0; y < mat.rows; y++)
        {
            const uchar* matRow = mat.ptr<uchar>(y);
            uchar* imgRow = img.GetData() + y * img.GetWidth() * 3;
            memcpy(imgRow, matRow, mat.cols * 3);
        }
    }
    else if (mat.channels() == 1)
    {
        cv::Mat mat3;
        cv::cvtColor(mat, mat3, cv::COLOR_GRAY2BGR);

        for (int y = 0; y < mat3.rows; y++)
        {
            const uchar* matRow = mat3.ptr<uchar>(y);
            uchar* imgRow = img.GetData() + y * img.GetWidth() * 3;
            memcpy(imgRow, matRow, mat3.cols * 3);
        }
    }
    return img;
}


class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title);

private:
    wxImage image;
    wxStaticBitmap* bitmap;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyFrame *frame = new MyFrame("Hello World");
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title)
{
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

    std::string imgSrc = "images/IMG_0067.jpg";

    cv::Mat mat = cv::imread(imgSrc, cv::IMREAD_COLOR);

    if (mat.empty())
    {
        wxLogError("Cannot load the image file.");
        return;
    }

    image = MatToWxImage(mat);

    int width = GetSize().GetWidth();
    int height = GetSize().GetHeight();
    double scaleFactor = std::min((double)width / image.GetWidth(), (double)height / image.GetHeight());
    int newWidth = image.GetWidth() * scaleFactor;
    int newHeight = image.GetHeight() * scaleFactor;
    image.Rescale(newWidth, newHeight, wxIMAGE_QUALITY_HIGH);

    bitmap = new wxStaticBitmap(this, wxID_ANY, wxBitmap(image));
    sizer->Add(bitmap, 1, wxALL | wxEXPAND, 5);

    SetSizer(sizer);
    Layout();
}



int main()
{
    MyApp app;
    app.MainLoop();
    return 0;
}
