//
// Created by ghisl on 30/04/2024.
//

#include <wx/wx.h>
#include <opencv2/opencv.hpp>

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