#pragma once

#include "RGBPixel.h"
#include <vector>
#include <string>

class Image
{
private:
    /// <summary>
    /// Matrix of the rgb pixels of this image.
    /// </summary>
    RGBPixel** colourpixels = nullptr;
    /// <summary>
    /// Matrix of the greyscale pixels of this image.
    /// </summary>
    GreyPixel** greypixels = nullptr;

    unsigned long int height;
    unsigned long int width;

    std::string filePath = "";
    enum class IMAGEFORMAT { UNKNOWN = 0, BMP24 = 0x4D42} format = IMAGEFORMAT::UNKNOWN;
    //enum class COLOURSPACE { GREYSCALE, RGB } colourspace;

    /// <summary>
    /// This is used to calculate the initial toner usage value before any modifications.
    /// </summary>
    unsigned long long int pixelsum = 0;

    void initPixels();
    void initGreyscale();
    void initFrequency(unsigned int* &frequency);

    void ValidateDimensions(unsigned long int& minWidth, unsigned long int& minHeight, unsigned long int& maxWidth, unsigned long int& maxHeight);
        
    mutable char* fileBuffer = nullptr;
    mutable int bufferSize = 0;
public:
    inline Image(unsigned long int width = 0, unsigned long int height = 0)
    {
        this->width = width;
        this->height = height;
    }

    inline Image(std::string file)
    {
        height = width = 0;
        filePath = file;
        Read();
    }


    inline unsigned long int GetHeight() const { return height; }
    inline unsigned long int GetWidth() const { return width; }

    RGBPixel** GetColourPixels() const;
    GreyPixel** GetGreyPixels() const;

    /// <summary>
    /// Creates the greyscale pixel matrix for an RGB image.
    /// </summary>
    void RGBtoGreyscale();
    double TonerUsage();

    /// <summary>
    /// Returns an image of the specified rectangle of this image.
    /// </summary>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    /// <returns></returns>
    Image Crop(unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight) const;

    /// <summary>
    /// Returns an array containing the amount of pixels that are a certain colour. The array contains all the possible colours and the indices are the colour value.
    /// In case you don't want to get the colour spectrum of the entire image you can specify a rectangle inside the image.
    /// </summary>
    /// <param name="nameOfFileToCreate">The name (path) of the file to be created.</param>
    /// 
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    /// <returns>An unsigned int array of the amount of pixels that are a certain colour.</returns>
    unsigned int* GetGreyScaleFrequency(unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0);

    /// <summary>
    /// Removes the background (or precisely some of the background) of the greyscale image using global thresholding.
    /// </summary>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    void FindAndDeleteBackground(unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0);
    /// <summary>
    /// Removes the background (or precisely some of the background) of the greyscale image using local thresholding.
    /// The image is divided into several zones close to the size of zoneSize × zoneSize.
    /// </summary>
    /// <param name="zoneSize">The width and height of a single zone.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    void FindAndDeleteBackgroundInZones(int zoneSize = 100, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0);
    /// <summary>
    /// Removes the background (or precisely some of the background) of the greyscale image using local thresholding.
    /// The image is divided into zones amount of rectangular zones.
    /// </summary>
    /// <param name="zones">The amount of zones the image will be divided into.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    void FindAndDeleteBackgroundInZonesWithZoneAmount(int zones = 10000, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0);

    /// <summary>
    /// Sets all the given colour on the RGB image to white.
    /// Can be called to the entire image or a rectangle inside it can be specified.
    /// </summary>
    /// <param name="Colour">The RGBPixel colour that you want to set white.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    void CutOutColour(const RGBPixel Colour, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0);
    /// <summary>
    /// Sets all occurrences of the given colour on the greyscale image to white.
    /// Can be called to the entire image or a rectangle inside it can be specified.
    /// </summary>
    /// <param name="Colour">The RGBPixel colour that you want to set white.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    inline void CutOutGrey(RGBPixel Colour, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0) { CutOutGrey(Colour.toGrey(), minWidth, minHeight, maxWidth, maxHeight); }
    /// <summary>
    /// Returns a copy of the image with all the given grey shade set to white on the greyscale image.
    /// Can be called to the entire image or a rectangle inside it can be specified.
    /// Does not modify the object it was called on.
    /// </summary>
    /// <param name="Grey">The GreyPixel grey shade you want to set white.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    /// <returns>A new Image object with the modifications.</returns>
    Image CutOutGrey(const GreyPixel Grey, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0) const;
    /// <summary>
    /// Sets all occurrences of the given grey shade on the greyscale image to white.
    /// Can be called to the entire image or a rectangle inside it can be specified.
    /// </summary>
    /// <param name="Grey">The GreyPixel grey shade you want to set white.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    void CutOutGrey(const GreyPixel Grey, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0);
    /// <summary>
    /// Returns a copy of the image with all the given grey shades set to white on the greyscale image.
    /// Can be called to the entire image or a rectangle inside it can be specified.
    /// Does not modify the object it was called on.
    /// </summary>
    /// <param name="minGrey">The minimum GreyPixel grey shade you want to set white.</param>
    /// <param name="maxGrey">The maximum GreyPixel grey shade you want to set white.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    /// <returns>A new Image object with the modifications.</returns>
    Image CutOutGreys(const GreyPixel minGrey, const GreyPixel maxGrey, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0) const;
    /// <summary>
    /// Sets all occurrences of the given grey shades on the greyscale image to white.
    /// Can be called to the entire image or a rectangle inside it can be specified.
    /// </summary>
    /// <param name="minGrey">The minimum GreyPixel grey shade you want to set white.</param>
    /// <param name="maxGrey">The maximum GreyPixel grey shade you want to set white.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    void CutOutGreys(const GreyPixel minGrey, const GreyPixel maxGrey, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0);

