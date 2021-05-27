#include "stdafx.h"
#include <string>
#include "math.h"

/*
extern PBITMAPINFO GetBitmapInfo(HBITMAP hBitmap);
extern HBITMAP CopyScreenToBitmap(LPRECT lpRect);
extern BOOL		SaveBitmap(HBITMAP hBitmap,int xDPI,int yDPI,CString szFileName);
extern BOOL		SaveBitmapFromHandle(HGLOBAL hBits,CString szFileName);
extern DWORD	GetBitmapHandleSize(HBITMAP hBitmap);
extern BOOL		GetBitmapHandle(HBITMAP hBitmap,int xDPI,int yDPI,HGLOBAL hBits);
extern DWORD	GetHandleSize(HGLOBAL hBits);
extern HBITMAP	MakeBitmapFromHandle(HGLOBAL hBits);
extern HBITMAP  MakeBitmapFromHandle(HGLOBAL hBits,int &xDPI,int &yDPI);
extern void		GetXDPIFromHandle(HGLOBAL hBits,int &xDPI,int &yDPI);
*/
#define  WIDTHBYTES(i)    ((i+31)/32*4)

HBITMAP CopyScreenToBitmap(LPRECT lpRect){  
    HDC hScrDC, hMemDC;             // 屏幕和内存设备描述表  
    HBITMAP hBitmap, hOldBitmap;    // 位图句柄  

	CRect rect;
    int nWidth, nHeight;            // 位图宽度和高度  
    int xScrn, yScrn;               // 屏幕分辨率  
  

    if(IsRectEmpty(lpRect))  
        return NULL;  
  
    hScrDC = CreateDC("DISPLAY", NULL, NULL, NULL);
    hMemDC = CreateCompatibleDC(hScrDC);
  
    rect.left = lpRect->left;  
    rect.top = lpRect->top;  
    rect.right = lpRect->right;  
    rect.bottom = lpRect->bottom;  
      
    xScrn = GetSystemMetrics(SM_CXSCREEN);; // 获得屏幕水平分辨率  
    yScrn = GetDeviceCaps(hScrDC, VERTRES);  
  
    if (rect.left < 0)  
        rect.left = 0;  
    if (rect.top < 0)  
        rect.top = 0;  
    if (rect.right > xScrn)  
        rect.right = xScrn;  
    if (rect.bottom > yScrn)  
        rect.bottom = yScrn;  
	rect.NormalizeRect();
    nWidth = rect.Width();  
    nHeight = rect.Height();  
  
    hBitmap = CreateCompatibleBitmap(hScrDC, nWidth, nHeight);      // 创建一个与屏幕设备描述表兼容的位图  
    hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);            // 把新位图选到内存设备描述表中  
    BitBlt(hMemDC, 0, 0, nWidth, nHeight, hScrDC, rect.left, rect.top, SRCCOPY); // 把屏幕设备描述表拷贝到内存设备描述表中  
    hBitmap = (HBITMAP)SelectObject(hMemDC, hOldBitmap);            // 得到屏幕位图的句柄  
  
    DeleteDC(hScrDC);  
    DeleteDC(hMemDC);  
  
    return hBitmap;  
}


