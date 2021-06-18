#include "RGBPixel.h"

unsigned char const RGBPixel::RGBtoGreyAverage() const
{
	return red / 3 + green / 3 + blue / 3;
}

unsigned char const RGBPixel::RGBtoGreyWeighted() const
{
	return char(0.2126 * red + 0.7152 * green + 0.0722 * blue);
}

RGBPixel::RGBPixel()
{
	Red(0);
	Green(0);
	Blue(0);
}

RGBPixel::RGBPixel(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	Red(red);
	Green(green);
	Blue(blue);
}

void RGBPixel::Set(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	Red(red);
	Green(green);
	Blue(blue);
}

GreyPixel RGBPixel::toGrey()
{
	//return GreyPixel(RGBtoGreyAverage());
	return GreyPixel(RGBtoGreyWeighted());
}

unsigned char const RGBPixel::Red() const
{
	return red;
}

unsigned char const RGBPixel::Green() const
{
	return green;
}

unsigned char const RGBPixel::Blue() const
{
	return blue;
}

unsigned char RGBPixel::Red(char red)
{
	return this->red = red;
}

unsigned char RGBPixel::Green(char green)
{
	return this->green = green;
}

unsigned char RGBPixel::Blue(char blue)
{
	return this->blue = blue;
}

RGBPixel RGBPixel::operator=(const RGBPixel& Rhs)
{
	red = Rhs.red;
	green = Rhs.green;
	blue = Rhs.blue;
	return *this;
}

bool RGBPixel::operator==(const RGBPixel& Rhs)
{
	return (red == Rhs.red) && (green == Rhs.green) && (blue == Rhs.blue);
}
