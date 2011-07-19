/////////////////////////////////////////////////////////////////////////////
// Name:        src/common/imagraw.cpp
// Purpose:     wxImage RAW handler
// Author:      Peter Pettersson
// Version:     1.0
// CVS-ID:      $Id: 
// Copyright:   (c) 2010 Peter Pettersson
// Licence:     wxWindows licence
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx.h".
//#include "wx/wxprec.h"
#include "wx/wx.h"
#define wxUSE_RAW	0

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#if wxUSE_IMAGE && wxUSE_RAW

#ifndef WX_PRECOMP
    #include "wx/object.h"
    #include "wx/list.h"
    #include "wx/log.h"
    #include "wx/intl.h"
    #include "wx/palette.h"
    #include "wx/hash.h"
    #include "wx/module.h"
#endif

//#include "wx/imagraw.h"
#include "imagraw.h"
#include "wx/wfstream.h"

#include <vector>

//-----------------------------------------------------------------------------
// wxPCXHandler
//-----------------------------------------------------------------------------

IMPLEMENT_DYNAMIC_CLASS(wxRAWHandler,wxImageHandler)

#if wxUSE_STREAMS

//-----------------------------------------------------------------------------
// wxRAWHandler
//-----------------------------------------------------------------------------

bool wxRAWHandler::LoadFile( wxImage *image, wxInputStream& stream, bool verbose, int WXUNUSED(index) )
{
    //int error;

    if (!CanRead(stream))
    {
        if (verbose)
            wxLogError(_("RAW: this is not a RAW file."));

        return false;
    }

    image->Destroy();

	// Read in the whole file.
	// ToDo: very inefficient, is there a way to get the file size through wxInputStream?
	std::vector<unsigned char> buffer;
	while (wxEOF != stream.GetC())
		buffer.push_back(stream.LastRead());

	size_t bufferSize = buffer.size();

	// Guess format and size.
	int guessedWidth = -1, guessedHeight = -1, guessedDepth = -1;
	for (int d = 4; d > 0; --d)
	{
		if ((bufferSize % d) == 0)
		{
			float sqSize = sqrt((double)(bufferSize / d));
			if (sqSize == (float)((int)sqSize))
			{
				guessedWidth = guessedHeight = (int)sqSize;
				guessedDepth = d;
				break;
			}
		}
	}
	// ToDo: if all else fails, guess 1 x buffersize, 8 bit image.

	// ToDo: Show dialog with the guess and let the user input correct values.
	// Load and save preconfigs.
	// header data (pre or post).
	// rgba or palette (after header, at the end, offset).
	// width, height.

	// Verify that that it would match.
	if (guessedWidth == -1)
		return false;

	int width = guessedWidth;
	int height = guessedHeight;
	int depth = guessedDepth;

	image->Create(width, height);
	if (!image->Ok())
	{
		if (verbose)
			wxLogError(_("RAW: couldn't allocate memory"));
		image->Destroy();
		return false;
	}

	unsigned char *imageBuffer = image->GetData();

	int i = 0, src = 0, dst = 0;
	switch (depth)
	{
	case 4:	// In this case we just discard the alpha channel.
	case 3:
		for (; i < width * height; ++i)
		{
			imageBuffer[dst + 0] = buffer[src + 0];
			imageBuffer[dst + 1] = buffer[src + 1];
			imageBuffer[dst + 2] = buffer[src + 2];
			src += depth;
			dst += 3;
		}
		break;

	case 2:
		for (; i < width * height; ++i)
		{
			// from [rrrrrggg][gggbbbbb]
			unsigned short rgb = buffer[src + 0] | (buffer[src + 1] << 8);
			imageBuffer[dst + 0] = (rgb >>  0) & 0x1f;
			imageBuffer[dst + 1] = (rgb >>  5) & 0x3f;
			imageBuffer[dst + 2] = (rgb >> 12) & 0x1f;
			src += depth;
			dst += 3;
		}
		break;

	case 1:
		for (; i < width * height; ++i)
		{
			// The color becomes a greyscale intensity instead.
			imageBuffer[dst + 0] = buffer[src + 0];
			imageBuffer[dst + 1] = buffer[src + 0];
			imageBuffer[dst + 2] = buffer[src + 0];
			src += depth;
			dst += 3;
		}
		break;
	}

    return true;
}

bool wxRAWHandler::DoCanRead( wxInputStream& stream )
{
	return true;
}

#endif // wxUSE_STREAMS

#endif // wxUSE_IMAGE && wxUSE_RAW