BOOL SaveBitmap(HBITMAP hBitmap,int xDPI,int yDPI,CString szFileName)
{
	HDC hDC;                        // 设备描述表
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwBFSize, dwWritten;    // 调色板大小，位图数据大小，位图文件大小，写入文件字节数  
	BITMAP Bitmap;                  //位图属性结构  
	BITMAPFILEHEADER bmfHdr;        // 位图文件头  
	BITMAPINFOHEADER BmiHeader;            // 位图信息头  
	LPBITMAPINFOHEADER pBmiHeader;        // 指向位图信息头结构  
	
	HANDLE hFile, hBits;                // 定义文件，分配内存句柄
	HPALETTE hPal, hOldPal=NULL;    // 调色板句柄  
  	/*
    // 计算位图文件每个像素所占字节数  
    hDC = CreateDC("DISPLAY", NULL, NULL, NULL);  
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
        wBitCount = 32; 
    if (wBitCount <= 8)  
        dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);       // 计算调色板大小  
  */
	BITMAP bmp;
	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp))
		return NULL;
	WORD    cClrBits;
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
    else if (cClrBits <= 8)
		cClrBits = 8;
    else if (cClrBits <= 16)
		cClrBits = 16;
    else if (cClrBits <= 24)
		cClrBits = 24;
    else cClrBits = 32;


	if(cClrBits < 24)
		dwPaletteSize = (1 << cClrBits) * sizeof(RGBQUAD);


    // 设置位图信息头结构  
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    BmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BmiHeader.biWidth = Bitmap.bmWidth;
    BmiHeader.biHeight = Bitmap.bmHeight;
    BmiHeader.biPlanes = 1;
    BmiHeader.biBitCount = cClrBits;
    BmiHeader.biCompression = BI_RGB;
    BmiHeader.biSizeImage = 0;
	if(xDPI>0){
		long bPP = long(xDPI*10000/254 +0.5);
		BmiHeader.biXPelsPerMeter = bPP;  
	}else{
		BmiHeader.biXPelsPerMeter = 0;  
	}

	if(yDPI>0){
		long bPP = long(yDPI*10000/254 +0.5);
		BmiHeader.biYPelsPerMeter = bPP;
	}else{
		BmiHeader.biYPelsPerMeter = 0;
	}

    BmiHeader.biClrUsed = 0;
    BmiHeader.biClrImportant = 0;

	if(cClrBits < 24)
		BmiHeader.biClrUsed = (1<<cClrBits);
	//dwBmBitsSize = ((bi.biWidth * cClrBits +31) & ~31) /8 * bi.biHeight;
    dwBmBitsSize = ((Bitmap.bmWidth * cClrBits + 31) / 32) * 4 * Bitmap.bmHeight;  
  
    hBits = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  // 为位图内容分配内存  
	if(hBits==NULL)
		return FALSE;
    pBmiHeader = (LPBITMAPINFOHEADER)GlobalLock(hBits);  
    *pBmiHeader = BmiHeader;  
    // 处理调色板  
    hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
    if(hPal){ 
        hDC = GetDC(NULL);  
        hOldPal = SelectPalette(hDC, hPal, FALSE);  
        RealizePalette(hDC); 
      
		// 获取该调色板下新的像素值  
		GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)pBmiHeader + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)pBmiHeader, DIB_RGB_COLORS);
		if(hOldPal)// 恢复调色板
		{ 
			SelectPalette(hDC, hOldPal, TRUE);
			RealizePalette(hDC);
		}
		ReleaseDC(NULL, hDC);
	}else{
		return FALSE;
	}
    // 创建位图文件   
    hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);  
    if(hFile == INVALID_HANDLE_VALUE)  
        return FALSE;  
  
    // 设置位图文件头  
    bmfHdr.bfType = 0x4D42;     // 文件类型: "BM"  
    dwBFSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;   
    bmfHdr.bfSize = dwBFSize;  // 位图文件大小  
    bmfHdr.bfReserved1 = 0;  
    bmfHdr.bfReserved2 = 0;  
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;  
      
    WriteFile(hFile, (LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER), &dwWritten, NULL);  // 写入位图文件头  
    WriteFile(hFile, (LPSTR)pBmiHeader, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize, &dwWritten, NULL);                    // 写入位图文件其余内容  
  
    GlobalUnlock(hBits);  
    GlobalFree(hBits);  
    CloseHandle(hFile);  
  
    return TRUE;  
}

std::string GetBitmapFileData(HBITMAP hBitmap)
{
	std::string sData = "";

	HDC hDC;                        // 设备描述表
	DWORD dwPaletteSize = 0, dwBmBitsSize, dwBFSize;    // 调色板大小，位图数据大小，位图文件大小，写入文件字节数  
	BITMAP Bitmap;                  //位图属性结构  
	BITMAPFILEHEADER bmfHdr;        // 位图文件头  
	BITMAPINFOHEADER BmiHeader;            // 位图信息头  
	LPBITMAPINFOHEADER pBmiHeader;        // 指向位图信息头结构  
	
	HANDLE hBits;                // 定义文件，分配内存句柄
	HPALETTE hPal, hOldPal=NULL;    // 调色板句柄  
  	/*
    // 计算位图文件每个像素所占字节数  
    hDC = CreateDC("DISPLAY", NULL, NULL, NULL);  
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
        wBitCount = 32; 
    if (wBitCount <= 8)  
        dwPaletteSize = (1 << wBitCount) * sizeof(RGBQUAD);       // 计算调色板大小  
  */
	BITMAP bmp;
	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp))
		return NULL;
	WORD    cClrBits;
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
    else if (cClrBits <= 8)
		cClrBits = 8;
    else if (cClrBits <= 16)
		cClrBits = 16;
    else if (cClrBits <= 24)
		cClrBits = 24;
    else cClrBits = 32;


	if(cClrBits < 24)
		dwPaletteSize = (1 << cClrBits) * sizeof(RGBQUAD);


    // 设置位图信息头结构  
    GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap);
    BmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BmiHeader.biWidth = Bitmap.bmWidth;
    BmiHeader.biHeight = Bitmap.bmHeight;
    BmiHeader.biPlanes = 1;
    BmiHeader.biBitCount = cClrBits;
    BmiHeader.biCompression = BI_RGB;
    BmiHeader.biSizeImage = 0;
	
	BmiHeader.biXPelsPerMeter = 0;  
	BmiHeader.biYPelsPerMeter = 0;

    BmiHeader.biClrUsed = 0;
    BmiHeader.biClrImportant = 0;

	if(cClrBits < 24)
		BmiHeader.biClrUsed = (1<<cClrBits);
	//dwBmBitsSize = ((bi.biWidth * cClrBits +31) & ~31) /8 * bi.biHeight;
    dwBmBitsSize = ((Bitmap.bmWidth * cClrBits + 31) / 32) * 4 * Bitmap.bmHeight;  
  
    hBits = GlobalAlloc(GHND, dwBmBitsSize + dwPaletteSize + sizeof(BITMAPINFOHEADER));  // 为位图内容分配内存  
	if(hBits==NULL)
		return sData;
    pBmiHeader = (LPBITMAPINFOHEADER)GlobalLock(hBits);  
    *pBmiHeader = BmiHeader;  
    // 处理调色板  
    hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
    if(hPal){ 
        hDC = GetDC(NULL);  
        hOldPal = SelectPalette(hDC, hPal, FALSE);  
        RealizePalette(hDC); 
      
		// 获取该调色板下新的像素值  
		GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)pBmiHeader + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)pBmiHeader, DIB_RGB_COLORS);
		if(hOldPal)// 恢复调色板
		{ 
			SelectPalette(hDC, hOldPal, TRUE);
			RealizePalette(hDC);
		}
		ReleaseDC(NULL, hDC);
	}else{
		return FALSE;
	}
  
    // 设置位图文件头  
    bmfHdr.bfType = 0x4D42;     // 文件类型: "BM"  
    dwBFSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize;   
    bmfHdr.bfSize = dwBFSize;  // 位图文件大小  
    bmfHdr.bfReserved1 = 0;  
    bmfHdr.bfReserved2 = 0;  
    bmfHdr.bfOffBits = (DWORD)sizeof(BITMAPFILEHEADER) + (DWORD)sizeof(BITMAPINFOHEADER) + dwPaletteSize;  
    
