#ifndef IMAGE_DEBUG_LIB_H
#define IMAGE_DEBUG_LIB_H

#include <fstream>

enum ImageDebug_Format
{
	kUnknown,
	kRGBA8888,
	kRGB565
};

// ptr		:	raw pointer to the image data.
// width	:	number of pixels horizontally.
// height	:	number of pixel vertically.
// depth	:	number of bytes per pixel.
// format	:	pixel format if known, otherwise it's possible to configure this in the viewer.
inline void ImageDebug_Dump(void *ptr, unsigned int width, unsigned int height, unsigned int depth, unsigned int format = kUnknown)
{
	std::ofstream file("c:/tmp/debugimage/dbg.wiv", std::ios::binary);
	if (!file)
		return;

	// Write header.
	file.write((char *)&width, sizeof(width));
	file.write((char *)&height, sizeof(height));
	file.write((char *)&depth, sizeof(depth));
	file.write((char *)&format, sizeof(format));

	// Write the data.
	file.write((char *)ptr, width * height * depth);
}

// user defines:
#define IMAGEDUMP_VEGASWBUFFER(buf)	ImageDebug_Dump(buf.GetRawBufferPtr(), buf.width, buf.height, 2, kUnknown)

#endif // IMAGE_DEBUG_LIB_H
