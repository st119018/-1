#include <iostream>
#include <fstream>
/* Загловок не должен называться просто "заголовок" */
#include "Header.h"
/* Так делать не стоит из-за конфликтов имен. Пусть у тебя есть функция с именем как у функции, 
 * которая орпеделена в стд. Тогда при вызове узнать, какая из эти двух функций на самом деле 
 * вызывается будет сложно. А вот если бы функцию из стд ты вызывал std::func(), то такой проблемы
 * возникнуть не может */
using namespace std;
/* Следует разбивать программу на файлы. Во первых, просто ради
 * организации кода. Во вторых, чтобы была возможность проводить
 * раздельную компиляцию. Стоит помещать каждый класс\структуру 
 * в отдельный .h файл, а определения этих методов в .cpp. Тогда
 * при изменении реализации какой-то функции придется перекомпили-
 * ровать только этот .cpp файл. Если определение было в заголовке,
 * то придется перекомпилировать все файлы, в которые включен заголо-
 * вок (в крупном проекте это может сократить время компиляции на
 * несколько часов) */
/* Программа хорошо ложится на использование класса картинки. То есть
 * у нас мог бы быть класс BMP, который содержит все эти методы,
 * в приватных полях хранит указатель на пиксели и BMPHeader. При 
 * изменении картинки создает новый экземпляр картинки. Тогда мы бы 
 * имели более безопасную программу за счет инкапсуляции. */