//	char* pBuffer = (char*)malloc(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize);
//	memcpy(pBuffer,(LPSTR)&bmfHdr, sizeof(BITMAPFILEHEADER));
//	memcpy((pBuffer+sizeof(BITMAPFILEHEADER)),(LPSTR)pBmiHeader, (sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize));
	sData.insert(0,(LPSTR)&bmfHdr,sizeof(BITMAPFILEHEADER));
	sData.insert(sizeof(BITMAPFILEHEADER),(LPSTR)pBmiHeader,sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize);
	//Base64 base64;
	//sData = base64.base64_encode((const unsigned char*)pBuffer,(sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize));
  
    GlobalUnlock(hBits);  
    GlobalFree(hBits);  
  
    return sData;  
}

PBITMAPINFO GetBitmapInfo(HBITMAP hBitmap){
	BITMAP bmp;
	PBITMAPINFO pbmi;
	WORD    cClrBits;
	
	// Retrieve the bitmap color format, width, and height.
	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bmp))
		return NULL;
	// Convert the color format to a count of bits.
	
	cClrBits = (WORD)(bmp.bmPlanes * bmp.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
    else if (cClrBits <= 8)
		cClrBits = 8;
    else if (cClrBits <= 16)
		cClrBits = 16;
    else if (cClrBits <= 24)
		cClrBits = 24;
    else cClrBits = 32;

	// Allocate memory for the BITMAPINFO structure. (This structure
	// contains a BITMAPINFOHEADER structure and an array of RGBQUAD
	// data structures.)    
	if(cClrBits < 24)
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR,sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * (1<< cClrBits));
		// There is no RGBQUAD array for the 24-bit-per-pixel format.
	else
		pbmi = (PBITMAPINFO) LocalAlloc(LPTR,sizeof(BITMAPINFOHEADER));
    // Initialize the fields in the BITMAPINFO structure.
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	pbmi->bmiHeader.biWidth = bmp.bmWidth;
	pbmi->bmiHeader.biHeight = bmp.bmHeight;
	pbmi->bmiHeader.biPlanes = bmp.bmPlanes;
	pbmi->bmiHeader.biBitCount = bmp.bmBitsPixel;
	if(cClrBits < 24)
		pbmi->bmiHeader.biClrUsed = (1<<cClrBits);
	// If the bitmap is not compressed, set the BI_RGB flag.
	pbmi->bmiHeader.biCompression = BI_RGB;

	// Compute the number of bytes in the array of color
	// indices and store the result in biSizeImage.
	// For Windows NT, the width must be DWORD aligned unless
	// the bitmap is RLE compressed. This example shows this.
    // For Windows 95/98/Me, the width must be WORD aligned unless the
    // bitmap is RLE compressed.
	pbmi->bmiHeader.biSizeImage = ((pbmi->bmiHeader.biWidth * cClrBits +31) & ~31) /8 * pbmi->bmiHeader.biHeight;
	// Set biClrImportant to 0, indicating that all of the
	// device colors are important.
	pbmi->bmiHeader.biClrImportant = 0;
	return pbmi;    
}

DWORD GetHandleSize(HGLOBAL hBits){
	BITMAPINFOHEADER* pBmiHeader = NULL;
	// 解析成位图文件头
	//DWORD dwBitsSize = GlobalSize(hBits);
	pBmiHeader = (BITMAPINFOHEADER*)GlobalLock(hBits);
	/*dwPaletteSize = pBmiHeader->biClrUsed * sizeof(RGBQUAD)-ERROR*/
	DWORD dwPaletteSize = 0;
	if(pBmiHeader->biBitCount<24){
		DWORD cClrBits = pBmiHeader->biClrUsed ? pBmiHeader->biClrUsed : 1 << pBmiHeader->biBitCount;
		dwPaletteSize = cClrBits * sizeof(RGBQUAD);
	}
	DWORD dwSize = sizeof(BITMAPINFOHEADER) + dwPaletteSize + ((((pBmiHeader->biWidth*pBmiHeader->biBitCount+31)/32)*4)*pBmiHeader->biHeight);  
    
	GlobalUnlock(hBits);
    return dwSize;
}

