#ifndef IMAGE_FRAME_H
#define IMAGE_FRAME_H

#include <wx/wx.h>

class ImageFrame : public wxFrame
{
public:
  ImageFrame(wxFrame *parent, const wxImage &image, const wxString &fileName = wxT("<new image>"));


protected:
  wxImage   image;      // Platform independent image.

  void UpdateImage();


private:
  enum Id
  {
    kPopup_  = wxID_HIGHEST + 100,

    kPopup_Blur,
    kPopup_BlurHorizontal,
    kPopup_BlurVertical,
    kPopup_MirrorHorizontal,
    kPopup_MirrorVertical,

    kPopup_Crop,
    kPopup_Resize,

    kPopup_SaveAs,

    kPopup_Close
  };

  wxBitmap  bitmap;     // Platform dependent image suitable for direct rendering.

  void OnEraseBackground(wxEraseEvent &event)				{ }
  void OnPaint(wxPaintEvent &event);
  void OnContextMenu(wxContextMenuEvent &event);

  void OnPopupBlur(wxCommandEvent &event);
  void OnPopupBlurHorizontal(wxCommandEvent &event);
  void OnPopupBlurVertical(wxCommandEvent &event);
  void OnPopupMirrorHorizontal(wxCommandEvent &event);
  void OnPopupMirrorVertical(wxCommandEvent &event);

  void OnPopupCrop(wxCommandEvent &event);
  void OnPopupResize(wxCommandEvent &event);

  void OnPopupSaveAs(wxCommandEvent &event);

  void OnPopupClose(wxCommandEvent &event);

  DECLARE_EVENT_TABLE()
};

#endif // IMAGE_FRAME_H
