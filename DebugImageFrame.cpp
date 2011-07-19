#include "DebugImageFrame.h"
#include <fstream>

#define DEFAULT_DIR			"c:/tmp/debugimage/"
#define DEFAULT_FILE_NAME	"dbg"
#define DEFAULT_FILE_EXT	".wiv"

DebugImageFrame::DebugImageFrame(wxFrame *parent)
  : ImageFrame(parent, wxImage(320, 240), wxT("Debug Image"))
{
  static int globalId = 0;
  id = globalId++;

  dirNotifier = new DirNotifier(TEXT(DEFAULT_DIR), this);
  if (dirNotifier->Create() != wxTHREAD_NO_ERROR)
  {
    wxLogError(wxT("Unable to create the thread for directory notification callbacks."));
  }
  else
  {
    dirNotifier->Run();
  }

  fileName = wxString(DEFAULT_DIR);
  fileName += wxString(DEFAULT_FILE_NAME);
  fileName += wxString::Format(wxT("_%d"), id);
  fileName += wxString(DEFAULT_FILE_EXT);
}

DebugImageFrame::~DebugImageFrame()
{
  // Shut down the thread.
  //delete dirNotifier;
}

void DebugImageFrame::DirNotifyCallback()
{
  // ToDo: only check the file that is connected to this view.

  // Reload the image.
  std::ifstream file((const char *)fileName.c_str(), std::ios::binary);
  if (file)
  {
    // Check file size.
    file.seekg(0, std::ios::end);
    size_t fileLength = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read header.
    unsigned int width, height, depth, format;
    file.read((char *)&width, sizeof(width));
    file.read((char *)&height, sizeof(height));
    file.read((char *)&depth, sizeof(depth));
    file.read((char *)&format, sizeof(format));

    unsigned int bufferSize = width * height * depth;
    if (fileLength == (bufferSize + 4 * sizeof(unsigned int)))
    {
      // Read image data.
      char *buffer = new char [bufferSize];
      if (file.read(buffer, bufferSize))
      {
        // ToDo: Check if it's a known format.
        if (depth == 2)	// assume RGB565
        {
          // Convert to RGB888

          // This buffer is passed in to wxImage who will then own the memory.
          // It needs to be allocated with malloc, not new.
          unsigned char *rgb = (unsigned char *)malloc((width * height * 3) * sizeof(unsigned char));

          unsigned char *dst = rgb;
          unsigned short *src = (unsigned short *)buffer;

          for (unsigned int y = 0; y < height; ++y)
          {
            for (unsigned int x = 0; x < width; ++x)
            {
              unsigned short rgb565 = *src;

              unsigned char r5 = (rgb565 & 0xf800) >> 11;
              unsigned char g6 = (rgb565 & 0x07e0) >> 5;
              unsigned char b5 = (rgb565 & 0x001f) >> 0;

              dst[0] = (r5 << 3);	// ToDo: don't leave the last bits at zero, reuse bits from the number.
              dst[1] = (g6 << 2);
              dst[2] = (b5 << 3);

              dst += 3;
              src += 2;
            }
          }

          // Upload the image data.
          image.SetData(rgb, width, height);
          UpdateImage();

          // Clean up buffers.
          delete buffer;
        }
      }
    }
  }
}