BOOL SaveBitmapFromHandle(HGLOBAL hBits,CString szFileName){
	BITMAPFILEHEADER BmfHeader = {0};
	BITMAPINFOHEADER* pBmiHeader = NULL;
	HANDLE hFile = NULL;
	hFile = CreateFile(szFileName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);
	if(hFile == INVALID_HANDLE_VALUE){
		return FALSE;
	}
	// 解析成位图文件头
	//DWORD dwBitsSize = GlobalSize(hBits);
	pBmiHeader = (BITMAPINFOHEADER*)GlobalLock(hBits);
	/*dwPaletteSize = pBmiHeader->biClrUsed * sizeof(RGBQUAD)-ERROR*/
	DWORD dwPaletteSize = 0;
	if(pBmiHeader->biBitCount<24){
		DWORD cClrBits = pBmiHeader->biClrUsed ? pBmiHeader->biClrUsed : 1 << pBmiHeader->biBitCount;
		dwPaletteSize = cClrBits * sizeof(RGBQUAD);
	}


	DWORD dwBytesWritten = 0l;
	BmfHeader.bfType = ((WORD) ('M' << 8) | 'B');
	BmfHeader.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize + ((((pBmiHeader->biWidth*pBmiHeader->biBitCount+31)/32)*4)*pBmiHeader->biHeight);  
    BmfHeader.bfReserved1 = 0;
    BmfHeader.bfReserved2 = 0;
    BmfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dwPaletteSize;
    // 写入位图到文件
    WriteFile(hFile,&BmfHeader,sizeof(BITMAPFILEHEADER),&dwBytesWritten,NULL);  
    WriteFile(hFile,pBmiHeader,BmfHeader.bfSize-sizeof(BITMAPFILEHEADER),&dwBytesWritten,NULL);  
    CloseHandle(hFile);
	hFile = INVALID_HANDLE_VALUE;      
    GlobalUnlock(hBits);
    return TRUE;
}

DWORD GetBitmapHandleSize(HBITMAP hBitmap)
{
	if(hBitmap==NULL)
		return 0;

	PBITMAPINFO pBitmapInfo = GetBitmapInfo(hBitmap);
	PBITMAPINFOHEADER pBitmapHeader;
	
	pBitmapHeader = (PBITMAPINFOHEADER)pBitmapInfo;

	// Get length of the remainder of the file and allocate memory
	DWORD length = pBitmapHeader->biSize + pBitmapHeader->biClrUsed * sizeof(RGBQUAD) + pBitmapHeader->biSizeImage;	
	LocalFree(pBitmapInfo);
	return length;
}

BOOL GetBitmapHandle(HBITMAP hBitmap,int xDPI,int yDPI,HGLOBAL hBits)
{
    HDC hDC;
    DWORD dwPaletteSize = 0, dwBmBitsSize;// 调色板大小，位图数据大小
    BITMAP Bitmap;                  //位图属性结构  
    BITMAPINFOHEADER BmiHeader;            // 位图信息头  
    LPBITMAPINFOHEADER pBmiHeader;        // 指向位图信息头结构     
    HPALETTE hPal, hOldPal=NULL;    // 调色板句柄  

//	BITMAP Bitmap;
	if (!GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&Bitmap))
		return NULL;

	WORD    cClrBits;
	cClrBits = (WORD)(Bitmap.bmPlanes * Bitmap.bmBitsPixel);
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
    else if (cClrBits <= 8)
		cClrBits = 8;
    else if (cClrBits <= 16)
		cClrBits = 16;
    else if (cClrBits <= 24)
		cClrBits = 24;
    else cClrBits = 32;


	if(cClrBits < 24)
		dwPaletteSize = (1 << cClrBits) * sizeof(RGBQUAD);



    BmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    BmiHeader.biWidth = Bitmap.bmWidth;
    BmiHeader.biHeight = Bitmap.bmHeight;
    BmiHeader.biPlanes = 1;
    BmiHeader.biBitCount = cClrBits;
    BmiHeader.biCompression = BI_RGB;
    BmiHeader.biSizeImage = 0;
	if(xDPI>0){
		long bPP = long(xDPI*10000/254 +0.5);
		BmiHeader.biXPelsPerMeter = bPP;  
	}else{
		BmiHeader.biXPelsPerMeter = 0;  
	}

	if(yDPI>0){
		long bPP = long(yDPI*10000/254 +0.5);
		BmiHeader.biYPelsPerMeter = bPP;
	}else{
		BmiHeader.biYPelsPerMeter = 0;
	}

    BmiHeader.biClrUsed = 0;
    BmiHeader.biClrImportant = 0;

	if(cClrBits < 24)
		BmiHeader.biClrUsed = (1<<cClrBits);
	//dwBmBitsSize = ((BmiHeader.biWidth * cClrBits +31) & ~31) /8 * BmiHeader.biHeight;
    dwBmBitsSize = ((Bitmap.bmWidth * cClrBits + 31) / 32) * 4 * Bitmap.bmHeight;  
  
    //hDIB = GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize);  // 为位图内容分配内存
	

    pBmiHeader = (LPBITMAPINFOHEADER)GlobalLock(hBits);  
    *pBmiHeader = BmiHeader;

	/*
	long dwOutBufferSize = GlobalSize(hBits);
	long dwBitsSize = (sizeof(BITMAPINFOHEADER) + dwPaletteSize + dwBmBitsSize);
	if(dwOutBufferSize < dwBitsSize){
		return FALSE;
	}*/


    // 处理调色板  
    hPal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);
    if(hPal){ 
        hDC = GetDC(NULL);  
        hOldPal = SelectPalette(hDC, hPal, FALSE);  
        RealizePalette(hDC); 
		// 获取该调色板下新的像素值  
		GetDIBits(hDC, hBitmap, 0, (UINT)Bitmap.bmHeight, (LPSTR)pBmiHeader + sizeof(BITMAPINFOHEADER) + dwPaletteSize, (BITMAPINFO*)pBmiHeader, DIB_RGB_COLORS);
		if(hOldPal)// 恢复调色板
		{ 
			SelectPalette(hDC, hOldPal, TRUE);
			RealizePalette(hDC);
		}
		ReleaseDC(NULL, hDC);
	}else{
		GlobalUnlock(hBits);
		return FALSE;
	}    
	GlobalUnlock(hBits);
    return TRUE;  
}