    //Reads:
    bool Read();
    bool ReadBMP24();

    //Writes:
    //Colour
    /// <summary>
    /// Creates a file with the stored RGB pixels of this object.
    /// </summary>
    /// <param name="nameOfFileToCreate">The path of the file to be created.</param>
    /// <param name="format">The IMAGEFORMAT type of the image format to be created in. If unspecified it will use whatever the source image's format was.</param>
    /// <returns>true if file creation is successful, false otherwise.</returns>
    bool Write(std::string nameOfFileToCreate, IMAGEFORMAT format = IMAGEFORMAT::UNKNOWN) const;
    /// <summary>
    /// Writes the RGB image as a 24 bit bmp file.
    /// </summary>
    /// <param name="nameOfFileToCreate">The path and name of the file to write.</param>
    /// <returns>true if successfully created file. False otherwise.</returns>
    bool WriteBMP24(std::string nameOfFileToCreate) const;

    //Greyscale
    /// <summary>
    /// Creates a file with the stored greyscale pixels of this object.
    /// </summary>
    /// <param name="nameOfFileToCreate">The path of the file to be created.</param>
    /// <param name="format">The IMAGEFORMAT type of the image format to be created in. If unspecified it will use whatever the source image's format was.</param>
    /// <returns>true if file creation is successful, false otherwise.</returns>
    bool WriteGreyscale(std::string nameOfFileToCreate, IMAGEFORMAT format = IMAGEFORMAT::UNKNOWN);
    /// <summary>
    /// Writes the greyscale image as a 24 bit bmp file.
    /// </summary>
    /// <param name="nameOfFileToCreate">The path and name of the file to write.</param>
    /// <returns>true if successfully created file. False otherwise.</returns>
    bool WriteBMP24Greyscale(std::string nameOfFileToCreate);

    /// <summary>
    /// Writes the frequency of each greyshade on this image (or in a smaller part of the image) to a CSV file.
    /// </summary>
    /// <param name="nameOfFileToCreate">The name (path) of the file to be created.</param>
    /// <param name="groupEvery">Sums up the given number of values. for example: if this is 2 every 1st and 2nd value will be summed together and this will result in halving the amount of values in the file.</param>
    /// <param name="minWidth"> The width (x) position of the upper left corner of the custom rectangle.</param>
    /// <param name="minHeight">The height (y) position of the upper left corner of the custom rectangle.</param>
    /// <param name="maxWidth"> The width (x) position of the lower right corner of the custom rectangle.</param>
    /// <param name="maxHeight">The height (y) position of the lower right corner of the custom rectangle.</param>
    void WriteFrequencyToCSV(std::string nameOfFileToCreate, int groupEvery = 1, unsigned long int minWidth = 0, unsigned long int minHeight = 0, unsigned long int maxWidth = 0, unsigned long int maxHeight = 0);
    static void WriteFrequenciesToCSV(std::string nameOfFileToCreate, unsigned int** frequencies, unsigned int frequenciesSize, int groupEvery = 1);
    static void WriteFrequenciesToCSV(std::string nameOfFileToCreate, std::vector<unsigned int*> frequencies, int groupEvery = 1);

    /// <summary>
    /// Writes the inverted (white is 0, black is 255) greyscale values of the pixels in the given row between the given columns.
    /// </summary>
    /// <param name="nameOfFileToCreate">The path of the file to be created.</param>
    /// <param name="row">The row to write the values of.</param>
    /// <param name="minCol">Optional. The first column to be written.</param>
    /// <param name="maxCol">Optional. The last column to be written.</param>
    void WriteRowGreyShades(std::string nameOfFileToCreate, unsigned long int row, unsigned long int minCol = 0, unsigned long int maxCol = 0);

};

//BMP Headers:
#pragma pack(push, 1)
typedef int LONG;
typedef unsigned short WORD;
typedef unsigned int DWORD;

typedef struct tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, * PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, * PBITMAPINFOHEADER;
#pragma pack(pop)