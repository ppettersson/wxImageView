#include <wx/numdlg.h>
#include "ImageFrame.h"

BEGIN_EVENT_TABLE(ImageFrame, wxFrame)
  //EVT_ERASE_BACKGROUND(ImageFrame::OnEraseBackground)
  EVT_PAINT(ImageFrame::OnPaint)
  EVT_CONTEXT_MENU(ImageFrame::OnContextMenu)
  EVT_MENU(kPopup_Blur, ImageFrame::OnPopupBlur)
  EVT_MENU(kPopup_BlurHorizontal, ImageFrame::OnPopupBlurHorizontal)
  EVT_MENU(kPopup_BlurVertical, ImageFrame::OnPopupBlurVertical)
  EVT_MENU(kPopup_MirrorHorizontal, ImageFrame::OnPopupMirrorHorizontal)
  EVT_MENU(kPopup_MirrorVertical, ImageFrame::OnPopupMirrorVertical)
  EVT_MENU(kPopup_Crop, ImageFrame::OnPopupCrop)
  EVT_MENU(kPopup_Resize, ImageFrame::OnPopupResize)
  EVT_MENU(kPopup_SaveAs, ImageFrame::OnPopupSaveAs)
  EVT_MENU(kPopup_Close, ImageFrame::OnPopupClose)
  END_EVENT_TABLE()


  ImageFrame::ImageFrame(wxFrame *parent, const wxImage &img, const wxString &fileName)
  : wxFrame(parent, wxID_ANY, fileName, wxDefaultPosition, wxDefaultSize, wxCAPTION | wxSYSTEM_MENU | wxCLOSE_BOX)
  , image(img)
  , bitmap(img)
{
  // Resize the window to tightly fit the image, then show it.
  SetClientSize(bitmap.GetWidth(), bitmap.GetHeight());
  Show();
}

void ImageFrame::UpdateImage()
{
  bitmap = wxBitmap(image);
  SetClientSize(bitmap.GetWidth(), bitmap.GetHeight());
  Refresh();
}

void ImageFrame::OnPaint(wxPaintEvent &event)
{
  wxPaintDC dc(this);

  // If image is transparent, check settings if we should:
  // - draw a checkerboard
  // - draw a color
  // - leave transparent

  dc.DrawBitmap(bitmap, 0, 0, true);
}

void ImageFrame::OnContextMenu(wxContextMenuEvent &event)
{
  wxPoint position = event.GetPosition();

  // Context menus can be triggered from keyboard as well.
  if (position.x == -1 && position.y == -1)
  {
    wxSize size = GetSize();
    position.x = size.x / 2;
    position.y = size.y / 2;
  }
  else
    position = ScreenToClient(position);

  // Create the popup menu.
  wxMenu menu;

  wxMenu *blur = new wxMenu;
  blur->Append(kPopup_Blur, wxT("&Blur"), wxT("Blurs the image in both horizontal and vertical directions"));
  blur->Append(kPopup_BlurHorizontal, wxT("Blur &Horizontal"), wxT("Blurs the image in the horizontal direction only"));
  blur->Append(kPopup_BlurVertical, wxT("Blur &Vertical"), wxT("Blurs the image in the vertical direction only"));

  wxMenu *mirror = new wxMenu;
  mirror->Append(kPopup_MirrorHorizontal, wxT("Mirror &Horizontal"), wxT("Mirrors the image horizontally"));
  mirror->Append(kPopup_MirrorVertical, wxT("Mirror &Vertical"), wxT("Mirrors the image vertically"));

  wxMenu *imageProcessing = new wxMenu;
  imageProcessing->AppendSubMenu(blur, wxT("&Blur"));
  imageProcessing->AppendSubMenu(mirror, wxT("&Mirror"));

  menu.AppendSubMenu(imageProcessing, wxT("&Image processing"));
  menu.AppendSeparator();

  wxMenu *size = new wxMenu;
  size->Append(kPopup_Crop, wxT("&Crop"), wxT("Crop the image"));
  size->Append(kPopup_Resize, wxT("&Resize"), wxT("Resize this image"));
  menu.AppendSubMenu(size, wxT("&Size"));
  menu.AppendSeparator();

  menu.Append(kPopup_SaveAs, wxT("Save &As"), wxT("Save this frame as a new image on disc"));
  menu.AppendSeparator();

  menu.Append(kPopup_Close, wxT("&Close"), wxT("Close this frame"));

  PopupMenu(&menu, position);
}

void ImageFrame::OnPopupBlur(wxCommandEvent &event)
{
  int radius = ::wxGetNumberFromUser(wxEmptyString, wxT("radius:"), wxT("Blur options"), 10, 0, 100, this);
  image = image.Blur(radius);
  UpdateImage();
}

