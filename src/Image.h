#pragma once

#include "Pixel.h"
#include <string>

class Image
{
	typedef unsigned char UCH;
	typedef unsigned int UINT;
	UINT width, height, rowsize, size, padding;
	Pixel* array;
public:
	Image();
	Image(UINT wArg, UINT hArg);
	Image(const Image& arg);
	~Image();
	Image operator=(const Image& arg);
	void Set();
	UINT GetWidth() const;
	UINT GetHeight() const;
	void Print() const;
	Pixel& GetPixel(UINT xArg, UINT yArg) const;
	void SetPixel(UINT xArg, UINT yArg, UINT rArg, UINT gArg, UINT bArg);
	void ChangePixel(UINT xArg, UINT yArg, UINT rArg, UINT gArg, UINT bArg);
	void CyclePixel(UINT xArg, UINT yArg, UINT rArg, UINT gArg, UINT bArg);
	void SetAll(UINT rArg, UINT gArg, UINT bArg);
	void Export(std::string fileName = "default") const;
};