HBITMAP MakeBitmapFromHandle(HGLOBAL hBits)
{
	PBITMAPINFO pBitmapInfo = (LPBITMAPINFO)GlobalLock(hBits) ;
	PBITMAPINFOHEADER pBmiHeader = (LPBITMAPINFOHEADER)pBitmapInfo;
	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	
	//int nColors = pBmiHeader->biClrUsed ? pBmiHeader->biClrUsed : 1 << pBmiHeader->biBitCount;

	DWORD dwPaletteSize = 0;
	if(pBmiHeader->biBitCount<24){
		DWORD cClrBits = pBmiHeader->biClrUsed ? pBmiHeader->biClrUsed : 1 << pBmiHeader->biBitCount;
		dwPaletteSize = cClrBits * sizeof(RGBQUAD);
	}
	DWORD dwSizeImage = pBmiHeader->biSizeImage;
	HBITMAP hBitmap = NULL;
	HDC hDC = GetDC(NULL);
	void * pBmBits;
	hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS, &pBmBits, NULL, 0);

	GlobalUnlock(hBits);
	if(pBmBits){
		char* pBitsData = (char*)GlobalLock(hBits);
		memcpy(pBmBits, pBitsData + sizeof(BITMAPINFOHEADER) + dwPaletteSize, dwSizeImage);
		ReleaseDC(NULL, hDC);
		GlobalUnlock(hBits);
	}else{
		return NULL;
	}
	return hBitmap;
}

HBITMAP MakeBitmapFromHandle(HGLOBAL hBits,int &xDPI,int &yDPI)
{
	PBITMAPINFO pBitmapInfo = (LPBITMAPINFO)GlobalLock(hBits) ;
	PBITMAPINFOHEADER pBmiHeader = (LPBITMAPINFOHEADER)pBitmapInfo;
	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	
	//int nColors = pBmiHeader->biClrUsed ? pBmiHeader->biClrUsed : 1 << pBmiHeader->biBitCount;

	DWORD dwPaletteSize = 0;
	if(pBmiHeader->biBitCount<24){
		DWORD cClrBits = pBmiHeader->biClrUsed ? pBmiHeader->biClrUsed : 1 << pBmiHeader->biBitCount;
		dwPaletteSize = cClrBits * sizeof(RGBQUAD);
	}
	
	xDPI = (long)floor(pBmiHeader->biXPelsPerMeter * 254.0 / 10000.0 + 0.5);
	yDPI = (long)floor(pBmiHeader->biYPelsPerMeter * 254.0 / 10000.0 + 0.5);

	DWORD dwSizeImage = pBmiHeader->biSizeImage;
	HBITMAP hBitmap = NULL;
	HDC hDC = GetDC(NULL);
	void * pBmBits;
	hBitmap = CreateDIBSection(hDC, pBitmapInfo, DIB_RGB_COLORS, &pBmBits, NULL, 0);

	GlobalUnlock(hBits);
	if(pBmBits){
		char* pBitsData = (char*)GlobalLock(hBits);
		memcpy(pBmBits, pBitsData + sizeof(BITMAPINFOHEADER) + dwPaletteSize, dwSizeImage);
		ReleaseDC(NULL, hDC);
		GlobalUnlock(hBits);
	}else{
		return NULL;
	}
	return hBitmap;
}

void GetXDPIFromHandle(HGLOBAL hBits,int &xDPI,int &yDPI)
{
	PBITMAPINFOHEADER pBmiHeader = (LPBITMAPINFOHEADER)GlobalLock(hBits);
	xDPI = (long)floor(pBmiHeader->biXPelsPerMeter * 254.0 / 10000.0 + 0.5);
	yDPI = (long)floor(pBmiHeader->biYPelsPerMeter * 254.0 / 10000.0 + 0.5);
	GlobalUnlock(hBits);
}


