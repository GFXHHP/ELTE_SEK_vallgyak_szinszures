#include "Image.h"
#include <iostream>
#include <fstream>
#include <string>

//#define _USE_MATH_DEFINES
#include <math.h>

using std::ofstream;
using std::ifstream;

void Image::initPixels()
{
	colourpixels = new RGBPixel * [width];
	for (unsigned long int i = 0; i < width; i++) colourpixels[i] = new RGBPixel[height];
}

void Image::initGreyscale()
{
	pixelsum = 0;
	greypixels = new GreyPixel * [width];
	for (unsigned long int i = 0; i < width; i++) greypixels[i] = new GreyPixel[height];
}

void Image::initFrequency(unsigned int* &frequency)
{
	const int size = GreyPixel::maxValue + 1;
	frequency = new unsigned int[size];
	for (unsigned long int i = 0; i < size; i++)
	{
		frequency[i] = 0;
	}
}

void Image::ValidateDimensions(unsigned long int& minWidth, unsigned long int& minHeight, unsigned long int& maxWidth, unsigned long int& maxHeight)
{
	if (maxWidth == 0 || maxWidth <= minWidth || maxWidth > GetWidth()) maxWidth = GetWidth();
	if (maxHeight == 0 || maxHeight <= minHeight || maxHeight > GetHeight()) maxHeight = GetHeight();
}

RGBPixel** Image::GetColourPixels() const
{
	return colourpixels;
}

GreyPixel** Image::GetGreyPixels() const
{
	return greypixels;
}

void Image::RGBtoGreyscale()
{
	initGreyscale();
	for (unsigned long int i = 0; i < width; i++)
	{
		for (unsigned long int j = 0; j < height; j++)
		{
			greypixels[i][j] = colourpixels[i][j].toGrey();
			pixelsum += -1 * (long long)greypixels[i][j].GetLuminance() + GreyPixel::maxValue;
		}
	}
}

double Image::TonerUsage()
{
	unsigned long long int sum = 0;
	for (unsigned long int i = 0; i < GetWidth(); i++)
	{
		for (unsigned long int j = 0; j < GetHeight(); j++)
		{
			sum += -1 * (long long)greypixels[i][j].GetLuminance() + GreyPixel::maxValue;
		}
	}
	unsigned long long int difference = pixelsum - sum;
	double saved = double(difference) / GreyPixel::maxValue;

	std::cout << filePath + "" << ":\n" << "Toner units used for original image:\t\t\t" << (double)pixelsum / GreyPixel::maxValue << "\n"
		<< "Toner units used after removing backround pixels:\t" << (double)sum / GreyPixel::maxValue << "\n"
		<< "Difference in toner units:\t\t\t\t" << saved << "\n"
		<< "Difference in percentage:\t\t\t\t" << ((double)difference / pixelsum) * 100 << "%" << std::endl;

	return saved;
}

Image Image::Crop(unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight) const
{
	if (maxWidth > width)
	{
		//TODO: error handling
		return NULL;
	}
	if (maxHeight > height)
	{
		//TODO: error handling
		return NULL;
	}

	unsigned long int cropWidth = maxWidth - minWidth;
	unsigned long int cropHeight = maxHeight - minHeight;

	Image Crop = Image(cropWidth, cropHeight);
	Crop.format = this->format;
	Crop.initPixels();
	RGBPixel** pixelArray = Crop.GetColourPixels();
	for (unsigned long int i = 0; i < Crop.GetWidth(); i++)
	{
		for (unsigned long int j = 0; j < Crop.GetHeight(); j++)
		{
			pixelArray[i][j] = colourpixels[minWidth + i][minHeight + j];
		}
	}

	return Crop;
}

unsigned int* Image::GetGreyScaleFrequency(unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight)
{
	if (greypixels == nullptr) RGBtoGreyscale();
	unsigned int* frequency = nullptr;
	initFrequency(frequency);
	ValidateDimensions(minWidth, minHeight, maxWidth, maxHeight);
	for (unsigned long int i = minWidth; i < maxWidth; i++)
	{
		for (unsigned long int j = minHeight; j < maxHeight; j++)
		{
			unsigned char idx = greypixels[i][j].GetLuminance();
			frequency[idx]++;
		}
	}
	return frequency;
}

