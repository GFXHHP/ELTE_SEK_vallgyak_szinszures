#pragma once
#include "GreyPixel.h"

class RGBPixel
{
protected:
	/// <summary>
	/// Red value.
	/// </summary>
	unsigned char red;
	/// <summary>
	/// Green value.
	/// </summary>
	unsigned char green;
	/// <summary>
	/// Blue value.
	/// </summary>
	unsigned char blue;

	/// <summary>
	/// Converts colour from RGB colour space to greyscale.
	/// Uses the average formula.
	/// Red / 3 + Green / 3 + Blue / 3
	/// </summary>
	/// <returns>The luminance value as a single number between 0-255.</returns>
	unsigned char const RGBtoGreyAverage() const;
	/// <summary>
	/// Converts colour from RGB colour space to greyscale.
	/// Uses the weighted formula.
	/// 0.299 * Red + 0.587 * Green + 0.114 * Blue
	/// </summary>
	/// <returns>The luminance value as a single number between 0-255.</returns>
	unsigned char const RGBtoGreyWeighted() const;

public:
	/// <summary>
	/// Constructor. Default values: R - 0, G - 0, B - 0
	/// </summary>
	RGBPixel();
	/// <summary>
	/// Constructor. Sets the colour to be the one in parameters.
	/// </summary>
	/// <param name="red">Red component. 0-255</param>
	/// <param name="green">Green component. 0-255</param>
	/// <param name="blue">Blue component. 0-255</param>
	RGBPixel(const unsigned char red, const unsigned char green, const unsigned char blue);

	/// <summary>
	/// Sets the colour of the object.
	/// </summary>
	/// <param name="red">Red component. 0-255</param>
	/// <param name="green">Green component. 0-255</param>
	/// <param name="blue">Blue component. 0-255</param>
	void Set(const unsigned char red, const unsigned char green, const unsigned char blue);

	GreyPixel toGrey();
	/// <summary>
	/// Getter. Returns the value of red.
	/// </summary>
	/// <returns>The value of red. 0-255</returns>
	inline unsigned char const R() const { return Red(); }
	/// <summary>
	/// Getter. Returns the value of red.
	/// </summary>
	/// <returns>The value of red. 0-255</returns>
	unsigned char const Red() const;
	/// <summary>
	/// Getter. Returns the value of green.
	/// </summary>
	/// <returns>The value of green. 0-255</returns>
	inline unsigned char const G() const { return Green(); }
	/// <summary>
	/// Getter. Returns the value of green.
	/// </summary>
	/// <returns>The value of green. 0-255</returns>
	unsigned char const Green()const;
	/// <summary>
	/// Getter. Returns the value of blue.
	/// </summary>
	/// <returns>The value of blue. 0-255</returns>
	inline unsigned char const B() const { return Blue(); }
	/// <summary>
	/// Getter. Returns the value of blue.
	/// </summary>
	/// <returns>The value of blue. 0-255</returns>
	unsigned char const Blue()const;

	/// <summary>
	/// Setter. Sets the value of red.
	/// </summary>
	/// <returns>The value of red.</returns>
	inline unsigned char R(char red) { return Red(red); }
	/// <summary>
	/// Setter. Sets the value of red.
	/// </summary>
	/// <returns>The value of red.</returns>
	unsigned char Red(char red);
	/// <summary>
	/// Setter. Sets the value of green.
	/// </summary>
	/// <returns>The value of green.</returns>
	inline unsigned char G(char green) { return Green(green); }
	/// <summary>
	/// Setter. Sets the value of green.
	/// </summary>
	/// <returns>The value of green.</returns>
	unsigned char Green(char green);
	/// <summary>
	/// Setter. Sets the value of blue.
	/// </summary>
	/// <returns>The value of blue.</returns>
	inline unsigned char B(char blue) { return Blue(blue); }
	/// <summary>
	/// Setter. Sets the value of blue.
	/// </summary>
	/// <returns>The value of blue.</returns>
	unsigned char Blue(char blue);
	
	RGBPixel operator=(const RGBPixel& Rhs);
	bool operator==(const RGBPixel &Rhs);
	inline bool operator!=(const RGBPixel& Rhs) { return !(this == &Rhs); }

	inline static const RGBPixel White() { return RGBPixel(255, 255, 255); }
	inline static const RGBPixel Black() { return RGBPixel(0, 0, 0); }
};

