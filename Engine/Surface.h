#pragma once

#include "Colors.h"
#include <string>
#include "Rect.h"

//Surface dude = Surface("Images//link90x90.bmp");
//https://online-converting.com/image/convert2bmp/

class Surface
{
public:
	Surface(const std::string& filename);
	Surface(int width, int height);
	~Surface();
	Surface(const Surface&);
	Surface operator=(const Surface&);

	int GetWidth() const;
	int GetHeight() const;
	RectI GetRect() const;
	Color GetPixel(int x, int y) const;
	void putPixel(int x, int y, Color c);
	

	
private:
	int width;
	int height;
	Color* pPixels = nullptr;
};