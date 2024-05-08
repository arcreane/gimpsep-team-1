//
// Created by ghisl on 08/05/2024.
//

#include "GimpIsep.h"
#include "MenuFrame.h"

bool GimpIsep::OnInit() {
    auto* frame = new MenuFrame();
    frame->Show(true);
    return true;
}
