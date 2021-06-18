#pragma once

class GreyPixel
{
protected:
	/// <summary>
	/// Luminance value.
	/// </summary>
	unsigned char luminance;

public:
	/// <summary>
	/// Constructor. Default value: 0
	/// </summary>
	inline GreyPixel() { luminance = 0; }
	/// <summary>
	/// Constructor. Sets the luminance to be the one in parameters.
	/// </summary>
	/// <param name="luminance">Luminance. 0-255</param>
	inline GreyPixel(unsigned char luminance) { this->luminance = luminance; }

	static const unsigned char minValue = 0x00;
	static const unsigned char maxValue = 0xff;

	unsigned char SetLuminance(const unsigned char luminance);

	unsigned char GetLuminance() const;

	GreyPixel operator=(const GreyPixel& Rhs);
	bool operator==(const GreyPixel &Rhs);
	bool operator!=(const GreyPixel &Rhs) { return !(this == &Rhs); }

	inline static const GreyPixel White() { return GreyPixel(255); }
	inline static const GreyPixel Black() { return GreyPixel(0); }
};