void ImageFrame::OnPopupBlurHorizontal(wxCommandEvent &event)
{
  int radius = ::wxGetNumberFromUser(wxEmptyString, wxT("radius:"), wxT("Blur options"), 10, 0, 100, this);
  image = image.BlurHorizontal(radius);
  UpdateImage();
}

void ImageFrame::OnPopupBlurVertical(wxCommandEvent &event)
{
  int radius = ::wxGetNumberFromUser(wxEmptyString, wxT("radius:"), wxT("Blur options"), 10, 0, 100, this);
  image = image.BlurVertical(radius);
  UpdateImage();
}

void ImageFrame::OnPopupMirrorHorizontal(wxCommandEvent &event)
{
  image = image.Mirror(true);
  UpdateImage();
}

void ImageFrame::OnPopupMirrorVertical(wxCommandEvent &event)
{
  image = image.Mirror(false);
  UpdateImage();
}

void ImageFrame::OnPopupCrop(wxCommandEvent &event)
{
  wxString caption(wxT("Crop image"));

  wxString message = wxString::Format(wxT("Enter a number between 0 and %d"), image.GetWidth() - 1);
  long x = wxGetNumberFromUser(message, wxT("x:"), caption, 0, 0, image.GetWidth() - 1, this);
  if (x < 1)
    return;

  message = wxString::Format(wxT("Enter a number between 0 and %d"), image.GetHeight() - 1);
  long y = wxGetNumberFromUser(message, wxT("y:"), caption, 0, 0, image.GetHeight() - 1, this);
  if (y < 1)
    return;

  int maxWidth = image.GetWidth() - x;
  message = wxString::Format(wxT("Enter a number between 1 and %d"), maxWidth);
  long width = wxGetNumberFromUser(message, wxT("Width:"), caption, maxWidth, 1, maxWidth, this);
  if (width < 1)
    return;

  int maxHeight = image.GetHeight() - y;
  message = wxString::Format(wxT("Enter a number between 1 and %d"), maxHeight);
  long height = wxGetNumberFromUser(message, wxT("Height:"), caption, maxHeight, 1, maxHeight, this);
  if (height < 1)
    return;

  if ((x == 0) && (y == 0) && (width == image.GetWidth()) && (height == image.GetHeight()))
  {
    wxMessageBox(wxT("Same size as original image, nothing to be done."), wxT("Warning"), wxOK | wxCENTER, this);
    return;
  }

  image = image.Size(wxSize(width, height), wxPoint(-x, -y));
  UpdateImage();
}

void ImageFrame::OnPopupResize(wxCommandEvent &event)
{
  wxString caption(wxT("Resize image"));
  long width = wxGetNumberFromUser(wxT("Enter a number between 1 and 4096"), wxT("Width:"), caption, image.GetWidth(), 1, 4096, this);
  if (width < 1)
    return;

  long height = wxGetNumberFromUser(wxT("Enter a number between 1 and 4096"), wxT("Height:"), caption, image.GetHeight(), 1, 4096, this);
  if (height < 1)
    return;

  if ((width == image.GetWidth()) && (height == image.GetHeight()))
  {
    wxMessageBox(wxT("Same size as original image, nothing to be done."), wxT("Warning"), wxOK | wxCENTER, this);
    return;
  }

  wxArrayString choices;
  choices.Add(wxT("Nearest"));
  choices.Add(wxT("Bilinear"));
  choices.Add(wxT("bicubic"));
  wxImageResizeQuality quality;
  switch (wxGetSingleChoiceIndex(wxT("Select quality algorithm"), caption, choices, 1, this))
  {
  case -1:  return;
  case  0:  quality = wxIMAGE_QUALITY_NEAREST;  break;
  case  1:  quality = wxIMAGE_QUALITY_BILINEAR; break;
  case  2:
  default:  quality = wxIMAGE_QUALITY_BICUBIC;  break;
  }

  image = image.Scale(width, height, quality);
  UpdateImage();
}

void ImageFrame::OnPopupSaveAs(wxCommandEvent &event)
{
  wxString fileName = ::wxFileSelector(wxFileSelectorPromptStr, wxEmptyString, wxEmptyString, wxEmptyString, wxFileSelectorDefaultWildcardStr);
  if (fileName.empty())
    return;

  if (!image.SaveFile(fileName))
  {
    wxMessageBox(wxT("Could not save image file."), wxT("File error"), wxICON_ERROR | wxOK, this);
    return;
  }
}

void ImageFrame::OnPopupClose(wxCommandEvent &event)
{
  Close();
}
