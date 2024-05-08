//
// Created by ghisl on 08/05/2024.
//

#include "EditorFrame.h"

EditorFrame::EditorFrame() : wxFrame(nullptr, wxID_ANY, wxT("GimpIsep"),wxDefaultPosition,
                                      wxSize(1200, 700),wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)  {

    wxStaticText* title = new wxStaticText(this, wxID_ANY, wxT("Editor Frame"));

    wxBoxSizer* vBoxSizer = new wxBoxSizer(wxVERTICAL);
    vBoxSizer->AddStretchSpacer();
    vBoxSizer->Add(title, 0, wxALIGN_CENTER_HORIZONTAL, 0);
    vBoxSizer->AddStretchSpacer();

    SetSizer(vBoxSizer);

    SetBackgroundColour(*wxWHITE);

}
