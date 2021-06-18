#include "GreyPixel.h"

unsigned char GreyPixel::SetLuminance(const unsigned char luminance)
{
	return this->luminance = luminance;
}

unsigned char GreyPixel::GetLuminance() const
{
	return luminance;
}

GreyPixel GreyPixel::operator=(const GreyPixel& Rhs)
{
	luminance = Rhs.luminance;
	return *this;
}

bool GreyPixel::operator==(const GreyPixel &Rhs)
{
	return luminance == (Rhs).luminance;
}