void Image::FindAndDeleteBackground(unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight)
{
	ValidateDimensions(minWidth, minHeight, maxWidth, maxHeight);
	
	unsigned int* frequency = GetGreyScaleFrequency(minWidth, minHeight, maxWidth, maxHeight);

	//Last local maximum:
	/*unsigned int maxIdx = GreyPixel::White().GetLuminance() - 1;
	while (maxIdx > 1 && frequency[maxIdx] <= frequency[maxIdx - 1])
	{
		maxIdx--;
	}*/

	//"Global" maximum between 150 and 250:
	unsigned int maxIdx = 150;
	for (int i = maxIdx + 1; i < 250; i++)
	{
		if (frequency[i] > frequency[maxIdx]) maxIdx = i;
	}

	//Calculating the start of the interval that will be set to white:
	//start = percent% of maxIdx
	const double percent = 0.15;
	//const double percent = 0.015;
	//About 1.5% is the best result we got for peldaDok.bmp
	unsigned int startIdx = maxIdx;
	while (frequency[startIdx] > (frequency[maxIdx] * percent) && startIdx > 0) startIdx--;
	const GreyPixel start = GreyPixel(startIdx);

	//Symmetrical: max is the center point of the interval
	//const GreyPixel start = GreyPixel(maxIdx - (GreyPixel::White().GetLuminance() - maxIdx));
	
	//Asymmetrical: start = maxIdx/divisor
	/*const double divisor = 2;
	const GreyPixel start = GreyPixel((maxIdx - (GreyPixel::White().GetLuminance() - maxIdx/divisor)));*/

	CutOutGreys(start, GreyPixel::White(), minWidth, minHeight, maxWidth, maxHeight);
}

void Image::FindAndDeleteBackgroundInZones(int zoneSize, unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight)
{
	ValidateDimensions(minWidth, minHeight, maxWidth, maxHeight);

	unsigned long int cols = (maxWidth - minWidth) / zoneSize;
	unsigned long int rows = (maxHeight - minHeight) / zoneSize;

	double zoneWidth = double(maxWidth - minWidth) / cols;
	double zoneHeight = double(maxHeight - minHeight) / rows;

	for (unsigned long int i = 0; i < cols; i++)
	{
		unsigned long int startWidth = floor((double)i * zoneWidth);
		unsigned long int stopWidth = floor(double(i + 1) * zoneWidth);
		for (unsigned long int j = 0; j < rows; j++)
		{
			unsigned long int startHeight = floor((double)j * zoneHeight);
			unsigned long int stopHeight = floor(double(j + 1) * zoneHeight);

			FindAndDeleteBackground(startWidth, startHeight, stopWidth, stopHeight);
		}
	}
}

void Image::FindAndDeleteBackgroundInZonesWithZoneAmount(int zones, unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight)
{
	ValidateDimensions(minWidth, minHeight, maxWidth, maxHeight);

	unsigned long long int imageArea = (long long)(maxWidth - minWidth) * (long long)(maxHeight - minHeight);
	double zoneArea = (double)imageArea / zones;

	double zoneSize = sqrt(zoneArea);

	unsigned long int cols = (maxWidth - minWidth) / zoneSize;
	unsigned long int rows = (maxHeight - minHeight) / zoneSize;

	double zoneWidth = double(maxWidth - minWidth) / cols;
	double zoneHeight = double(maxHeight - minHeight) / rows;

	for (unsigned long int i = 0; i < cols; i++)
	{
		unsigned long int startWidth = floor((double)i * zoneWidth);
		unsigned long int stopWidth = floor(double(i + 1) * zoneWidth);
		for (unsigned long int j = 0; j < rows; j++)
		{
			unsigned long int startHeight = floor((double)j * zoneHeight);
			unsigned long int stopHeight = floor(double(j + 1) * zoneHeight);

			FindAndDeleteBackground(startWidth, startHeight, stopWidth, stopHeight);
		}
	}
}

void Image::CutOutColour(const RGBPixel Colour, unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight)
{
	ValidateDimensions(minWidth, minHeight, maxWidth, maxHeight);

	for (unsigned int i = minWidth; i < maxWidth; i++)
	{
		for (unsigned int j = minHeight; j < maxHeight; j++)
		{
			if (colourpixels[i][j] == Colour) colourpixels[i][j] = RGBPixel::White();
		}
	}
}

Image Image::CutOutGrey(const GreyPixel Grey, unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight) const
{
	Image Cut = Image(width,height);
	Cut.initPixels();
	for (unsigned long int i = 0; i < Cut.GetWidth(); i++)
	{
		for (unsigned long int j = 0; j < Cut.GetHeight(); j++)
		{
			Cut.GetColourPixels()[i][j] = colourpixels[i][j];
		}
	}
	Cut.RGBtoGreyscale();
	Cut.CutOutGrey(Grey, minWidth, minHeight, maxWidth, maxHeight);
	return Cut;
}

