/////////////////////////////////////////////////////////////////////////////
// Name:        imagraw.h
// Purpose:     wxImage RAW handler
// Author:      Peter Pettersson
// RCS-ID:      $Id: 
// Copyright:   (c) 2010 Peter Pettersson
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _WX_IMAGRAW_H_
#define _WX_IMAGRAW_H_

#include "wx/image.h"

#define wxUSE_RAW 0
#define wxBITMAP_TYPE_RAW	(wxBITMAP_TYPE_ANY + 100)

//-----------------------------------------------------------------------------
// wxRAWHandler
//-----------------------------------------------------------------------------

#if wxUSE_RAW
class WXDLLEXPORT wxRAWHandler : public wxImageHandler
{
public:
    inline wxRAWHandler()
    {
        m_name = wxT("RAW file");
        m_extension = wxT("raw");
        m_type = wxBITMAP_TYPE_RAW;
        m_mime = wxT("image/raw");
    }

#if wxUSE_STREAMS
    virtual bool LoadFile( wxImage *image, wxInputStream& stream, bool verbose=true, int index=-1 );
    //virtual bool SaveFile( wxImage *image, wxOutputStream& stream, bool verbose=true );
protected:
    virtual bool DoCanRead( wxInputStream& stream );
#endif // wxUSE_STREAMS

private:
    DECLARE_DYNAMIC_CLASS(wxPCXHandler)
};
#endif // wxUSE_RAW


#endif
  // _WX_IMAGRAW_H_
