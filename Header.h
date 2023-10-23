#pragma once
#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED
typedef int FXPT2DOT30; // ������ int � FXPT2DOT30  � ��������
/* Так, ну весь этот ужос я уже где-то видел */
/* Все typedef убрать, сделать нормальные структуры,
 * Стремных названий тоже не надо таких... */
typedef struct {        // �������� ����� ��������� CIEXYZ
    FXPT2DOT30 ciexyzX;
    FXPT2DOT30 ciexyzY;
    FXPT2DOT30 ciexyzZ;
} CIEXYZ;

typedef struct {
    CIEXYZ  ciexyzRed;
    CIEXYZ  ciexyzGreen;
    CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;
/* short и int занимают необязательно 4 байта, тут надо int16_t и int32_t */
typedef struct {
    unsigned short  bfType; //��������� ������� ����� (������ �����), ������ ���� BM = 0x4d42
    unsigned int   bfSize; //������ ����� � ������
    unsigned short    bfReserved1; //2 ����� ��������������� � ������ ���� ����������� �� 0
    unsigned short    bfReserved2; //��. bfReserved1
    unsigned int   bfOffBits; //�������� �� ������ �� ��������� ������
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int    biSize; //������ ������ ��������� � ������, ����������� ����� �� �� ������ ���������
    int             biWidth; //������ ������ � ��������. ����������� ����� ������ �� ������. ���� � ������������� �� ���������������
    int             biHeight; //����� ����� �� ������, ���������� 2 ���������: ������ ������ � �������� (���������� �������� �����) � ������� ���������� ����� � ��������� �������� (���� �����). ������� �������� �� ���������������.
    unsigned short  biPlanes; //2 �����, ����������� ���������� �������� ���������� ��� �������� ���������, ������ ������ 1
    unsigned short  biBitCount; //���������� ��� �� �������, ����� 0 (������� �������� � ������� JPEG ��� PNG), 1, 4, 8, 16, 24, 32. � ��� 24
    unsigned int    biCompression; //��������� �� ������ �������� ��������. BI_RGB | BI_RLE8 | BI_RLE4 |BI_BITFIELDS | BI_JPEG | BI_PNG. ������� ������������ ������ BI_RGB. ��������� ��� ������ �����������, BI_RGB �������� ���������� ������ (��� ���� ����� 0)
    unsigned int    biSizeImage; //��������� ������ ��������� ������; ��� ������������� BI_RGB ��� ����� ���������� ������ 0 (�.�. ���� �������� �������������� ��������� ��������)
    int             biXPelsPerMeter; //��������� �������������� ����������, ������� ��������� - �������/����, ����� ����� �� ������
    int             biYPelsPerMeter; //��������� ������������ ����������, ������� ��������� - �������/����, ����� ����� �� ������
    unsigned int    biClrUsed; //���������� ������������ ������ (���� ���� ������� ������). 0 ����� �������� ������������ ���. ����� ��� 24 RGBTRIPPLE
    unsigned int    biClrImportant; //���������� ������ ������. ���� 0, �� ��� ����� ��������� �������. ������ ����� ���� �������� �������
    unsigned int    biRedMask;
    unsigned int    biGreenMask;
    unsigned int    biBlueMask;
    unsigned int    biAlphaMask;
    unsigned int    biCSType;
    CIEXYZTRIPLE    biEndpoints;
    unsigned int    biGammaRed;
    unsigned int    biGammaGreen;
    unsigned int    biGammaBlue;
    unsigned int    biIntent;
    unsigned int    biProfileData;
    unsigned int    biProfileSize;
    unsigned int    biReserved;
} BITMAPINFOHEADER;

#endif // MAIN_H_INCLUDEDs
#pragma once