void Image::CutOutGrey(const GreyPixel Grey, unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight)
{
	ValidateDimensions(minWidth, minHeight, maxWidth, maxHeight);

	if (greypixels == nullptr) RGBtoGreyscale();

	if (Grey.GetLuminance() != GreyPixel::maxValue)
	{
		for (unsigned int i = minWidth; i < maxWidth; i++)
		{
			for (unsigned int j = minHeight; j < maxHeight; j++)
			{
				if (greypixels[i][j] == Grey) greypixels[i][j] = GreyPixel::White();
			}
		}
	}
}

Image Image::CutOutGreys(const GreyPixel minGrey, const GreyPixel maxGrey, unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight) const
{
	Image Cut = Image(width, height);
	Cut.initPixels();
	for (unsigned long int i = 0; i < Cut.GetWidth(); i++)
	{
		for (unsigned long int j = 0; j < Cut.GetHeight(); j++)
		{
			Cut.GetColourPixels()[i][j] = colourpixels[i][j];
		}
	}
	Cut.RGBtoGreyscale();
	Cut.CutOutGreys(minGrey, maxGrey, minWidth, minHeight, maxWidth, maxHeight);
	return Cut;
}

void Image::CutOutGreys(const GreyPixel minGrey, const GreyPixel maxGrey, unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight)
{
	unsigned char min = minGrey.GetLuminance();
	unsigned char max = maxGrey.GetLuminance();
	if (max < min)
	{
		unsigned char tmp = min;
		min= max;
		max= tmp;
	}
	if (max == GreyPixel::maxValue) max = GreyPixel::maxValue - 1;
	for (unsigned char i = min; i <= max; i++)
	{
		CutOutGrey(GreyPixel(i), minWidth, minHeight, maxWidth, maxHeight);
	}
}

bool Image::Read()
{
	std::string fileext = filePath.substr(filePath.find_last_of('.') + 1);
	if (fileext == "bmp") return ReadBMP24();
	std::cout << "Could not get file extension or not supported." << std::endl;
	return false;
}

bool Image::ReadBMP24()
{
	std::ifstream file(filePath, std::ios::binary);

	if (file) {
		//Gets how many bytes there are in the stream.
		file.seekg(0, std::ios::end);           //Sets current character in the stream to the last character(byte).
		std::streampos length = file.tellg();   //Returns the position of the current character in the input stream.
		file.seekg(0, std::ios::beg);           //Sets current character in the stream back to the first byte.

		fileBuffer = new char[length];
		file.read(&fileBuffer[0], length);

		PBITMAPFILEHEADER file_header;
		PBITMAPINFOHEADER info_header;

		file_header = (PBITMAPFILEHEADER)(&fileBuffer[0]);                              //Reads the Bitmap file header (beginning of the file).
		info_header = (PBITMAPINFOHEADER)(&fileBuffer[0] + sizeof(BITMAPFILEHEADER));   //Reads the Bitmap info header (right after file header).
		height = info_header->biHeight;
		width = info_header->biWidth;
		bufferSize = file_header->bfSize;

		std::cout << "BMP Headers read.\n";
		format = IMAGEFORMAT::BMP24;

		initPixels();

		int count = 1;			// Because this will be substracted from the length to get an index
		int extra = width % 4;   // The nubmer of bytes in a row (the number of columns) will be a multiple of 4.
		for (unsigned long int i = 0; i < height; i++)
		{
			count += extra;     // Because of the padding at the end of the row.
			for (unsigned long int j = width - 1; j < -1; j--)
			{
				for (int k = 0; k < 3; k++)	//The 3 colours of 1 pixel
				{
					//Since BMP has a colour order of Blue, Green, Red and we're reading from the end of the row to the beginning red comes first, followed by green, and finally blue.
					switch (k) {
					case 0: //red
						colourpixels[j][i].Red(fileBuffer[bufferSize - count++]);
						break;
					case 1: //green
						colourpixels[j][i].Green(fileBuffer[bufferSize - count++]);
						break;
					case 2: //blue
						colourpixels[j][i].Blue(fileBuffer[bufferSize - count++]);
						break;
					}
				}
			}
		}
		std::cout << filePath << " pixel information read." << std::endl;
		return true;
	}
	else {
		std::cout << "File " << filePath << " does not exist!" << std::endl;
		return false;
	}
}

