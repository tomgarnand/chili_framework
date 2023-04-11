#pragma once

#include "Colors.h"

class Surface
{
public:
	Surface(int width, int height);
	~Surface();
	Surface(const Surface&);
	Surface operator=(const Surface&);

	int GetWidth() const;
	int GetHeight() const;
	Color GetPixel(int x, int y) const;
	void putPixel(int x, int y, Color c);


	
private:
	int width;
	int height;
	Color* pPixels = nullptr;
};