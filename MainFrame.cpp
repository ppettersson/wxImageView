#include "MainFrame.h"
#include "DebugImageFrame.h"
#include "ImageFrame.h"
#include <wx/file.h>

// ToDo: add our own image handler for raw images:
// - guess: assume squared, rgba.
// - get for user: format, width and height.

BEGIN_EVENT_TABLE(MainFrame, wxFrame)
  EVT_MENU(kFile_Open,      MainFrame::OnFileOpen)
  EVT_MENU(kFile_OpenDebug, MainFrame::OnFileOpenDebug)
  EVT_MENU(wxID_EXIT,       MainFrame::OnFileExit)
  EVT_LEFT_DCLICK(MainFrame::OnMouseEvent)
  END_EVENT_TABLE()


  MainFrame::MainFrame()
  : wxFrame(NULL, wxID_ANY, wxT("ImageView"))
{
  GenerateAllImageFormats();

  wxMenu *fileMenu = new wxMenu;
  fileMenu->Append(kFile_Open, wxT("&Open"), wxT("Open an image file"));
  fileMenu->Append(kFile_OpenDebug, wxT("Open &Debug"), wxT("Open a debug image host"));
  fileMenu->AppendSeparator();
  fileMenu->Append(wxID_EXIT, wxT("E&xit"), wxT("Quit this program"));

  wxMenuBar *menuBar = new wxMenuBar();
  menuBar->Append(fileMenu, wxT("&File"));

  SetMenuBar(menuBar);

  Show(true);
}

void MainFrame::OnFileOpen(wxCommandEvent &event)
{
  wxString fileName = ::wxFileSelector(wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxEmptyString, allImageFormats);
  if (!fileName.empty())
  {
    wxImage image;
    if (!image.LoadFile(fileName))
    {
      wxMessageBox(wxT("Could not load image file."), wxT("File error"), wxICON_ERROR | wxOK, this);
      return;
    }

    // Open a frame with the image.
    new ImageFrame(this, image, fileName);
  }
}

void MainFrame::OnFileOpenDebug(wxCommandEvent &event)
{
  new DebugImageFrame(this);
}

void MainFrame::OnFileExit(wxCommandEvent &event)
{
  Close(true);
}

void MainFrame::OnMouseEvent(wxMouseEvent &event)
{
  OnFileOpen(wxCommandEvent());
}

struct Ext
{
  wxString      name;
  wxString      ext;
  wxArrayString alts;
};

bool operator < (const Ext &a, const Ext &b)  { return a.name < b.name; }

void MainFrame::GenerateAllImageFormats()
{
  // Start of with a generic filter for all files.
  allImageFormats = wxT("All files (*.*)|*.*");

  // Grab all the installed image handlers.
  wxList &list = wxImage::GetHandlers();

  // Guard against no handlers installed.
  if (list.GetCount() < 1)
    return;

  // Create a sorted (by name) list of them all.
  wxVector<Ext> sortedList;
  for (wxList::compatibility_iterator node = list.GetFirst(); node; node = node->GetNext())
  {
    wxImageHandler *handler = (wxImageHandler *)node->GetData();

    Ext e;
    e.name  = handler->GetName();
    e.ext   = handler->GetExtension();
    e.alts  = handler->GetAltExtensions();

    sortedList.push_back(e);
  }
  wxVectorSort(sortedList);

  // Create a separate list where we sort all the extension together.
  wxArrayString sortedExtensions;
  for (wxVector<Ext>::const_iterator itr = sortedList.begin(); itr != sortedList.end(); ++itr)
  {
    const Ext &e = *itr;

    sortedExtensions.push_back(e.ext);
    for (size_t i = 0; i < e.alts.GetCount(); ++i)
      sortedExtensions.push_back(e.alts.Item(i));
  }
  sortedExtensions.Sort();

  wxString sortedExtensionsStr;
  for (size_t i = 0; i < sortedExtensions.GetCount(); ++i)
  {
    if (i > 0)
      sortedExtensionsStr += wxT("; ");

    sortedExtensionsStr += wxT("*.");
    sortedExtensionsStr += sortedExtensions.Item(i);
  }

  allImageFormats += wxT("|All image files (");
  allImageFormats += sortedExtensionsStr;
  allImageFormats += wxT(")|");
  allImageFormats += sortedExtensionsStr;

  // Add an entry for each file format.
  for (wxVector<Ext>::const_iterator itr = sortedList.begin(); itr != sortedList.end(); ++itr)
  {
    const Ext &e = *itr;

    allImageFormats += wxT("|");
    allImageFormats += e.name;
    allImageFormats += wxT("s (*.");

    wxString ext;
    ext += e.ext;
    for (size_t i = 0; i < e.alts.GetCount(); ++i)
    {
      ext += wxT("; *.");
      ext += e.alts.Item(i);
    }

    allImageFormats += ext;
    allImageFormats += wxT(")|*.");
    allImageFormats += ext;
  }
}
