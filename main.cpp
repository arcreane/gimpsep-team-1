#include <wx/wx.h>
#include "MainFrame.h"

class MyApp : public wxApp
{
public:
    virtual bool OnInit() override
    {
        MainFrame* frame = new MainFrame(wxT("My Application"));
        frame->Show(true);
        return true;
    }
};

IMPLEMENT_APP(MyApp)