// commandlineExcel2picture.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <objidl.h>
#include <gdiplus.h>
#include <atlconv.h>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

using namespace Gdiplus;

#pragma comment(lib, "gdiplus.lib")

BOOL SaveBitmapToFile(HBITMAP hBitmap, LPCTSTR lpFileName)
{
	HDC hDC; //�豸������ 
	int iBits; //��ǰ��ʾ�ֱ�����ÿ��������ռ�ֽ��� 
	WORD wBitCount; //λͼ��ÿ��������ռ�ֽ��� 
	DWORD dwPaletteSize = 0, //�����ɫ���С�� λͼ�������ֽڴ�С ��λͼ�ļ���С �� д���ļ��ֽ��� 
		dwBmBitsSize,
		dwDIBSize, dwWritten;
	BITMAP Bitmap; //λͼ���Խṹ 
	BITMAPFILEHEADER bmfHdr; //λͼ�ļ�ͷ�ṹ 
	BITMAPINFOHEADER bi; //λͼ��Ϣͷ�ṹ 
	LPBITMAPINFOHEADER lpbi; //ָ��λͼ��Ϣͷ�ṹ 

	HANDLE fh, hDib, hPal, hOldPal = NULL; //�����ļ��������ڴ�������ɫ���� 

	//����λͼ�ļ�ÿ��������ռ�ֽ��� 
	HDC hWndDC = CreateDC(TEXT("DISPLAY"), NULL, NULL, NULL);
	hDC = ::CreateCompatibleDC(hWndDC);
	iBits = GetDeviceCaps(hDC, BITSPIXEL) * GetDeviceCaps(hDC, PLANES);
	DeleteDC(hDC);

	if (iBits <= 1)
		wBitCount = 1;
	else if (iBits <= 4)
		wBitCount = 4;
	else if (iBits <= 8)
		wBitCount = 8;
	else if (iBits <= 24)
		wBitCount = 24;
	else
		wBitCount = 24;

	//�����ɫ���С 
	if (wBitCount <= 8)
		dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);

	//����λͼ��Ϣͷ�ṹ 
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biWidth = Bitmap.bmWidth;
	bi.biHeight = Bitmap.bmHeight;
	bi.biPlanes = 1;
	bi.biBitCount = wBitCount;
	bi.biCompression = BI_RGB;
	bi.biSizeImage = 0;
	bi.biXPelsPerMeter = 0;
	bi.biYPelsPerMeter = 0;
	bi.biClrUsed = 0;
	bi.biClrImportant = 0;

	dwBmBitsSize = ((Bitmap.bmWidth * wBitCount + 31) / 32) * 4 * Bitmap.bmHeight;

	//Ϊλͼ���ݷ����ڴ� 
	hDib = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));
	lpbi = (LPBITMAPINFOHEADER)GlobalLock(hDib);
	*lpbi = bi;

	// �����ɫ�� 
	hPal = GetStockObject(DEFAULT_PALETTE);
	if (hPal)
	{
		hDC = ::GetDC(NULL);
		hOldPal = ::SelectPalette(hDC, (HPALETTE)hPal, FALSE);
		RealizePalette(hDC);
	}

	// ��ȡ�õ�ɫ�����µ�����ֵ 
	GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight,
		(LPSTR)lpbi + sizeof(BITMAPINFOHEADER)
		+dwPaletteSize,
		(LPBITMAPINFO)
		lpbi, DIB_RGB_COLORS);

	//�ָ���ɫ�� 
	if (hOldPal)
	{
		SelectPalette(hDC, (HPALETTE)hOldPal, TRUE);
		RealizePalette(hDC);
		::ReleaseDC(NULL, hDC);
	}

	//����λͼ�ļ� 
	fh = CreateFile(lpFileName, GENERIC_WRITE,
		0, NULL, CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if (fh == INVALID_HANDLE_VALUE)
		return FALSE;

	// ����λͼ�ļ�ͷ 
	bmfHdr.bfType = 0x4D42; // "BM" 
	dwDIBSize = sizeof(BITMAPFILEHEADER)
		+sizeof(BITMAPINFOHEADER)
		+dwPaletteSize + dwBmBitsSize;
	bmfHdr.bfSize = dwDIBSize;
	bmfHdr.bfReserved1 = 0;
	bmfHdr.bfReserved2 = 0;
	bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER)
		+(DWORD)sizeof(BITMAPINFOHEADER)
		+dwPaletteSize;

	// д��λͼ�ļ�ͷ 
	WriteFile(fh, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);

	// д��λͼ�ļ��������� 
	WriteFile(fh, (LPSTR)lpbi, dwDIBSize,
		&dwWritten, NULL);

	//��� 
	GlobalUnlock(hDib);
	GlobalFree(hDib);
	CloseHandle(fh);

	return TRUE;

}

