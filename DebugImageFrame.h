#ifndef DEBUG_IMAGE_FRAME_H
#define DEBUG_IMAGE_FRAME_H

#include "ImageFrame.h"
#include "DirNotifier.h"

class DebugImageFrame : public ImageFrame, public DirNotifierCallback
{
public:
  DebugImageFrame(wxFrame *parent);
  ~DebugImageFrame();

private:
  int			 id;
  DirNotifier	*dirNotifier;
  wxString	 fileName;

  virtual void DirNotifyCallback();
};

#endif // DEBUG_IMAGE_FRAME_H
