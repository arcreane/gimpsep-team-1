//
// Created by ghisl on 08/05/2024.
//

#include "GimpIsep.h"
#include "MainFrame.h"

bool GimpIsep::OnInit() {
    auto* frame = new MainFrame();
    frame->Show(true);
    return true;
}
