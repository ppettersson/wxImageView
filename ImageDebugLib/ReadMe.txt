How to use:
===========

Inside the code where you want to debug images, add the following include:

	#include "path/to/ImageDebugLib.h"


Whenever you have an image pointer that you want to view, insert this call:

	ImageDebug_Dump(imagePtr, imageWidth, imageHeight, ImageDebug_RGB565);


To view the image keep the wxImageView program running and open a debug image view.


ToDo:
=====

Add optional exception/SEH trapping for bad image pointers.