int main() {
    setlocale(LC_CTYPE, "rus");

    const char* inputfile = "sample.bmp";
    const char* outputfile_counterclockwise = "bmpRotate(counterclockwise).bmp";
    const char* outputfile_clockwise = "bmpRotate(clockwise).bmp";
    const char* outputfile_GaussianFilter = "bmp(GaussianFilter).bmp";

    ifstream myPicture(inputfile, ifstream::binary);
    if (!myPicture.is_open()) {
        cout << "File opening error. Try again or enter a different name.";
        myPicture.close();
        return 0;
    }
    /* Ну а вот 100 картнок скажу прочитать, 100 раз скопируешь? */
    //reading header
    BITMAPFILEHEADER myHeader;
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    //checking the conditions associated with the header
    if (myHeader.bfType != 0x4d42) {
        /* Ошибки выводятся в cerr */
        cout << "This file is not a BMP file." << endl;
        myPicture.close();
        return 0;
    }

    //reading the information
    BITMAPINFOHEADER myInfo;
    /* Красть код у одногруппников надо осторожно, особенно такой опознаваемый. 
     * Прошу то же, что и там. Сделай это одним чтением. */
    myPicture.read(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biWidth), sizeof(myInfo.biWidth));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biPlanes), sizeof(myInfo.biPlanes));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biBitCount), sizeof(myInfo.biBitCount));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biCompression), sizeof(myInfo.biCompression));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biSizeImage), sizeof(myInfo.biSizeImage));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biXPelsPerMeter), sizeof(myInfo.biXPelsPerMeter));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biYPelsPerMeter), sizeof(myInfo.biYPelsPerMeter));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biClrUsed), sizeof(myInfo.biClrUsed));
    myPicture.read(reinterpret_cast<char*>(&myInfo.biClrImportant), sizeof(myInfo.biClrImportant));
    /* Тебе эти данные не нужны в принципе так-то. Их можно в одну переменную читать */
    if (myInfo.biSize == 108 || myInfo.biSize == 124)
    {
        myPicture.read(reinterpret_cast<char*>(&myInfo.biRedMask), sizeof(myInfo.biRedMask));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biGreenMask), sizeof(myInfo.biGreenMask));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biBlueMask), sizeof(myInfo.biBlueMask));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biAlphaMask), sizeof(myInfo.biAlphaMask));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biCSType), sizeof(myInfo.biCSType));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biEndpoints), sizeof(myInfo.biEndpoints));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biGammaRed), sizeof(myInfo.biGammaRed));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biGammaGreen), sizeof(myInfo.biGammaGreen));
        myPicture.read(reinterpret_cast<char*>(&myInfo.biGammaBlue), sizeof(myInfo.biGammaBlue));
        if (myInfo.biSize == 124) {
            myPicture.read(reinterpret_cast<char*>(&myInfo.biIntent), sizeof(myInfo.biIntent));
            myPicture.read(reinterpret_cast<char*>(&myInfo.biProfileData), sizeof(myInfo.biProfileData));
            myPicture.read(reinterpret_cast<char*>(&myInfo.biProfileSize), sizeof(myInfo.biProfileSize));
            myPicture.read(reinterpret_cast<char*>(&myInfo.biReserved), sizeof(myInfo.biReserved));
        }
    }
    //checking the conditions
    if ((myInfo.biSize != 40 && myInfo.biSize != 108 && myInfo.biSize != 124) ||
        myHeader.bfReserved1 != 0 ||
        myHeader.bfReserved2 != 0 ||
        myInfo.biPlanes != 1 ||
        myInfo.biCompression != 0 ||  //only uncompressed images
        myInfo.biBitCount != 24)
    { /* А где else вообще? */
        cout << "Unsupported BMP format." << endl;
        myPicture.close();
        return 0;
    }
    /* Формулы какие-то очень неинтуитивные. Если хочешь считать паддинг
     * попробуй воспользоваться взятием остатка. Потому что манипуляции
     * с делением и умножением на одно и то же выглядят как шаманство */
    //calculating the width, taking into account the alignment
    int bytesPerPixel = myInfo.biBitCount / 8;
    int alignment = 4;                                         // alignment
    int stride = (myInfo.biWidth * bytesPerPixel) + (alignment - 1);
    stride /= alignment;
    stride *= alignment;                       // width, multiple of 4
    int padding = stride - myInfo.biWidth * bytesPerPixel;

    //creating a buffer
    unsigned char* buffer = new unsigned char[stride * myInfo.biHeight];

    cout << "The amount of memory allocated for image loading is " << stride * myInfo.biHeight << " bytes." << endl;

    //reading to the buffer
    myPicture.read(reinterpret_cast<char*> (buffer), stride * myInfo.biHeight);
    myPicture.close();


    // creating a new buffer for the rotated image
    int newPadding = ((myInfo.biHeight * bytesPerPixel + (alignment - 1)) / alignment) * alignment - myInfo.biHeight * bytesPerPixel;
    int newStride = myInfo.biHeight * bytesPerPixel + newPadding;                 // new width multiple of 4
    unsigned char* newBuffer = new unsigned char[myInfo.biWidth * newStride];
    char null = buffer[0];                                                        // memorizing the zero element
    myHeader.bfSize = 14 + myInfo.biSize + myInfo.biWidth * newStride;            // counting the size of new file with the rotated image

    // opening the file for the image rotated counterclockwise
    BITMAPFILEHEADER myNewHeader;
    /* Просто жесть... */
    ofstream myNewPicture(outputfile_counterclockwise, ofstream::binary);
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biWidth), sizeof(myInfo.biWidth));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biPlanes), sizeof(myInfo.biPlanes));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biBitCount), sizeof(myInfo.biBitCount));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biCompression), sizeof(myInfo.biCompression));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biSizeImage), sizeof(myInfo.biSizeImage));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biXPelsPerMeter), sizeof(myInfo.biXPelsPerMeter));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biYPelsPerMeter), sizeof(myInfo.biYPelsPerMeter));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biClrUsed), sizeof(myInfo.biClrUsed));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biClrImportant), sizeof(myInfo.biClrImportant));
    if (myInfo.biSize == 108 || myInfo.biSize == 124)
    {
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biRedMask), sizeof(myInfo.biRedMask));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biGreenMask), sizeof(myInfo.biGreenMask));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biBlueMask), sizeof(myInfo.biBlueMask));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biAlphaMask), sizeof(myInfo.biAlphaMask));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biCSType), sizeof(myInfo.biCSType));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biEndpoints), sizeof(myInfo.biEndpoints));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biGammaRed), sizeof(myInfo.biGammaRed));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biGammaGreen), sizeof(myInfo.biGammaGreen));
        myNewPicture.write(reinterpret_cast<char*>(&myInfo.biGammaBlue), sizeof(myInfo.biGammaBlue));
        if (myInfo.biSize == 124) {
            myNewPicture.write(reinterpret_cast<char*>(&myInfo.biIntent), sizeof(myInfo.biIntent));
            myNewPicture.write(reinterpret_cast<char*>(&myInfo.biProfileData), sizeof(myInfo.biProfileData));
            myNewPicture.write(reinterpret_cast<char*>(&myInfo.biProfileSize), sizeof(myInfo.biProfileSize));
            myNewPicture.write(reinterpret_cast<char*>(&myInfo.biReserved), sizeof(myInfo.biReserved));
        }
    }

    // rotating pixels counterclockwise
    for (int i = 0; i < myInfo.biHeight; i++) {
        for (int j = 0; j < myInfo.biWidth; j++) {

            int oldIndex = i * myInfo.biWidth + j;                                            // counting the buffer pixel index
            int newIndex = (myInfo.biHeight - i - 1) + j * myInfo.biHeight;                   // counting the newBuffer pixel index

            oldIndex = oldIndex * bytesPerPixel + i * padding;                                // counting the buffer byte index
            newIndex = newIndex * bytesPerPixel + (newIndex / myInfo.biHeight) * newPadding;  // counting the newBuffer byte index

            for (int k = 0; k < bytesPerPixel; k++) {
                newBuffer[newIndex + k] = buffer[oldIndex + k];                               // filling in accordingly
            }
        }
    }

    myNewPicture.write(reinterpret_cast<char*> (newBuffer), newStride * myInfo.biWidth);      // writing the newBuffer with rotated counterclockwise pixels
    myNewPicture.close();

    // opening the file for the image rotated clockwise
    ofstream myNewPicture2(outputfile_clockwise, ofstream::binary);
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biWidth), sizeof(myInfo.biWidth));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biPlanes), sizeof(myInfo.biPlanes));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biBitCount), sizeof(myInfo.biBitCount));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biCompression), sizeof(myInfo.biCompression));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biSizeImage), sizeof(myInfo.biSizeImage));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biXPelsPerMeter), sizeof(myInfo.biXPelsPerMeter));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biYPelsPerMeter), sizeof(myInfo.biYPelsPerMeter));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biClrUsed), sizeof(myInfo.biClrUsed));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biClrImportant), sizeof(myInfo.biClrImportant));
    if (myInfo.biSize == 108 || myInfo.biSize == 124)
    {
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biRedMask), sizeof(myInfo.biRedMask));
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biGreenMask), sizeof(myInfo.biGreenMask));
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biBlueMask), sizeof(myInfo.biBlueMask));
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biAlphaMask), sizeof(myInfo.biAlphaMask));
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biCSType), sizeof(myInfo.biCSType));
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biEndpoints), sizeof(myInfo.biEndpoints));
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biGammaRed), sizeof(myInfo.biGammaRed));
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biGammaGreen), sizeof(myInfo.biGammaGreen));
        myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biGammaBlue), sizeof(myInfo.biGammaBlue));
        if (myInfo.biSize == 124) {
            myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biIntent), sizeof(myInfo.biIntent));
            myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biProfileData), sizeof(myInfo.biProfileData));
            myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biProfileSize), sizeof(myInfo.biProfileSize));
            myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biReserved), sizeof(myInfo.biReserved));
        }
    }

    // rotating pixels clockwise
    for (int i = 0; i < myInfo.biHeight; i++) {
        for (int j = 0; j < myInfo.biWidth; j++) {

            int oldIndex = i * myInfo.biWidth + j;                                           // counting the buffer pixel index
            int newIndex = (myInfo.biWidth - j - 1) * myInfo.biHeight + i;                   // counting the newBuffer pixel index

            oldIndex = oldIndex * bytesPerPixel + i * padding;                               // counting the buffer byte index
            newIndex = newIndex * bytesPerPixel + (newIndex / myInfo.biHeight) * newPadding; // counting the newBuffer byte index

            for (int k = 0; k < bytesPerPixel; k++) {
                newBuffer[newIndex + k] = buffer[oldIndex + k];                              // filling in accordingly
            }
            if ((myInfo.biHeight - i - 1) * bytesPerPixel + 3 == newStride - 3) {
                for (int k = 0; k < 3; k++) {
                    newBuffer[newIndex + bytesPerPixel + k] = null;                          // filling in with zeros
                }
            }
        }
    }

    delete[] buffer;    // deleting buffer
    myNewPicture2.write(reinterpret_cast<char*> (newBuffer), newStride * myInfo.biWidth);    // writing the newBuffer with rotated clockwise pixels
    myNewPicture2.close();

    // opening the file for the Gaussian filter
    ofstream myNewPicture3(outputfile_GaussianFilter, ofstream::binary);
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biWidth), sizeof(myInfo.biWidth));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biPlanes), sizeof(myInfo.biPlanes));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biBitCount), sizeof(myInfo.biBitCount));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biCompression), sizeof(myInfo.biCompression));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biSizeImage), sizeof(myInfo.biSizeImage));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biXPelsPerMeter), sizeof(myInfo.biXPelsPerMeter));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biYPelsPerMeter), sizeof(myInfo.biYPelsPerMeter));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biClrUsed), sizeof(myInfo.biClrUsed));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biClrImportant), sizeof(myInfo.biClrImportant));
    if (myInfo.biSize == 108 || myInfo.biSize == 124)
    {
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biRedMask), sizeof(myInfo.biRedMask));
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biGreenMask), sizeof(myInfo.biGreenMask));
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biBlueMask), sizeof(myInfo.biBlueMask));
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biAlphaMask), sizeof(myInfo.biAlphaMask));
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biCSType), sizeof(myInfo.biCSType));
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biEndpoints), sizeof(myInfo.biEndpoints));
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biGammaRed), sizeof(myInfo.biGammaRed));
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biGammaGreen), sizeof(myInfo.biGammaGreen));
        myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biGammaBlue), sizeof(myInfo.biGammaBlue));
        if (myInfo.biSize == 124) {
            myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biIntent), sizeof(myInfo.biIntent));
            myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biProfileData), sizeof(myInfo.biProfileData));
            myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biProfileSize), sizeof(myInfo.biProfileSize));
            myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biReserved), sizeof(myInfo.biReserved));
        }
    }

    // Gaussian filter
    // creating kernel
    const int radius = 5;
    double sigma = 1.92;
    const int kernelSide = radius * 2 + 1;       // side of the kernel
    double kernel[kernelSide * kernelSide];
    double sum = 0;

    // counting the coefficients
    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {

            kernel[(i + radius) * kernelSide + (j + radius)] = exp(-(i * i + j * j) / (2 * sigma * sigma));
            sum += kernel[(i + radius) * kernelSide + (j + radius)];
        }
    }

    // normalizing the coefficients
    for (int i = 0; i < kernelSide * kernelSide; i++) {
        kernel[i] /= sum;
    }

    // creating a new buffer for the Gaussian filter
    unsigned char* newBufferGauss = new unsigned char[myInfo.biWidth * newStride];

    // applying a filter to an image
    for (int i = 0; i < myInfo.biWidth; i++) {                // row number
        for (int j = 0; j < myInfo.biHeight; j++) {           // column number

            double byte1 = 0;                                 // taking into account 3 bytes per pixel
            double byte2 = 0;
            double byte3 = 0;

            for (int x = -radius; x <= radius; x++) {
                for (int y = -radius; y <= radius; y++) {

                    // counting indexes considering the edges
                    int ix = i + x, jy = j + y;
                    if (i + x < 0) {
                        ix = -(i + x);
                    }
                    if (i + x > myInfo.biWidth - 1) {
                        ix = myInfo.biWidth - (i + x + 1 - myInfo.biWidth) - 1;
                    }
                    if (j + y < 0) {
                        jy = -(j + y);
                    }
                    if (j + y > myInfo.biHeight - 1) {
                        jy = myInfo.biHeight - (j + y + 1 - myInfo.biHeight) - 1;
                    }

                    int kernelIndex = (x + radius) * kernelSide + (y + radius);
                    int newBufferIndex = bytesPerPixel * (ix * myInfo.biHeight + jy) + ix * newPadding;

                    if (newBufferIndex >= 0 && newBufferIndex < myInfo.biWidth * newStride) {

                        byte1 += kernel[kernelIndex] * newBuffer[newBufferIndex];               // taking into account 3 bytes per pixel
                        byte2 += kernel[kernelIndex] * newBuffer[newBufferIndex + 1];
                        byte3 += kernel[kernelIndex] * newBuffer[newBufferIndex + 2];
                    }
                }
            }
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding] = (unsigned char)byte1;      // taking into account 3 bytes per pixel
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding + 1] = (unsigned char)byte2;
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding + 2] = (unsigned char)byte3;
        }
    }

    myNewPicture3.write(reinterpret_cast<char*> (newBufferGauss), newStride * myInfo.biWidth);
    myNewPicture3.close();

    delete[] newBuffer;       // deleting buffers
    delete[] newBufferGauss;

    return 0;
}