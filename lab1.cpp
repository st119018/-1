#include <iostream>
#include <fstream>
#include "Header.h"
using namespace std;

int main() {
    setlocale(LC_CTYPE, "rus");

    ifstream myPicture("Dsample.bmp", ifstream::binary);
    if (!myPicture) {
        cout << "File opening error. Try again or enter a different name.";
        myPicture.close();
        return 0;
    }

    //read header
    BITMAPFILEHEADER myHeader;
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myPicture.read(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    //checking the conditions associated with the header
    if (myHeader.bfType != 0x4d42) {
        cout << "This file is not a BMP file." << endl;
        myPicture.close();
        return 0;
    }

    //reading the information
    BITMAPINFOHEADER myInfo;
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
    {
        cout << "Unsupported BMP format." << endl;
        myPicture.close();
        return 0;
    }

    //calculating the width of the string, taking into account the alignment
    int bytesPerPixel = myInfo.biBitCount / 8;
    int alignment = 4;                         // alignment
    int stride = (myInfo.biWidth * bytesPerPixel) + (alignment - 1);
    stride /= alignment;
    stride *= alignment;  // width, multiple of 4
    int padding = stride - myInfo.biWidth * bytesPerPixel;

    //creating a buffer
    unsigned char* buffer = new unsigned char[stride * myInfo.biHeight];
    //read to the buffe
    myPicture.read(reinterpret_cast<char*> (buffer), stride * myInfo.biHeight);
    myPicture.close();


    // Создаем новый буфер для повернутого изображения
    int newPadding = ((myInfo.biHeight * bytesPerPixel + 3) / 4) * 4 - myInfo.biHeight * bytesPerPixel;
    int newStride = myInfo.biHeight * bytesPerPixel + newPadding;        // новая ширина кратная 4
    unsigned char* newBuffer = new unsigned char[myInfo.biWidth * newStride];
    char null = buffer[0];                               // запоминаем чтобы заполнить нулевые байты в будущем))
    myHeader.bfSize = 14 + myInfo.biSize + myInfo.biWidth * newStride;// считаем новый размер файла с повернутым изображением

    BITMAPFILEHEADER myNewHeader;
    ofstream myNewPicture("bmpRotate(counterclockwise).bmp", ofstream::binary);
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myNewPicture.write(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myNewPicture.write(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));    // поменяли местами высоту и ширину
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

    for (int i = 0; i < myInfo.biHeight; i++) {        //в пикселях 
        for (int j = 0; j < myInfo.biWidth; j++) {     // в пикселях

            int oldIndex = i * myInfo.biWidth + j;                    // индекс без учета трех байт в пикселе и без padding
            int newIndex = (myInfo.biHeight - i - 1) + j * myInfo.biHeight;

            oldIndex = oldIndex * bytesPerPixel + i * padding;      // теперь старый индекс соответствует индексу старого массива
            newIndex = newIndex * bytesPerPixel + (newIndex / myInfo.biHeight) * newPadding; // теперь новый индекс соответствует инд нового массива

            for (int k = 0; k < bytesPerPixel; k++) {
                newBuffer[newIndex + k] = buffer[oldIndex + k];      //заполняем соответственно
            }
        }
    }

    myNewPicture.write(reinterpret_cast<char*> (newBuffer), newStride * myInfo.biWidth);
    myNewPicture.close();

    // открываем файл для поворота по часовой стрелке
    ofstream myNewPicture2("bmpRotate(clockwise).bmp", ofstream::binary);
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myNewPicture2.write(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myNewPicture2.write(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));    // поменяли местами высоту и ширину
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

    for (int i = 0; i < myInfo.biHeight; i++) {
        for (int j = 0; j < myInfo.biWidth; j++) {

            int oldIndex = i * myInfo.biWidth + j;                    // индекс без учета трех байт в пикселе и без padding
            int newIndex = (myInfo.biWidth - j - 1) * myInfo.biHeight + i;

            oldIndex = oldIndex * bytesPerPixel + i * padding;      // теперь старый индекс соответствует индексу старого массива
            newIndex = newIndex * bytesPerPixel + (newIndex / myInfo.biHeight) * newPadding; // теперь новый индекс соответствует инд нового

            for (int k = 0; k < bytesPerPixel; k++) {
                newBuffer[newIndex + k] = buffer[oldIndex + k];      //заполняем соответственно
            }
            if ((myInfo.biHeight - i - 1) * bytesPerPixel + 3 == newStride - 3) {    // а эти индексы заполняем нулями
                for (int k = 0; k < 3; k++) {
                    newBuffer[newIndex + bytesPerPixel + k] = null;
                }
            }
        }
    }

    delete[] buffer;
    myNewPicture2.write(reinterpret_cast<char*> (newBuffer), newStride * myInfo.biWidth);
    myNewPicture2.close();

    ofstream myNewPicture3("bmpRotateV2(GaussianFilter).bmp", ofstream::binary);
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfType), sizeof(myHeader.bfType));
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfSize), sizeof(myHeader.bfSize));
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfReserved1), sizeof(myHeader.bfReserved1));
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfReserved2), sizeof(myHeader.bfReserved2));
    myNewPicture3.write(reinterpret_cast<char*>(&myHeader.bfOffBits), sizeof(myHeader.bfOffBits));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biSize), sizeof(myInfo.biSize));
    myNewPicture3.write(reinterpret_cast<char*>(&myInfo.biHeight), sizeof(myInfo.biHeight));    // поменяли местами высоту и ширину
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


    //фильтр Гаусса
    // создаем ядро
    const int radius = 5;                  
    double sigma = 1.92;
    const int kernelSide = radius * 2 + 1; // сторона ядра
    double kernel[kernelSide * kernelSide];
    double sum = 0;

    for (int i = -radius; i <= radius; i++) {
        for (int j = -radius; j <= radius; j++) {

            kernel[(i + radius) * kernelSide + (j + radius)] = exp(-(i * i + j * j) / (2 * sigma * sigma));
            sum += kernel[(i + radius) * kernelSide + (j + radius)];
        }
    }

    // нормализуем коэффициенты
    for (int i = 0; i < kernelSide * kernelSide; i++) {
        kernel[i] /= sum;
    }

    // создаем новый буфер для фильтра гаусса
    unsigned char* newBufferGauss = new unsigned char[myInfo.biWidth * newStride];



    // создали ядро, теперь 
    for (int i = 0; i < myInfo.biWidth; i++) {                // номер строки (т.к. изображение повернуто)
        for (int j = 0; j < myInfo.biHeight; j++) {           // номер столбца изображения
            double byte1 = 0;
            double byte2 = 0;
            double byte3 = 0;

            for (int x = -radius; x <= radius; x++) {
                for (int y = -radius; y <= radius; y++) {

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
                    int newBufferIndex = bytesPerPixel * ((ix)*myInfo.biHeight + jy) + (ix)*newPadding;

                    if (newBufferIndex >= 0 && newBufferIndex < myInfo.biWidth * newStride) {
                        byte1 += kernel[kernelIndex] * newBuffer[newBufferIndex];
                        byte2 += kernel[kernelIndex] * newBuffer[newBufferIndex + 1];
                        byte3 += kernel[kernelIndex] * newBuffer[newBufferIndex + 2];
                    }
                }
            }
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding] = (unsigned char)byte1;
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding + 1] = (unsigned char)byte2;
            newBufferGauss[bytesPerPixel * (i * myInfo.biHeight + j) + i * newPadding + 2] = (unsigned char)byte3;
        }
    }

    myNewPicture3.write(reinterpret_cast<char*> (newBufferGauss), newStride * myInfo.biWidth);
    myNewPicture3.close();

    delete[] newBuffer;
    delete[] newBufferGauss;

    return 0;
}