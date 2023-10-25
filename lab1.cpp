#include <iostream>
#include <fstream>
#include "Header.h"

int main() {
    setlocale(LC_CTYPE, "rus");

    const char* inputfile = "sample.bmp";
    const char* outputfile_counterclockwise = "RotatedCounterclockwise.bmp";
    const char* outputfile_clockwise = "RotatedClockwise.bmp";
    const char* outputfile_GaussianFilter = "GaussianFilter.bmp";

    std::ifstream Picture(inputfile, std::ifstream::binary);
    if (!Picture.is_open()) {
        std::cerr << "File opening error. Try again or enter a different name." << std::endl;
        Picture.close();
        return 0;
    }

    //reading header
    BITMAPFILEHEADER Header{};

    Picture.read(reinterpret_cast<char*>(&Header), sizeof(BITMAPFILEHEADER));

    //checking the conditions associated with the header
    if (Header.bfType != 0x4d42) {
        std::cerr << "This file is not a BMP file." << std::endl;
        Picture.close();
        return 0;
    }

    //reading the information
    BITMAPINFOHEADER Info{};
    PALETTE1 Palette1{};
    PALETTE2 Palette2{};

    Picture.read(reinterpret_cast<char*>(&Info), sizeof(BITMAPINFOHEADER));

    if (Info.biSize == 108 || Info.biSize == 124)
    {
        Picture.read(reinterpret_cast<char*>(&Palette1), sizeof(PALETTE1));
        if (Info.biSize == 124)
        {
            Picture.read(reinterpret_cast<char*>(&Palette2), sizeof(PALETTE2));
        }
    }

    //checking the conditions
    if ((Info.biSize != 40 && Info.biSize != 108 && Info.biSize != 124) ||
        Header.bfReserved1 != 0 ||
        Header.bfReserved2 != 0 ||
        Info.biPlanes != 1 ||
        Info.biCompression != 0 ||  //only uncompressed images
        Info.biBitCount != 24)
    {
        std::cerr << "Unsupported BMP format." << std::endl;
        Picture.close();
        return 0;
    }


    //calculating the width, taking into account the alignment
    int Width = Info.biWidth;
    int Height = Info.biHeight;
    int bytesPerPixel = Info.biBitCount / 8;
    int alignment = 4;                                         // alignment
    int Stride = ((Width * bytesPerPixel + (alignment - 1)) / alignment) * alignment;          // width, multiple of 4
    int Padding = Stride - Width * bytesPerPixel;

    //creating a buffer
    int bufferSize = Stride * Height;
    unsigned char* buffer = new unsigned char[bufferSize];

    std::cout << "The amount of memory allocated for image loading is " << bufferSize << " bytes." << std::endl;

    //reading to the buffer
    Picture.read(reinterpret_cast<char*> (buffer), bufferSize);
    Picture.close();

    // calculating a new width, taking into account the alignment
    int newStride = ((Height * bytesPerPixel + (alignment - 1)) / alignment) * alignment;               // new width multiple of 4
    int newPadding = newStride - Height * bytesPerPixel;

    //creating a new buffer
    int newBufferSize = Width * newStride;
    unsigned char* newBuffer = new unsigned char[newBufferSize];

    Header.bfSize = 14 + Info.biSize + Width * newStride;                // counting the size of new file with the rotated image
    Info.biHeight = Width;                                               // changing height and width
    Info.biWidth = Height;


    // opening the file for the image rotated counterclockwise
    std::ofstream NewPicture(outputfile_counterclockwise, std::ofstream::binary);

    NewPicture.write(reinterpret_cast<char*>(&Header), sizeof(BITMAPFILEHEADER));
    NewPicture.write(reinterpret_cast<char*>(&Info), sizeof(BITMAPINFOHEADER));

    if (Info.biSize == 108 || Info.biSize == 124) {
        NewPicture.write(reinterpret_cast<char*>(&Palette1), sizeof(PALETTE1));

        if (Info.biSize == 124) {
            NewPicture.write(reinterpret_cast<char*>(&Palette2), sizeof(PALETTE2));

        }
    }

    // rotating pixels counterclockwise
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {

            int oldIndex = i * Width + j;                                            // counting the buffer pixel index
            int newIndex = (Height - i - 1) + j * Height;                            // counting the newBuffer pixel index

            oldIndex = oldIndex * bytesPerPixel + i * Padding;                       // counting the buffer byte index
            newIndex = newIndex * bytesPerPixel + (newIndex / Height) * newPadding;  // counting the newBuffer byte index

            for (int k = 0; k < bytesPerPixel; k++) {
                newBuffer[newIndex + k] = buffer[oldIndex + k];                      // filling in accordingly
            }
        }
    }

    NewPicture.write(reinterpret_cast<char*> (newBuffer), newBufferSize);     // writing the newBuffer with rotated counterclockwise pixels
    NewPicture.close();


    // opening the file for the image rotated clockwise
    std::ofstream NewPicture2(outputfile_clockwise, std::ofstream::binary);

    NewPicture2.write(reinterpret_cast<char*>(&Header), sizeof(BITMAPFILEHEADER));
    NewPicture2.write(reinterpret_cast<char*>(&Info), sizeof(BITMAPINFOHEADER));

    if (Info.biSize == 108 || Info.biSize == 124) {
        NewPicture2.write(reinterpret_cast<char*>(&Palette1), sizeof(PALETTE1));

        if (Info.biSize == 124) {
            NewPicture2.write(reinterpret_cast<char*>(&Palette2), sizeof(PALETTE2));

        }
    }

    // rotating pixels clockwise
    for (int i = 0; i < Height; i++) {
        for (int j = 0; j < Width; j++) {

            int oldIndex = i * Width + j;                                            // counting the buffer pixel index
            int newIndex = (Width - j - 1) * Height + i;                             // counting the newBuffer pixel index

            oldIndex = oldIndex * bytesPerPixel + i * Padding;                       // counting the buffer byte index
            newIndex = newIndex * bytesPerPixel + (newIndex / Height) * newPadding;  // counting the newBuffer byte index

            for (int k = 0; k < bytesPerPixel; k++) {
                newBuffer[newIndex + k] = buffer[oldIndex + k];                      // filling in accordingly
            }
        }
    }

    delete[] buffer;    // deleting buffer
    NewPicture2.write(reinterpret_cast<char*> (newBuffer), newBufferSize);      // writing the newBuffer with rotated clockwise pixels
    NewPicture2.close();


    // Gaussian filter
    // opening the file for the Gaussian filter
    std::ofstream NewPicture3(outputfile_GaussianFilter, std::ofstream::binary);

    NewPicture3.write(reinterpret_cast<char*>(&Header), sizeof(BITMAPFILEHEADER));
    NewPicture3.write(reinterpret_cast<char*>(&Info), sizeof(BITMAPINFOHEADER));

    if (Info.biSize == 108 || Info.biSize == 124) {
        NewPicture3.write(reinterpret_cast<char*>(&Palette1), sizeof(PALETTE1));

        if (Info.biSize == 124) {
            NewPicture3.write(reinterpret_cast<char*>(&Palette2), sizeof(PALETTE2));

        }
    }

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
    unsigned char* newBufferGauss = new unsigned char[newBufferSize];

    // applying a filter to an image
    for (int i = 0; i < Width; i++) {                // row number
        for (int j = 0; j < Height; j++) {           // column number

            double byte1 = 0;                        // taking into account 3 bytes per pixel
            double byte2 = 0;
            double byte3 = 0;

            for (int x = -radius; x <= radius; x++) {
                for (int y = -radius; y <= radius; y++) {

                    // counting indexes considering the edges
                    int ix = i + x, jy = j + y;
                    if (i + x < 0) {
                        ix = -(i + x);
                    }
                    if (i + x > Width - 1) {
                        ix = Width - (i + x + 1 - Width) - 1;
                    }
                    if (j + y < 0) {
                        jy = -(j + y);
                    }
                    if (j + y > Height - 1) {
                        jy = Height - (j + y + 1 - Height) - 1;
                    }

                    int kernelIndex = (x + radius) * kernelSide + (y + radius);
                    int newBufferIndex = bytesPerPixel * (ix * Height + jy) + ix * newPadding;

                    if (newBufferIndex >= 0 && newBufferIndex < Width * newStride) {

                        byte1 += kernel[kernelIndex] * newBuffer[newBufferIndex];                // taking into account 3 bytes per pixel
                        byte2 += kernel[kernelIndex] * newBuffer[newBufferIndex + 1];
                        byte3 += kernel[kernelIndex] * newBuffer[newBufferIndex + 2];
                    }
                }
            }
            newBufferGauss[bytesPerPixel * (i * Height + j) + i * newPadding] = (unsigned char)byte1;      // taking into account 3 bytes per pixel
            newBufferGauss[bytesPerPixel * (i * Height + j) + i * newPadding + 1] = (unsigned char)byte2;
            newBufferGauss[bytesPerPixel * (i * Height + j) + i * newPadding + 2] = (unsigned char)byte3;
        }
    }

    NewPicture3.write(reinterpret_cast<char*> (newBufferGauss), newBufferSize);
    NewPicture3.close();

    delete[] newBuffer;       // deleting buffers
    delete[] newBufferGauss;

    return 0;
}
