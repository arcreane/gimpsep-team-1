#include "MainFrame.h"
#include "Manipulation.h"

MainFrame::MainFrame(const wxString& title)
        : wxFrame(NULL, wxID_ANY, title),
          lateralPanel(new wxPanel(this, wxID_ANY)),
          buttonSizer(new wxBoxSizer(wxVERTICAL)),
          textSizer(new wxBoxSizer(wxVERTICAL)),
          infoText(new wxStaticText(lateralPanel, wxID_ANY, wxT("")))
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

    wxButton* button1 = new wxButton(lateralPanel, wxID_ANY, wxT("Button 1"));
    wxButton* button2 = new wxButton(lateralPanel, wxID_ANY, wxT("Button 2"));
    wxButton* button3 = new wxButton(lateralPanel, wxID_ANY, wxT("Button 3"));


    buttonSizer->Add(button1, 0, wxALL, 5);
    buttonSizer->Add(button2, 0, wxALL, 5);
    buttonSizer->Add(button3, 0, wxALL, 5);

    textSizer->Add(infoText, 0, wxALL, 5);

    wxBoxSizer* lateralSizer = new wxBoxSizer(wxVERTICAL);
    lateralSizer->Add(buttonSizer, 1, wxEXPAND);
    lateralSizer->Add(textSizer, 0, wxEXPAND);

    lateralPanel->SetSizer(lateralSizer);

    mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->Add(sizer, 1, wxEXPAND);
    mainSizer->Add(lateralPanel, 0, wxEXPAND);

    SetSizer(mainSizer);
    Layout();

    button1->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
    button2->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
    button3->Bind(wxEVT_BUTTON, &MainFrame::OnButtonClick, this);
}

void MainFrame::OnButtonClick(wxCommandEvent& event)
{
    wxButton* button = dynamic_cast<wxButton*>(event.GetEventObject());
    if (button)
    {
        infoText->SetLabel(button->GetLabelText() + wxT(" was clicked!"));
    }
}

