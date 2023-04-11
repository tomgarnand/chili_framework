#include "Surface.h"
#include "ChiliWin.h"
#include <cassert>
#include <fstream>


Surface::Surface(const std::string& filename)
{
	std::ifstream file(filename, std::ios::binary);
	assert(file);
	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader),sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biCompression == BI_RGB);

	width = bmInfoHeader.biWidth;
	height = bmInfoHeader.biHeight;

	int yStart;
	int yEnd;
	int dy;

	if (height < 0)
	{
		height = -height;
		yStart = 0;
		yEnd = height;
		dy = 1;
		
	}
	else
	{
		yStart = height - 1;
		yEnd = 0;
		dy = -1;
	}
	
	pPixels = new Color[width * height];

	file.seekg(bmFileHeader.bfOffBits);
	const int padding = (4 - (width * 3) % 4) % 4;



	for (int y = yStart; y != yEnd ; y += dy)
	{
		for (int x = 0; x < width; x++)
		{
			putPixel(x, y , Color( file.get(), file.get(), file.get() ));
			if (bmInfoHeader.biBitCount == 32) { file.seekg(1, std::ios::cur); } //add logic for transparency pixel
		}
		file.seekg(padding, std::ios::cur);
	}

}

Surface::Surface(int width, int height)
	:
	width(width),
	height(height),
	pPixels(new Color[width * height])
{
}

Surface::~Surface()
{
	delete [] pPixels;
	pPixels = nullptr;
}

Surface::Surface(const Surface& rhs)
	:
	Surface(rhs.width, rhs.height)
{
	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++)
	{
		pPixels[i] = rhs.pPixels[i];
	}
}

Surface Surface::operator=(const Surface& rhs)
{
	width = rhs.width;
	height = rhs.height;

	delete[] pPixels;
	pPixels = new Color[width * height];

	const int nPixels = width * height;
	for (int i = 0; i < nPixels; i++)
	{
		pPixels[i] = rhs.pPixels[i];
	}

	return *this;
}

int Surface::GetWidth() const
{
	return width;
}

int Surface::GetHeight() const
{
	return height;
}

RectI Surface::GetRect() const
{
	return RectI(0, width, 0, height);
}

Color Surface::GetPixel(int x, int y) const
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	return pPixels[y * width + x];
}

void Surface::putPixel(int x, int y, Color c)
{
	assert(x >= 0);
	assert(x < width);
	assert(y >= 0);
	assert(y < height);
	pPixels[y * width + x] = c;
}
