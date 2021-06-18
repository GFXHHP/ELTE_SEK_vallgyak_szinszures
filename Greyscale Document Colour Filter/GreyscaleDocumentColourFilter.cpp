#include "Image.h"
#include <iostream>

int main(int args, char** cat)
{
    Image* peldaDok = new Image("peldaDok.bmp");

    /*Image* bookLossless = new Image("book.png.bmp");
    Image* bookLossy = new Image("book.jpg.bmp");*/

    //Background cutting:
    peldaDok->FindAndDeleteBackgroundInZones();
    //peldaDok->FindAndDeleteBackground();
    peldaDok->WriteGreyscale("peldaDok-backroundRemoved.bmp");
    peldaDok->TonerUsage();
    peldaDok->WriteFrequencyToCSV("peldaDok.csv");

    /*bookLossless->FindAndDeleteBackgroundInZones();
    bookLossless->WriteGreyscale("book-backroundRemoved.bmp");*/
    
    //Lossless - lossy comparison
    /*bookLossless->WriteFrequencyToCSV("lossless.csv");
    bookLossy->WriteFrequencyToCSV("lossy.csv");*/

    /*std::vector<unsigned int*> frequencies;
    frequencies.push_back(bookLossless->GetGreyScaleFrequency());
    frequencies.push_back(bookLossy->GetGreyScaleFrequency());
    Image::WriteFrequenciesToCSV("lossless-lossy.csv", frequencies);*/

    return 1;
}