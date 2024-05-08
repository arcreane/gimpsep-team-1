//
// Created by ghisl on 08/05/2024.
//

#include "MenuFrame.h"

MenuFrame::MenuFrame() : wxFrame(nullptr, wxID_ANY, wxT("GimpIsep"),wxDefaultPosition,
wxSize(1200, 700),wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)  {
    std::cout << "Built Menu Frame" << std::endl;

}