HBITMAP LoadImage(LPCTSTR sFile)//PNG 不支持
{
	HBITMAP hBitmap = NULL;
	::CoInitialize(NULL);
	HRESULT hr;
	CFile file;
	if(file.Open(sFile, CFile::modeRead | CFile::shareDenyNone)){ // 读入文件内容

		DWORD dwSize = (DWORD)file.GetLength();
		HGLOBAL hMem = ::GlobalAlloc(GHND,dwSize);
		if(hMem==NULL){
			file.Close();
			return NULL;
		}
		LPVOID lpBuf = ::GlobalLock(hMem);
		file.Read(lpBuf, dwSize);
		file.Close();
		::GlobalUnlock(hMem);
		
		IStream * pStream = NULL;
		IPicture * pPicture = NULL;
 
		 // 由HGLOBAL 得到IStream，参数TRUE 表示释放IStream 的同时，释放内存
		 hr = ::CreateStreamOnHGlobal(hMem,TRUE,&pStream );
		 if(SUCCEEDED(hr)){
			 hr = ::OleLoadPicture(pStream, dwSize, TRUE, IID_IPicture, (LPVOID *)&pPicture);
			 if(hr==S_OK){
				 long nWidth,nHeight; // 宽高，MM_HIMETRIC 模式，单位是.01毫米
				 pPicture->get_Width( &nWidth);
				 pPicture->get_Height( &nHeight);

				 HBITMAP hPictureBitmap;
				 pPicture->get_Handle((OLE_HANDLE*)&hPictureBitmap);
				 hBitmap = (HBITMAP)CopyImage(hPictureBitmap,IMAGE_BITMAP,0,0,LR_COPYRETURNORG);
				 //原大显示
				 //CSize sz(nWidth, nHeight);
				 //pDc->HIMETRICtoDP(&sz); // 转换MM_HIMETRIC 模式单位为MM_TEXT 像素单位
				 //pPicture->Render(pDc->m_hDC,0,0,sz.cx,sz.cy,
				 //			   0,nHeight,nWidth,-nHeight,NULL);
 
				 //按窗口尺寸显示
				 // CRect rect; GetClientRect(&rect);
				 // pPicture->Render(pDc->m_hDC,0,0,rect.Width(),rect.Height(),
				 //   0,nHeight,nWidth,-nHeight,NULL);
			 }
		 }
		 if(pStream)
			 pStream->Release();

		 
		 if(pPicture)
			 pPicture->Release();
		 

	}
	 ::CoUninitialize();
	 return hBitmap;
}

HANDLE HandleToGray(HANDLE hBits,BOOL to256Colors)
{
	PBITMAPINFO pBitmapInfo = (LPBITMAPINFO)GlobalLock(hBits);
	PBITMAPINFOHEADER pBmiHeader = (LPBITMAPINFOHEADER)pBitmapInfo;

	DWORD dwPaletteSize = 0;
	if(pBmiHeader->biBitCount<24){
		DWORD cClrBits = pBmiHeader->biClrUsed ? pBmiHeader->biClrUsed : 1 << pBmiHeader->biBitCount;
		dwPaletteSize = cClrBits * sizeof(RGBQUAD);
	}



	DWORD dwSizeImage = pBmiHeader->biSizeImage;
	WORD cClrBits = pBmiHeader->biBitCount;
//	cClrBits = (WORD)(pBmiHeader->bmPlanes * pBmiHeader->bmBitsPixel);

	int NumColors = 0;
	if (cClrBits == 1)
		NumColors = 2;
	else if (cClrBits <= 4)
		NumColors = 16;
    else if (cClrBits <= 8)
		NumColors = 256;
    else if (cClrBits <= 16)
		NumColors = 512;
    else
		NumColors = 0;
 
	DWORD dwWidth = pBmiHeader->biWidth;
	DWORD dwHeight = pBmiHeader->biHeight;
	DWORD dwLineBytes = (DWORD)WIDTHBYTES(pBmiHeader->biWidth*pBmiHeader->biBitCount);


	int NewNumColors = NumColors;//新图颜色数
	int NewBitCount = pBmiHeader->biBitCount;//新图颜色位数

	if(to256Colors){
		if(NewNumColors==0)//真彩图
		{
			NewNumColors=256;
			NewBitCount=8;
		}
    }
   //计算新图的缓冲区大小及每行字节数
	DWORD dwNewLineBytes=(DWORD)WIDTHBYTES(dwWidth*NewBitCount);
	DWORD dwNewBitsSize=(DWORD)(sizeof(BITMAPINFOHEADER)+NewNumColors*sizeof(RGBQUAD)+(DWORD)dwNewLineBytes*dwHeight);

    //为转换后的图分配内存
	HANDLE hNewBits = GlobalAlloc(GHND,dwNewBitsSize);
	if(!hNewBits){
		GlobalUnlock(hBits);
		return NULL;
	}

	PBITMAPINFOHEADER pNewBmiHeader = (LPBITMAPINFOHEADER)GlobalLock(hNewBits);
	memcpy(pNewBmiHeader,pBmiHeader,dwNewBitsSize);

	pNewBmiHeader->biClrUsed =0;
	pNewBmiHeader->biBitCount = NewBitCount;
	pNewBmiHeader->biSizeImage = (DWORD)dwNewLineBytes*dwHeight;

	/*
	GlobalUnlock(hBits);
	GlobalUnlock(hNewBits);
	char* lpNewPtr = (char*)GlobalLock(hNewBits);//(char*)pNewBmiHeader;
	lpNewPtr += sizeof(BITMAPINFOHEADER);

	char* lpPtr = (char*)GlobalLock(hBits);//(char*)pBmiHeader;
	lpPtr += sizeof(BITMAPINFOHEADER);
	*/
	GlobalUnlock(hNewBits);
	
	char* lpNewPtr = (char*)pNewBmiHeader;
	lpNewPtr += sizeof(BITMAPINFOHEADER);

	char* lpPtr = (char*)pBmiHeader;
	lpPtr += sizeof(BITMAPINFOHEADER);
	unsigned char Blue;
	unsigned char Green;
	unsigned char Red;
	unsigned char Alpha;
	unsigned char Gray;


	if(NumColors==0){
		if(to256Colors){////24 to 8 BitCount,增加调色板信息
			for(int i=0;i<256;i++){
				*(lpNewPtr+0) = (unsigned char)i;
				*(lpNewPtr+1) = (unsigned char)i;
				*(lpNewPtr+2) = (unsigned char)i;
				*(lpNewPtr+3) = 0;
				lpNewPtr += 4;
			}
		}
	}else{
		for(int i=0;i<NewNumColors;i++){//转换调色板
				Blue=(unsigned char)(*(lpPtr+0));   
				Green=(unsigned char)(*(lpPtr+1));   
				Red=(unsigned  char)(*(lpPtr+2));   
				Gray=(unsigned char)((float)(Red*0.299+Green*0.587+Blue*0.114));   
				*(lpNewPtr+0)=(unsigned char)Gray;  
				*(lpNewPtr+1)=(unsigned char)Gray; 
				*(lpNewPtr+2)=(unsigned char)Gray; 
				*(lpNewPtr+3)=(unsigned char)0; 
				lpPtr+=4;
				lpNewPtr += 4;
		}
	}

	if(NumColors==0){
		if(to256Colors){
			int nBytes = cClrBits/8;
			for(int i=0;i<(long)dwHeight;i++){
				for(int j=0;j<(long)dwWidth;j++){
					Red =	*(lpPtr + nBytes*j);
					Green = *(lpPtr + nBytes*j + 1);
					Blue =	*(lpPtr + nBytes*j + 2);
					Gray=(unsigned char)((float)(Red*0.299+Green*0.587+Blue*0.114));
					//if(cClrBits==32){
					//	Alpha = *(lpPtr + nBytes*j + 3);
					//}
					*(lpNewPtr + j) = Gray;
				}
				lpPtr += dwLineBytes;
				lpNewPtr += dwNewLineBytes;
			}
		}else{
			int nBytes = cClrBits/8;
			for(int i=0;i<(long)dwHeight;i++){
				for(int j=0;j<(long)dwWidth;j++){
					Red =	*(lpPtr + nBytes*j);
					Green = *(lpPtr + nBytes*j + 1);
					Blue =	*(lpPtr + nBytes*j + 2);
					Gray=(unsigned char)((float)(Red*0.299+Green*0.587+Blue*0.114));
					*(lpNewPtr + nBytes*j) = Gray;
					*(lpNewPtr + nBytes*j + 1) = Gray;
					*(lpNewPtr + nBytes*j + 2) = Gray;
					if(cClrBits==32){
						Alpha = *(lpPtr + nBytes*j + 3);
						*(lpNewPtr + nBytes*j + 3) = Alpha;
					}
				}
				lpPtr += dwLineBytes;
				lpNewPtr += dwNewLineBytes;

			}
		}
	}


	GlobalUnlock(hNewBits);
	GlobalUnlock(hBits);
	return hNewBits;
}

