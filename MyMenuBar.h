//
// Created by ghisl on 14/05/2024.
//

#ifndef GIMPSEP_TEAM_1_MYMENUBAR_H
#define GIMPSEP_TEAM_1_MYMENUBAR_H


#include <wx/wx.h>

class MyMenuBar : public wxMenuBar {
    wxPanel* contentPanel;
public:
    explicit MyMenuBar(wxPanel *pPanel);

};


#endif //GIMPSEP_TEAM_1_MYMENUBAR_H
