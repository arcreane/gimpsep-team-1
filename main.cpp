#include <wx/wx.h>
#include "GimpFrame.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        auto* frame = new GimpFrame(wxT("Gimpsep 1"));
        frame->Show(true);
        return true;
    }
};

IMPLEMENT_APP(MyApp)