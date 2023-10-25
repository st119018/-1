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
    short           bfType;           //��������� ������� ����� (������ �����), ������ ���� 0x4d42
    int             bfSize;           //������ ����� � ������
    short           bfReserved1;      //2 ����� ��������������� � ������ ���� ����������� �� 0
    short           bfReserved2;      //��. bfReserved1
    int             bfOffBits;        //�������� �� ������ �� ��������� ������ � ������
} BITMAPFILEHEADER;

typedef struct
{
    unsigned int    biSize;            //������ ��������� � ������
    int             biWidth;           //������ ������ � ��������. ����������� ����� ������ �� ������. ���� � ������������� �� ���������������
    int             biHeight;          //����� ����� �� ������, ���������� 2 ���������: ������ ������ � �������� (���������� �������� �����) � ������� ���������� ����� � ��������� �������� (���� �����); ������� �������� �� ���������������
    unsigned short  biPlanes;          //2 �����, ����������� ���������� �������� ���������� ��� �������� ���������, ������ ������ 1
    unsigned short  biBitCount;        //���������� ��� �� �������
    unsigned int    biCompression;     //��������� �� ������ �������� ��������; ������ ������������ BI_RGB; ��������� ��� ������ �����������, BI_RGB �������� ���������� ������ (��� ���� ����� 0)
    unsigned int    biSizeImage;       //��������� ������ ��������� ������; ��� ������������� BI_RGB ��� ����� ���������� ������ 0
    int             biXPelsPerMeter;   //��������� �������������� ����������, ����� ����� �� ������
    int             biYPelsPerMeter;   //��������� ������������ ����������, ����� ����� �� ������
    unsigned int    biClrUsed;         //���������� ������������ ������ (0 - ������������ ������������ �����)
    unsigned int    biClrImportant;    //���������� ������ ������; e��� 0, �� ��� ����� ��������� �������
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