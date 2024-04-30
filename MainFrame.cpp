#include "MainFrame.h"

MainFrame::MainFrame(const wxString& title) : wxFrame(nullptr, wxID_ANY, title)
{

    lateralPanel = new wxPanel(this, wxID_ANY);
    lateralSizer = new wxBoxSizer(wxVERTICAL);
    lateralPanel->SetSizer(lateralSizer);

    wxButton* button1 = new wxButton(lateralPanel, wxID_ANY, wxT("Button 1"));
    wxButton* button2 = new wxButton(lateralPanel, wxID_ANY, wxT("Button 2"));
    wxButton* button3 = new wxButton(lateralPanel, wxID_ANY, wxT("Button 3"));

    lateralSizer->Add(button1, 0, wxALL, 5);
    lateralSizer->Add(button2, 0, wxALL, 5);
    lateralSizer->Add(button3, 0, wxALL, 5);

    HideLayout();

    button1->Bind(wxEVT_BUTTON, &MainFrame::OnButton1Click, this);
    button2->Bind(wxEVT_BUTTON, &MainFrame::OnButton2Click, this);
    button3->Bind(wxEVT_BUTTON, &MainFrame::OnButton3Click, this);

    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);
    mainSizer->AddStretchSpacer();
    mainSizer->Add(lateralPanel, 0, wxEXPAND);

    SetSizer(mainSizer);
    Layout();
}

wxBoxSizer* MainFrame::CreateLayout1()
{
    wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* intInput1 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
    wxTextCtrl* intInput2 = new wxTextCtrl(this, wxID_ANY, wxT(""), wxDefaultPosition, wxDefaultSize, 0, wxTextValidator(wxFILTER_NUMERIC));
    wxButton* validateButton = new wxButton(this, wxID_ANY, wxT("Validate"));

    layout->Add(intInput1, 0, wxALL, 5);
    layout->Add(intInput2, 0, wxALL, 5);
    layout->Add(validateButton, 0, wxALL, 5);

    return layout;
}

wxBoxSizer* MainFrame::CreateLayout2()
{
    wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* textInput1 = new wxTextCtrl(lateralPanel, wxID_ANY, wxT(""));
    wxButton* validateButton = new wxButton(lateralPanel, wxID_ANY, wxT("Validate"));

    layout->Add(textInput1, 0, wxALL, 5);
    layout->Add(validateButton, 0, wxALL, 5);

    return layout;
}

wxBoxSizer* MainFrame::CreateLayout3()
{
    wxBoxSizer* layout = new wxBoxSizer(wxVERTICAL);

    wxTextCtrl* textInput1 = new wxTextCtrl(lateralPanel, wxID_ANY, wxT("1"));
    wxTextCtrl* textInput2 = new wxTextCtrl(lateralPanel, wxID_ANY, wxT("2"));
    wxButton* validateButton = new wxButton(lateralPanel, wxID_ANY, wxT("Validate"));

    layout->Add(textInput1, 0, wxALL, 5);
    layout->Add(textInput2, 0, wxALL, 5);
    layout->Add(validateButton, 0, wxALL, 5);

    return layout;
}

void MainFrame::OnButton1Click(wxCommandEvent& event)
{
    ShowLayout(CreateLayout1());
}

void MainFrame::OnButton2Click(wxCommandEvent& event)
{
    ShowLayout(CreateLayout2());
}

void MainFrame::OnButton3Click(wxCommandEvent& event)
{
    ShowLayout(CreateLayout3());
}

void MainFrame::ShowLayout(wxBoxSizer* layout)
{
    HideLayout();
    lateralPanel->SetSizer(layout);
    lateralPanel->Layout();
}

void MainFrame::HideLayout()
{
    if (lateralSizer->GetItemCount() > 3)
    {
        wxSizerItem* item = lateralSizer->GetItem(3);
        wxSizer* layout = item->GetSizer();
        lateralSizer->Remove(3);
        layout->Clear(true);
        delete layout;
        lateralPanel->Layout();
    }
}