bool saveImageFile(Image* pImage, const char * pszFileName, const char* pszImageType)
{
	LPCWSTR lpszFileName, lpszImageType;
	USES_CONVERSION;
	lpszImageType = A2W(pszImageType);
	lpszFileName = A2W(pszFileName);


	UINT num = 0;
	UINT size = 0;

	ImageCodecInfo* pImageCodecInfo = NULL;

	if (::GetImageEncodersSize(&num, &size) != Ok || size == 0)
	{
		return false;
	}

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

	if (pImageCodecInfo == NULL)
	{
		return false;
	}
	if (GetImageEncoders(num,size,pImageCodecInfo)!=Ok)
	{
		return false;
	}

	bool bOk = false;

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, lpszImageType) == 0)
		{
			pImage->Save(lpszFileName, &(pImageCodecInfo[j].Clsid));
			bOk = true;
			break;
		}
	}

	free(pImageCodecInfo);
	return bOk;
}

struct cardInfo
{
	std::string name;
	std::string unit;
	std::string department;
	std::string cardid;
	std::string data;
};
std::vector<std::vector<std::string> > infos;


void readCsvFile(const char* filename)
{
	infos.clear();
	std::vector<char> content;
	std::string line;
	std::string word;
	std::ifstream f(filename, std::ios::in | std::ios::binary);
	if (f.is_open())
	{
		f.seekg(0, std::ios::end);
		content.resize(f.tellg());
		f.seekg(0, std::ios::beg);
		f.read(content.data(), content.size());
		f.seekg(0, std::ios::beg);
	}
	else
	{
		std::cout << "can't open " << filename << std::endl;
		return;
	}
	std::string str = content.data();
	int nLine = 0;
	int nWords = 0;
	for (; str.find("\r\n") != std::string::npos;)
	{
		std::vector<std::string> vecLine;
		line = str.substr(0, str.find("\r\n") + 2);
		str = str.substr(str.find("\r\n") + 2);
		for (; line.find(",") != std::string::npos;)
		{
			word = line.substr(0, line.find(",") );
			line = line.substr(line.find(",") + 1);
			vecLine.push_back(word);
			if (line.find(",") == std::string::npos)
			{
				vecLine.push_back(line);
			}
			nWords++;
		}
		infos.push_back(vecLine);
		nLine++;
	}
}


int _tmain(int argc, _TCHAR* argv[])
{
	readCsvFile("book1.csv");
	std::ostringstream ostm;
	ostm << "read book.Csv  infos.size = "<<infos.size()<<"\n";
	std::cout << ostm.str();

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	USES_CONVERSION;

	for (unsigned i = 0, c = infos.size(); i < c; ++i)
	{
		Font myFont(L"����",12);
		StringFormat myFormat;
		myFormat.SetAlignment(StringAlignmentCenter);
		SolidBrush   myBrush(Color(0xff0000ff));
		Image image(L"card.jpg");
		Graphics g(&image);
		Pen myPen(Color(0xff00ff00));
		g.DrawRectangle(&myPen, Rect(0, 0, image.GetWidth(), image.GetHeight()));
		for (unsigned j = 0, jc = infos[i].size(); j < jc;++j)
		{
			g.DrawString(A2W(infos[i][j].c_str()), -1, &myFont, RectF(80, 56 + 29 * j, 122, 16), &myFormat, &myBrush);
		}
		std::ostringstream stm;
		stm <<i<<"_card" << infos[i][0] << ".jpg";
		saveImageFile(&image, stm.str().c_str(), "image/jpeg");
	}


	GdiplusShutdown(gdiplusToken);
	system("pause");
	return 0;
}



