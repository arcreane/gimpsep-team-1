//
// Created by ghisl on 14/05/2024.
//

#include "Manipulation.h"

wxImage cvMatToWxImage(const cv::Mat& mat)
{
    wxImage img(mat.cols, mat.rows);

    if (mat.channels() == 3)
    {
        for (int y = 0; y < mat.rows; y++)
        {
            const uchar* matRow = mat.ptr<uchar>(y);
            uchar* imgRow = img.GetData() + y * img.GetWidth() * 3;
            for (int x = 0; x < mat.cols; x++)
            {
                imgRow[0] = matRow[2];
                imgRow[1] = matRow[1];
                imgRow[2] = matRow[0];
                matRow += 3;
                imgRow += 3;
            }
        }
    }
    else if (mat.channels() == 1)
    {
        cv::Mat mat3;
        cv::cvtColor(mat, mat3, cv::COLOR_GRAY2RGB);

        for (int y = 0; y < mat3.rows; y++)
        {
            const uchar* matRow = mat3.ptr<uchar>(y);
            uchar* imgRow = img.GetData() + y * img.GetWidth() * 3;
            for (int x = 0; x < mat3.cols; x++)
            {
                imgRow[0] = matRow[2];
                imgRow[1] = matRow[1];
                imgRow[2] = matRow[0];
                matRow += 3;
                imgRow += 3;
            }
        }
    }
    return img;
}