bool Image::Write(std::string nameOfFileToCreate, IMAGEFORMAT format) const
{
	if (format == IMAGEFORMAT::UNKNOWN) format = this->format;
	switch (format)
	{
	case Image::IMAGEFORMAT::BMP24:
		return WriteBMP24(nameOfFileToCreate);
		break;
	default:
	case Image::IMAGEFORMAT::UNKNOWN:
		std::cerr << "format error" << std::endl;
		return false;
		break;
	}
}

bool Image::WriteBMP24(std::string nameOfFileToCreate) const
{
	std::ofstream write(nameOfFileToCreate, std::ios::binary);
	if (!write)
	{
		std::cout << "Failed to write " << nameOfFileToCreate << std::endl;
		return false;
	}
	if (fileBuffer == nullptr)
	{
		bufferSize = (width * 3 + width%4) * height + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		BITMAPFILEHEADER file_header;
		file_header.bfType = 0x4D42;
		file_header.bfSize = bufferSize;
		file_header.bfReserved1 = 0;
		file_header.bfReserved2 = 0;
		file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER info_header;
		info_header.biSize = sizeof(BITMAPINFOHEADER);
		info_header.biWidth = width;
		info_header.biHeight = height;
		info_header.biPlanes = 1;
		info_header.biBitCount = 8 * 3;
		info_header.biCompression = 0;
		info_header.biSizeImage = bufferSize - file_header.bfOffBits;
		info_header.biXPelsPerMeter = 0;
		info_header.biYPelsPerMeter = 0;
		info_header.biClrUsed = 0;
		info_header.biClrImportant = 0;

		fileBuffer = new char[bufferSize];
		for (int i = 0; i < sizeof(BITMAPFILEHEADER); i++)
		{
			fileBuffer[i] = ((char*)&file_header)[i];
		}
		for (int i = 0; i < sizeof(BITMAPINFOHEADER); i++)
		{
			fileBuffer[sizeof(BITMAPFILEHEADER) + i] = ((char*)&info_header)[i];
		}
		char debug = 'd';
	}
	
	int count = 1;
	int extra = width % 4;   // The nubmer of bytes in a row (width) will be a multiple of 4.
	for (unsigned long int i = 0; i < height; i++)
	{
		count += extra;     // Because of the padding.
		for (unsigned long int j = width - 1; j < -1; j--)
		{
			for (int k = 0; k < 3; k++)
			{
				switch (k) {
				case 0: //red
					fileBuffer[bufferSize - count] = colourpixels[j][i].R();
					break;
				case 1: //green
					fileBuffer[bufferSize - count] = colourpixels[j][i].G();
					break;
				case 2: //blue
					fileBuffer[bufferSize - count] = colourpixels[j][i].B();
					break;
				}
				count++;
			}
		}
	}
	write.write(fileBuffer, bufferSize);
	write.close();
	std::cout << nameOfFileToCreate << " file created." << std::endl;
	return true;
}

bool Image::WriteGreyscale(std::string nameOfFileToCreate, IMAGEFORMAT format)
{
	if (format == IMAGEFORMAT::UNKNOWN) format = this->format;
	if (greypixels == nullptr) RGBtoGreyscale();
	switch (format)
	{
	case Image::IMAGEFORMAT::BMP24:
		return WriteBMP24Greyscale(nameOfFileToCreate);
		break;
	default:
	case Image::IMAGEFORMAT::UNKNOWN:
		std::cerr << "format error" << std::endl;
		return false;
		break;
	}
}

bool Image::WriteBMP24Greyscale(std::string nameOfFileToCreate)
{
	std::ofstream write(nameOfFileToCreate, std::ios::binary);
	if (!write)
	{
		std::cout << "Failed to write " << nameOfFileToCreate << std::endl;
		return false;
	}
	if (fileBuffer == nullptr)
	{
		bufferSize = (width * 3 + width % 4) * height + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		BITMAPFILEHEADER file_header;
		file_header.bfType = 0x4D42;
		file_header.bfSize = bufferSize;
		file_header.bfReserved1 = 0;
		file_header.bfReserved2 = 0;
		file_header.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		BITMAPINFOHEADER info_header;
		info_header.biSize = sizeof(BITMAPINFOHEADER);
		info_header.biWidth = width;
		info_header.biHeight = height;
		info_header.biPlanes = 1;
		info_header.biBitCount = 8 * 3;
		info_header.biCompression = 0;
		info_header.biSizeImage = bufferSize - file_header.bfOffBits;
		info_header.biXPelsPerMeter = 0;
		info_header.biYPelsPerMeter = 0;
		info_header.biClrUsed = 0;
		info_header.biClrImportant = 0;

		fileBuffer = new char[bufferSize];
		for (int i = 0; i < sizeof(BITMAPFILEHEADER); i++)
		{
			fileBuffer[i] = ((char*)&file_header)[i];
		}
		for (int i = 0; i < sizeof(BITMAPINFOHEADER); i++)
		{
			fileBuffer[sizeof(BITMAPFILEHEADER) + i] = ((char*)&info_header)[i];
		}
		char debug = 'd';
	}
	int count = 1;
	int extra = width % 4;   // The nubmer of bytes in a row (width) will be a multiple of 4.
	for (unsigned long int i = 0; i < height; i++)
	{
		count += extra;     // Because of the padding.
		for (int j = width - 1; j >= 0; j--)
		{
			//red
			fileBuffer[bufferSize - count] = greypixels[j][i].GetLuminance();
			count++;
			//green
			fileBuffer[bufferSize - count] = greypixels[j][i].GetLuminance();
			count++;
			//blue
			fileBuffer[bufferSize - count] = greypixels[j][i].GetLuminance();
			count++;
		}
	}
	write.write(fileBuffer, bufferSize);
	write.close();
	std::cout << nameOfFileToCreate << " file created." << std::endl;
	return true;
}

