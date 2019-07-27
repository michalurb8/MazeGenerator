#pragma once
#include <ostream>

class Pixel
{
	typedef unsigned char UCH;
	typedef unsigned int UINT;
	UCH R, G, B;
public:
	Pixel();
	Pixel(UCH rArg, UCH gArg, UCH bArg);
	Pixel SetColor(UCH rArg, UCH gArg, UCH bArg);
	Pixel ChangeColor(int rArg, int gArg, int bArg);
	Pixel CycleColor(int rArg, int gArg, int bArg);
	Pixel SetRed(UCH rArg);
	Pixel SetGreen(UCH gArg);
	Pixel SetBlue(UCH bArg);
	UCH GetRed() const;
	UCH GetGreen() const;
	UCH GetBlue() const;
	UINT GetRedASCII() const;
	UINT GetGreenASCII() const;
	UINT GetBlueASCII() const;
	void Print() const;
	friend std::ostream& operator<<(std::ostream& ss, const Pixel& arg);
};

std::ostream& operator<<(std::ostream& ss, const Pixel& arg);