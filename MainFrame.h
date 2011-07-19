#ifndef MAIN_FRAME_H
#define MAIN_FRAME_H

#include <wx/wx.h>

class MainFrame : public wxFrame
{
public:
  MainFrame();

  void OnFileOpen(wxCommandEvent &event);
  void OnFileOpenDebug(wxCommandEvent &event);
  void OnFileExit(wxCommandEvent &event);

  void OnMouseEvent(wxMouseEvent &event);

private:
  enum Id
  {
    kMainFrame_ = wxID_HIGHEST + 1,

    kFile_Open,
    kFile_OpenDebug,
    kFile
  };

  wxString allImageFormats;

  DECLARE_EVENT_TABLE()

  void GenerateAllImageFormats();
};

#endif // MAIN_FRAME_H