HBITMAP BitmapToGray(HBITMAP hBitmap,int xDPI,int yDPI,BOOL to256Colos=TRUE){
	HBITMAP hResultBitmap = NULL;
	DWORD dwSize = GetBitmapHandleSize(hBitmap);
	HANDLE hBits = GlobalAlloc(GHND,dwSize);
	if(hBits==NULL)
		return NULL;

	GetBitmapHandle(hBitmap,xDPI,yDPI,hBits);
	HANDLE hNewBits = HandleToGray(hBits,to256Colos);
	hResultBitmap = MakeBitmapFromHandle(hNewBits,xDPI,yDPI);
	GlobalFree(hNewBits);
	GlobalFree(hBits);
	return hResultBitmap;
}


BOOL IsMono(HANDLE hBits)
{
	PBITMAPINFO pBitmapInfo = (LPBITMAPINFO)GlobalLock(hBits);
	PBITMAPINFOHEADER pBmiHeader = (LPBITMAPINFOHEADER)pBitmapInfo;

	int cClrBits = pBmiHeader->biBitCount;
	GlobalUnlock(hBits);

	if(cClrBits==1){
		return TRUE;
	}
	return FALSE;
}
HANDLE CombineHANDLE(HANDLE hBitsX,HANDLE hBitsY,int &error){
	error = 0;
	VOID *pData = NULL;

	PBITMAPINFOHEADER pBmpHeaderX;
	pBmpHeaderX = (LPBITMAPINFOHEADER)GlobalLock(hBitsX);  

	PBITMAPINFOHEADER pBmpHeaderY;
	pBmpHeaderY = (LPBITMAPINFOHEADER)GlobalLock(hBitsY); 
	
	if(pBmpHeaderX->biBitCount!=pBmpHeaderY->biBitCount){
		GlobalUnlock(hBitsX);
		GlobalUnlock(hBitsY);
		error = 1;
		return NULL;
	}

	DWORD dwPaletteSizeX = 0;
	if(pBmpHeaderX->biBitCount < 24)
		dwPaletteSizeX = (1 << pBmpHeaderX->biBitCount) * sizeof(RGBQUAD);
	VOID *pDataX = (LPSTR)pBmpHeaderX + sizeof(BITMAPINFOHEADER) + dwPaletteSizeX;

	DWORD dwPaletteSizeY = 0;
	if(pBmpHeaderY->biBitCount < 24)
		dwPaletteSizeY = (1 << pBmpHeaderY->biBitCount) * sizeof(RGBQUAD);
	VOID *pDataY = (LPSTR)pBmpHeaderY + sizeof(BITMAPINFOHEADER) + dwPaletteSizeY;


	
	int cClrBits = pBmpHeaderY->biBitCount>pBmpHeaderX->biBitCount?pBmpHeaderY->biBitCount:pBmpHeaderX->biBitCount;
	if (cClrBits == 1)
		cClrBits = 1;
	else if (cClrBits <= 4)
		cClrBits = 4;
	else if (cClrBits <= 8)
		cClrBits = 8;
	else if (cClrBits <= 16)
		cClrBits = 16;
	else if (cClrBits <= 24)
		cClrBits = 24;
	else cClrBits = 32;

	DWORD bmWidthBytesX = WIDTHBYTES( pBmpHeaderX->biBitCount*pBmpHeaderX->biWidth);
	DWORD bmWidthBytesY = WIDTHBYTES( pBmpHeaderY->biBitCount*pBmpHeaderY->biWidth);
	
	int nDividingLineHeight = 10;
	long biHeight = pBmpHeaderY->biHeight+nDividingLineHeight+pBmpHeaderX->biHeight;
	long biWidth = (pBmpHeaderX->biWidth>pBmpHeaderY->biWidth)?pBmpHeaderX->biWidth:pBmpHeaderY->biWidth;

	DWORD bmWidthBytes = 0;
	if(bmWidthBytesY > bmWidthBytesX){
		bmWidthBytes = bmWidthBytesY;
	}else{
		bmWidthBytes = bmWidthBytesX;
	}
	long dwBitsSize = biHeight * bmWidthBytes;

	DWORD hBitsSize = sizeof(BITMAPINFOHEADER)+(1<< cClrBits)*sizeof(RGBQUAD)+dwBitsSize;
	HANDLE hBits = GlobalAlloc(GHND,hBitsSize);

	if(hBits){
		PBITMAPINFOHEADER pBmpHeader;
		pBmpHeader = (LPBITMAPINFOHEADER)GlobalLock(hBits); 
		ZeroMemory(pBmpHeader,sizeof(BITMAPINFOHEADER));

		pBmpHeader->biBitCount = cClrBits;
		if(cClrBits < 24)
			pBmpHeader->biClrUsed = (1<<cClrBits);
		pBmpHeader->biHeight = biHeight;
		pBmpHeader->biWidth = biWidth;
		pBmpHeader->biSize = sizeof(BITMAPINFOHEADER);
		pBmpHeader->biSizeImage= dwBitsSize;
		pBmpHeader->biClrImportant = 0;
		pBmpHeader->biPlanes = pBmpHeaderX->biPlanes;
		pBmpHeader->biYPelsPerMeter = 0;
		pBmpHeader->biXPelsPerMeter = 0;
	            

		DWORD dwPaletteSize = 0;
		if(pBmpHeader->biBitCount < 24)
			dwPaletteSize = (1 << pBmpHeader->biBitCount) * sizeof(RGBQUAD);
		pData = (LPSTR)pBmpHeader + sizeof(BITMAPINFOHEADER) + dwPaletteSize;

		memset(pData,255,dwBitsSize);
		if(pBmpHeaderY->biBitCount>0){
			int wt = abs((pBmpHeaderY->biWidth - pBmpHeaderX->biWidth))/2;
			wt=0;
			if(bmWidthBytesY > bmWidthBytesX){
				memcpy(pData,pDataY,bmWidthBytesY * pBmpHeaderY->biHeight);
				LPBYTE lpTempPtr,lpPtr;
				for(int h=0;h<pBmpHeaderX->biHeight;h++){
					lpTempPtr=(LPBYTE)pData+h*bmWidthBytesY+(pBmpHeaderY->biHeight)*bmWidthBytesY+nDividingLineHeight*bmWidthBytesY+WIDTHBYTES(pBmpHeader->biBitCount*wt);
					lpPtr=(LPBYTE)pDataX+h*bmWidthBytesX;
					memcpy(lpTempPtr,lpPtr,bmWidthBytesX);
				}
			}else{
				LPBYTE lpTempPtr = NULL,lpPtr = NULL;
				for(int h=0; h<pBmpHeaderY->biHeight; h++){
					lpTempPtr = (LPBYTE)pData+h*bmWidthBytesX+WIDTHBYTES(pBmpHeader->biBitCount*wt);
					lpPtr = (LPBYTE)pDataY+h*bmWidthBytesY;
					memcpy(lpTempPtr,lpPtr,bmWidthBytesY);
				}
				
				memcpy((LPBYTE)pData+pBmpHeaderY->biHeight*bmWidthBytesX + nDividingLineHeight*bmWidthBytesX,pDataX,bmWidthBytesX*pBmpHeaderX->biHeight);
			}
			//拷贝调色板数据
			if(dwPaletteSize>0)
				memcpy((LPBYTE)pData-dwPaletteSize,(LPBYTE)pDataX-dwPaletteSizeX,dwPaletteSize);

		}
	}else{
		error = 2;
	}
	GlobalUnlock(hBitsX);
	GlobalUnlock(hBitsY);
	if(hBits)
		GlobalUnlock(hBits);

	return hBits;
}