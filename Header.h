#pragma once
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
#pragma pack(push, 1)

typedef struct {
    int     ciexyzX;
    int     ciexyzY;
    int     ciexyzZ;
} CIEXYZ;

typedef struct {
    CIEXYZ  ciexyzRed;
    CIEXYZ  ciexyzGreen;
    CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

typedef struct {
    short           bfType;           //сигнатура формата файла (формат файла), должно быть 0x4d42
    int             bfSize;           //размер файла в байтах
    short           bfReserved1;      //2 байта зарезервированы и должны быть установлены на 0
    short           bfReserved2;      //см. bfReserved1
    int             bfOffBits;        //смещение от начала до растровых данных в байтах
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int    biSize;            //размер структуры в байтах
    int             biWidth;           //ширина растра в пикселях. указывается целым числом со знаком. ноль и отрицательные не документированы
    int             biHeight;          //целое число со знаком, содержащее 2 параметра: высота растра в пикселях (абсолютное значение числа) и порядок следования строк в двумерных массивах (знак числа); нулевое значение не документировано
    unsigned short  biPlanes;          //2 байта, указывающие количество цветовых плоскостей для целевого устройста, всегда равное 1
    unsigned short  biBitCount;        //количество бит на пиксель
    unsigned int    biCompression;     //указывает на способ хранения пикселей; обычно используется BI_RGB; указывает тип сжатия изображения, BI_RGB означает отсутствие сжатия (это поле равно 0)
    unsigned int    biSizeImage;       //указывает размер растровых данных; при использовании BI_RGB его можно установить равным 0
    int             biXPelsPerMeter;   //указывает горизонтальное разрешение, целое число со знаком
    int             biYPelsPerMeter;   //указывает вертикальное разрешение, целое число со знаком
    unsigned int    biClrUsed;         //количество используемых цветов (0 - используется максимальное число)
    unsigned int    biClrImportant;    //количество важных цветов; eсли 0, то все цвета считаются важными
} BITMAPINFOHEADER;

typedef struct {
    unsigned int    biRedMask;
    unsigned int    biGreenMask;
    unsigned int    biBlueMask;
    unsigned int    biAlphaMask;
    unsigned int    biCSType;
    CIEXYZTRIPLE    biEndpoints;
    unsigned int    biGammaRed;
    unsigned int    biGammaGreen;
    unsigned int    biGammaBlue;
} PALETTE1;

typedef struct {
    unsigned int    biIntent;
    unsigned int    biProfileData;
    unsigned int    biProfileSize;
    unsigned int    biReserved;
} PALETTE2;

#pragma pack(pop)
#endif // MAIN_H_INCLUDED
#pragma once