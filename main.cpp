#include "MainFrame.h"
#include "imagraw.h"

class App : public wxApp
{
public:
  virtual bool OnInit()
  {
    wxInitAllImageHandlers();
#if wxUSE_RAW
    wxImage::AddHandler(new wxRAWHandler);
#endif

    if (!wxApp::OnInit())
      return false;

    MainFrame *frame = new MainFrame;

    return true;
  }
};

IMPLEMENT_APP(App)