void Image::WriteFrequencyToCSV(std::string nameOfFileToCreate, int groupEvery, unsigned long int minWidth, unsigned long int minHeight, unsigned long int maxWidth, unsigned long int maxHeight)
{
	std::ofstream file;
	file.open(nameOfFileToCreate, std::ios_base::out);
	ValidateDimensions(minWidth, minHeight, maxWidth, maxHeight);
	unsigned int* frequency = GetGreyScaleFrequency(minWidth, minHeight, maxWidth, maxHeight);

	file << "Grey shade" << ",";
	file << "Count" << "\n";

	for (int i = 0; i < GreyPixel::maxValue + 1; i += groupEvery)
	{
		unsigned int sum = 0;
		for (int j = 0; j < groupEvery && i + j < GreyPixel::maxValue + 1; j++)
		{
			sum += frequency[i + j];
		}
		file << std::to_string(i) << ",";
		file << std::to_string(sum) << "\n";
	}

	file.close();
}

void Image::WriteFrequenciesToCSV(std::string nameOfFileToCreate, unsigned int** frequencies, const unsigned int frequenciesSize, const int groupEvery)
{
	std::ofstream file;
	file.open(nameOfFileToCreate, std::ios_base::out);
	if (frequencies == nullptr) return;

	file << "Grey shade";
	for (unsigned int i = 0; i < frequenciesSize; i++) file << ",Count";
	file << "\n";

	for (int i = 0; i < GreyPixel::maxValue + 1; i += groupEvery)
	{
		file << std::to_string(i) << ",";
		for (unsigned int j = 0; j < frequenciesSize; j++)
		{
			unsigned long int sum = 0;
			for (int k = 0; k < groupEvery && i + k < GreyPixel::maxValue + 1; k++)
			{
				sum += frequencies[j][i + k];
			}
			file << std::to_string(sum) << ",";
		}
		file << "\n";
	}

	file.close();
}

void Image::WriteFrequenciesToCSV(std::string nameOfFileToCreate, std::vector<unsigned int*> frequencies, int groupEvery)
{
	std::ofstream file;
	file.open(nameOfFileToCreate, std::ios_base::out);

	file << "Grey shade";
	for (unsigned int i = 0; i < frequencies.size(); i++) file << ",Count";
	file << "\n";

	for (int i = 0; i < GreyPixel::maxValue + 1; i += groupEvery)
	{
		file << std::to_string(i) << ",";
		for (unsigned int j = 0; j < frequencies.size(); j++)
		{
			unsigned long int sum = 0;
			for (int k = 0; k < groupEvery && i + k < GreyPixel::maxValue + 1; k++)
			{
				sum += frequencies.at(j)[i + k];
			}
			file << std::to_string(sum) << ",";
		}
		file << "\n";
	}

	file.close();
}

void Image::WriteRowGreyShades(std::string nameOfFileToCreate, unsigned long int row, unsigned long int minCol, unsigned long int maxCol)
{
	if (maxCol == 0 || maxCol == minCol) maxCol = GetWidth();

	if (greypixels == nullptr) RGBtoGreyscale();

	std::ofstream file;
	file.open(nameOfFileToCreate, std::ios_base::out);

	for (unsigned long int i = minCol; i < maxCol; i++)
	{
		long long greylvl = (-1 * (long long)(greypixels[row][i].GetLuminance()) + GreyPixel::maxValue);
		file << std::to_string(greylvl) + "\n";
	}

	file.close();
}
