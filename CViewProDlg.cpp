// CViewProDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CViewPro.h"
#include "CViewProDlg.h"
#include <io.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//extern BOOL		SaveBitmap(HBITMAP hBitmap,int xDPI,int yDPI,CString szFileName);
//extern BOOL		SaveBitmapFromHandle(HGLOBAL hBits,CString szFileName);
extern DWORD	GetBitmapHandleSize(HBITMAP hBitmap);
extern BOOL		GetBitmapHandle(HBITMAP hBitmap,int xDPI,int yDPI,HGLOBAL hBits);
//extern DWORD	GetHandleSize(HGLOBAL hBits);
//extern HBITMAP	MakeBitmapFromHandle(HGLOBAL hBits);
extern HBITMAP  MakeBitmapFromHandle(HGLOBAL hBits,int &xDPI,int &yDPI);
//extern void		GetXDPIFromHandle(HGLOBAL hBits,int &xDPI,int &yDPI);
//extern BOOL GetWindowsSystemDirectory(char* sPath,int len);
extern void CreateDirectoryEx(CString szFilePath);

extern BOOL MyDeleteFile(CString sFile);
/////////////////////////////////////////////////////////////////////////////
// CCViewProDlg dialog

CCViewProDlg::CCViewProDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCViewProDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCViewProDlg)
		// NOTE: the ClassWizard will add member initialization here
		m_CurrentPath = "";
	//}}AFX_DATA_INIT
}


void CCViewProDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCViewProDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	DDX_Control(pDX, IDC_PIC_VSCROLLBAR, m_vScrollBar);
	DDX_Control(pDX, IDC_PIC_HSCROLLBAR, m_hScrollBar);
	DDX_Control(pDX, IDC_MAIN_PIC, m_Picture);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCViewProDlg, CDialog)
	//{{AFX_MSG_MAP(CCViewProDlg)
	ON_WM_ERASEBKGND()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_WM_COPYDATA()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCViewProDlg message handlers


/*
HBITMAP CopyBitmap(HBITMAP hBitmap,CPalette *pPal)   
{   
    PBITMAPINFO pBitmapInfo = GetBitmapInfo(hBitmap);
	PBITMAPINFOHEADER pBitmapHeader;
	LPBYTE lpBits;
	
	pBitmapHeader = (PBITMAPINFOHEADER)pBitmapInfo;
    lpBits = (LPBYTE) GlobalAlloc(GMEM_FIXED, pBitmapHeader->biSizeImage);
	if(!lpBits)
		return NULL;
	// Retrieve the color table (RGBQUAD array) and the bits
	// (array of palette indices) from the DIB.

	HDC hDC = CreateCompatibleDC(NULL);
    if(!GetDIBits(hDC, hBitmap, 0, (WORD)pBitmapHeader->biHeight, lpBits, pBitmapInfo, DIB_RGB_COLORS)){
		DeleteDC(hDC);
		GlobalFree(lpBits);
		return NULL;
	}

	
	if((pBitmapHeader->biSize + pBitmapHeader->biClrUsed * sizeof(RGBQUAD) + pBitmapHeader->biSizeImage)<=0)
	{
		DeleteDC(hDC);
		GlobalFree(lpBits);
		return NULL;//blank
	}
	  
	// Get length of the remainder of the file and allocate memory
	DWORD nPackedDIBLen = pBitmapHeader->biSize + pBitmapHeader->biClrUsed * sizeof(RGBQUAD) + pBitmapHeader->biSizeImage;	
	HGLOBAL hDIB = ::GlobalAlloc(GMEM_FIXED, nPackedDIBLen);
	if(!hDIB){
		DeleteDC(hDC);
		GlobalFree(lpBits);
		return NULL;
	}

	memcpy((LPSTR)hDIB,(LPVOID)pBitmapHeader,(pBitmapHeader->biSize + pBitmapHeader->biClrUsed * sizeof(RGBQUAD)));
	memcpy((LPSTR)hDIB+(pBitmapHeader->biSize + pBitmapHeader->biClrUsed * sizeof(RGBQUAD)),(LPSTR)lpBits,pBitmapHeader->biSizeImage);

	BITMAPINFOHEADER &bitmapHeader = *(LPBITMAPINFOHEADER)hDIB;
	BITMAPINFO &bitmapInfo = *(LPBITMAPINFO)hDIB ;

	// If bmiHeader.biClrUsed is zero we have to infer the number
	// of colors from the number of bits used to specify it.
	int nColors = bitmapHeader.biClrUsed ? bitmapHeader.biClrUsed : 1 << bitmapHeader.biBitCount;


	LPVOID lpDIBBits;
	if(bitmapInfo.bmiHeader.biBitCount > 8)
		lpDIBBits = (LPVOID)((LPDWORD)(bitmapInfo.bmiColors + bitmapInfo.bmiHeader.biClrUsed) + ((bitmapInfo.bmiHeader.biCompression == BI_BITFIELDS) ? 3 : 0));
	else
		lpDIBBits = (LPVOID)(bitmapInfo.bmiColors + nColors);

	// Create the logical palette
	if(pPal != NULL){
		// Create the palette
		if(nColors <= 256){
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY) * nColors);
			LOGPALETTE *pLP = (LOGPALETTE *) new BYTE[nSize];
			pLP->palVersion = 0x300;
			pLP->palNumEntries = nColors;
			for( int i=0; i < nColors; i++){
				pLP->palPalEntry[i].peRed = bitmapInfo.bmiColors[i].rgbRed;
				pLP->palPalEntry[i].peGreen = bitmapInfo.bmiColors[i].rgbGreen;
				pLP->palPalEntry[i].peBlue = bitmapInfo.bmiColors[i].rgbBlue;
				pLP->palPalEntry[i].peFlags = 0;
			}
			pPal->CreatePalette(pLP);
			delete[] pLP;
		}
	}

	CClientDC dc(NULL);
	CPalette* pOldPalette = NULL;
	if(pPal){
		pOldPalette = dc.SelectPalette(pPal, FALSE );
		dc.RealizePalette();
	}

	HBITMAP hBmp = CreateDIBitmap(dc.m_hDC,&bitmapHeader,CBM_INIT,lpDIBBits,&bitmapInfo,DIB_RGB_COLORS);

	if(pOldPalette)
		dc.SelectPalette(pOldPalette,FALSE);
	
	if(pPal){
		pPal->DeleteObject();
	}
	DeleteDC(hDC);
	LocalFree(pBitmapInfo);
	GlobalFree(hDIB);
	GlobalFree(lpBits);
	return hBmp;

}
*/
BOOL CCViewProDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	memset(&m_ITransHeader,0,sizeof(IMAGETRANSHEADER));
	m_Index = 0;
	m_Behavior = 0;
	m_Shrink = 1;
	m_AutoSize = 0;
	m_RotateAngle = 0.0;
	m_dShrink = 1.0;
	m_ZoomScale = 1.0;
	m_Saved = true;
	m_Picture.SetAlign(1);
	m_BorderColor = RGB(218,224,238);
	m_HoverBorderColor = RGB(218,224,238);
	m_Picture.SetBorderColor(m_BorderColor,m_BorderColor);
	m_Picture.SetHoverBorderColor(m_HoverBorderColor,m_HoverBorderColor);
	m_BackgroundColor = RGB(255,255,255);
	m_Picture.SetBackgroundColor(m_BackgroundColor);
	m_Picture.SetMode(DRAG_MODE);
	
	m_XDPI = 0;
	m_YDPI = 0;

	m_IsBitmapType = TRUE;
	/*
	CxImage* cxi = new CxImage();
	cxi->Load("c:\\pic.png");

	if(cxi->IsValid()){
		hCurBitmap = cxi->MakeBitmap();
		m_XDPI = cxi->GetXDPI();
		m_YDPI = cxi->GetYDPI();
		
	}
	cxi->Destroy();
	*/
	m_EnableWaterMark = 0;
	m_WaterMarkClass = 0;
	m_TextWaterMarkData = "";
	m_ClientIP = "";
	m_ServerDate = "";
	m_WaterMarkData = "";
	m_WaterMarkPostion = 1;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCViewProDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_Picture.OnVScroll(nSBCode, nPos);
	m_vScrollBar.SetScrollPos(m_Picture.GetVScrollPos());
	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CCViewProDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	m_Picture.OnHScroll(nSBCode, nPos);
	m_hScrollBar.SetScrollPos(m_Picture.GetHScrollPos());
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

BOOL CCViewProDlg::OnEraseBkgnd(CDC* pDC)
{

	//	::SendMessage(m_TestHwnd,PNM_DBLCLK,0,0);
	//	return TRUE;
	//return CDialog::OnEraseBkgnd(pDC);
	CRect rect;
	GetClientRect(&rect);

	//pDC->FillSolidRect(&rect,GetSysColor(COLOR_3DFACE));
	//pDC->FillSolidRect(&rect,RGB(255,255,255));

	//只对m_Picture之外的窗体区域做背景刷新，避免m_Picture重绘时的闪烁
	CRect prc;
	::GetWindowRect(m_Picture.m_hWnd,&prc);//屏幕坐标
	ScreenToClient(&prc);//相对于主窗体的客户区坐标

	HRGN rc1 = CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
	HRGN rc2 = CreateRectRgn(prc.left,prc.top,prc.right,prc.bottom);
	CombineRgn(rc1,rc1,rc2,RGN_XOR);

	DeleteObject(rc2);

	if(m_vScrollBar.IsWindowVisible()){
		::GetWindowRect(m_vScrollBar.m_hWnd,&prc);
		ScreenToClient(&prc);
		rc2 = CreateRectRgn(prc.left,prc.top,prc.right,prc.bottom);
		CombineRgn(rc1,rc1,rc2,RGN_XOR);
		DeleteObject(rc2);
	}

	if(m_hScrollBar.IsWindowVisible()){
		::GetWindowRect(m_hScrollBar.m_hWnd,&prc);
		ScreenToClient(&prc);
		rc2 = CreateRectRgn(prc.left,prc.top,prc.right,prc.bottom);
		DeleteObject(rc2);
		CombineRgn(rc1,rc1,rc2,RGN_XOR);
	}


	CRgn rc;
	rc.Attach(rc1);
	CBrush br;
	br.CreateSolidBrush(m_BackgroundColor);
	pDC->FillRgn(&rc,&br);
/*
CRect testRect;
testRect.left = 0;
testRect.top = 0;
testRect.right = rect.Width();
testRect.bottom = 20;

pDC->FillRect(&testRect,&br);
*/

	DeleteObject(rc1);
	br.DeleteObject();
	rc.DeleteObject();
	return true;
}
/*
HBITMAP CCViewProDlg::DoRotate(HBITMAP hBitmap){
	HBITMAP hResultBitmap = hBitmap;
	if(m_RotateAngle!=0.0){
		if(hBitmap){
			CxImage* cxi = new CxImage();
			cxi->CreateFromHBITMAP(hBitmap);
			if(cxi->IsValid()){
				if(m_RotateAngle!=0.0){
					cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
					hResultBitmap = cxi->MakeBitmap();
				}
			}
			cxi->Destroy();
		}
	}
	return hResultBitmap;
}*/


HBITMAP CCViewProDlg::DoRotateAndShrink(HBITMAP hBitmap){

	HBITMAP hResultBitmap = NULL;

	CRect rect;
	GetClientRect(&rect);
	//if(GetBorder()){
		SIZE size;
		size.cx=1;
		size.cy=1;
		rect.DeflateRect(size);
	//}
	m_dShrink = 1.0;
	int prcWidth = rect.Width();
	int prcHeight = rect.Height();

	if(hBitmap){
		CxImage* cxi = new CxImage();
		cxi->CreateFromHBITMAP(hBitmap);
		if(cxi->IsValid()){
			
			if(m_RotateAngle!=0.0){//旋转显示
				cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
			}

			if(m_Shrink){//缩小显示
				CxImage* newcxi = new CxImage();
				int m_height=cxi->GetHeight();
				int m_weight=cxi->GetWidth();
				if(m_weight>prcWidth || m_height>prcHeight){
					double dw = (double)prcWidth/m_weight;
					double dh = (double)prcHeight/m_height;
					m_dShrink = dw<dh?dw:dh;

					if((long)(m_weight*(m_ZoomScale*m_dShrink))>0 && (long)(m_height*(m_ZoomScale*m_dShrink))>0){
						cxi->Resample((long)(m_weight*(m_ZoomScale*m_dShrink)),(long)(m_height*(m_ZoomScale*m_dShrink)),0,newcxi);
						cxi->Destroy();
						//AddWaterMark(newcxi);
						//hResultBitmap = newcxi->MakeBitmap();
						hResultBitmap = AddWaterMark(newcxi);
						newcxi->Destroy();
						return hResultBitmap;
					}
				}else{
					if((long)(m_weight*(m_ZoomScale))>0 && (long)(m_height*(m_ZoomScale))>0){
						cxi->Resample((long)(m_weight*(m_ZoomScale)),(long)(m_height*(m_ZoomScale)),0,newcxi);
						cxi->Destroy();
						//AddWaterMark(newcxi);
						//hResultBitmap = newcxi->MakeBitmap();
						hResultBitmap = AddWaterMark(newcxi);
						newcxi->Destroy();
						return hResultBitmap;
					}
				}
				newcxi->Destroy();
			}else{//原图显示模式
				//m_Picture.SetBitmap(hCurBitmap);//不能直接使用hCurBitmap，使用hCurBitmap的副本
				if(m_ZoomScale>1.0 || m_ZoomScale<1.0){
					CxImage* newcxi = new CxImage();
					int m_height=cxi->GetHeight();
					int m_weight=cxi->GetWidth();
					cxi->Resample((long)(m_weight*(m_ZoomScale)),(long)(m_height*(m_ZoomScale)),0,newcxi);
					cxi->Destroy();
					//AddWaterMark(newcxi);
					//hResultBitmap = newcxi->MakeBitmap();
					hResultBitmap = AddWaterMark(newcxi);
					newcxi->Destroy();
					return hResultBitmap;
				}else{
					//AddWaterMark(cxi);
					//hResultBitmap = cxi->MakeBitmap();
					hResultBitmap = AddWaterMark(cxi);
					cxi->Destroy();
					return hResultBitmap;
				}
			}
		}
		cxi->Destroy();
	}

	return hResultBitmap;
}

void CCViewProDlg::BuildOutward()
{
	//设置statusBar位置
	int statusBarHeight = 0;
	/*
	if(m_statusBar){
		m_statusBar.MoveWindow(0,0,0,0,TRUE);
		CRect rect;
		m_statusBar.GetClientRect(&rect);
		rect.NormalizeRect();
		statusBarHeight = rect.Height();
	}*/

	CRect rect;
	GetClientRect(&rect);

	CRect mainGroupRect;
	GetClientRect(&mainGroupRect);

	//设置main_group的位置
	/*
	CRect mainGroupRect;
	if(m_main_group){

		m_main_group.MoveWindow(245,162,rect.Width()-245-3,rect.Height()-162-5);
		m_main_group.GetWindowRect(&mainGroupRect);
		ScreenToClient(&mainGroupRect);//转化为在主窗口中的ClientRect
	}*/
	//if(GetBorder()){
		SIZE size;
		size.cx=1;
		size.cy=1;	
		mainGroupRect.DeflateRect(size);
	//}

	//if(mainGroupRect)
	int vScrollBarWidth = 0;
	int hScrollBarHeight = 0;


	if(m_Picture){
		//if(hShrinkBitmap){
		HBITMAP hBitmap = m_Picture.GetBitmap();
		if(hBitmap){
			DeleteObject(hBitmap);
			hBitmap = NULL;
		}

		HBITMAP hShrinkBitmap = DoRotateAndShrink(hCurBitmap);

		m_Picture.SetBitmap(hShrinkBitmap);

		//}

		m_Picture.MoveWindow(mainGroupRect);
		if(m_Picture.VScroll()){
			vScrollBarWidth = 18;
			m_vScrollBar.ShowWindow(true);
		}else{
			vScrollBarWidth = 0;
			m_vScrollBar.ShowWindow(false);
		}
		if(m_Picture.HScroll()){
			hScrollBarHeight = 18;
			m_hScrollBar.ShowWindow(true);
		}else{
			hScrollBarHeight = 0;
			m_hScrollBar.ShowWindow(false);
		}

		m_vScrollBar.MoveWindow(mainGroupRect.right-vScrollBarWidth,mainGroupRect.top-1,vScrollBarWidth,mainGroupRect.Height()-hScrollBarHeight+2);
		m_hScrollBar.MoveWindow(mainGroupRect.left-1,mainGroupRect.bottom-hScrollBarHeight,mainGroupRect.Width()-vScrollBarWidth+2,hScrollBarHeight);

		mainGroupRect.right -= vScrollBarWidth;
		mainGroupRect.bottom -= hScrollBarHeight;
		m_Picture.MoveWindow(mainGroupRect);
		m_Picture.Adjust();


		vert.cbSize = sizeof(SCROLLINFO);
		vert.fMask = SIF_ALL;
		vert.nMin = 0;
		vert.nMax = m_Picture.GetVScrollMax();
		vert.nPage =0;
		vert.nPos = m_Picture.GetVScrollPos();
		vert.nTrackPos=0;
	
		m_vScrollBar.SetScrollInfo(&vert);


		horz.cbSize = sizeof(SCROLLINFO);
		horz.fMask = SIF_ALL;
		horz.nMin = 0;
		horz.nMax = m_Picture.GetHScrollMax();
		horz.nPage =0;
		horz.nPos = m_Picture.GetHScrollPos();
		horz.nTrackPos=0;
	
		m_hScrollBar.SetScrollInfo(&horz);
		RestoreAllLightRect();
		Invalidate();
	}
}

void CCViewProDlg::OnSize(UINT nType, int cx, int cy) 
{
	BuildOutward();
	CDialog::OnSize(nType, cx, cy);
}

/*
HBITMAP Skew(HBITMAP hBitmap){
	HBITMAP hResultBitmap = NULL;
	CxImage* cxi = new CxImage();
	cxi->CreateFromHBITMAP(hBitmap);
	if(cxi->IsValid()){
		cxi->Skew(0.5,0.5,100,100,false);
		hResultBitmap = cxi->MakeBitmap();
	}
	cxi->Destroy();
	return hResultBitmap;
}
*/

void SaveFile(CString sFileName,char* pData,long length)
{
	HANDLE hFile = CreateFile(sFileName,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_ARCHIVE,NULL);
	if(hFile!=INVALID_HANDLE_VALUE){
		DWORD dwWriteOfBytes = 0;
		WriteFile(hFile,pData,length,&dwWriteOfBytes,NULL);
		CloseHandle(hFile);
	}
}

BOOL CCViewProDlg::OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct) 
{
	if((pCopyDataStruct->dwData==CM_FROMCENTER_IMAGETRANS || pCopyDataStruct->dwData==CM_COPYFROMVIEW_IMAGETRANS) && pCopyDataStruct->cbData>0){
		if(pCopyDataStruct->cbData >= sizeof(IMAGETRANSHEADER)){
			IMAGETRANSHEADER* pTransHeader = (IMAGETRANSHEADER*)pCopyDataStruct->lpData;
			if(pCopyDataStruct->cbData == (pTransHeader->cbData + sizeof(IMAGETRANSHEADER))){
				if(VerifyTransHeaderSign(pTransHeader,pCopyDataStruct->dwData,pCopyDataStruct->cbData)){					
					//CString sMessage;
					//sMessage.Format("%s-%d",pTransHeader->Id,pTransHeader->cbData);
					//AfxMessageBox(sMessage);
					if(pTransHeader->cbData>0 && pTransHeader->state==IA_NOERROR){
						if(strcmp(pTransHeader->docType,"BMP")==0){
							Clear();
							char* pData = (char*)pCopyDataStruct->lpData;
							memcpy(&m_ITransHeader,pData,sizeof(IMAGETRANSHEADER));

							pData += sizeof(IMAGETRANSHEADER);
							if(pTransHeader->compression==0){
								hCurBitmap = MakeBitmapFromHandle(pData,m_XDPI,m_YDPI);
							}else{
								CxImage* cxi = new CxImage((unsigned char*)pData,pTransHeader->cbData,CXIMAGE_FORMAT_JPG);
								if(cxi->IsValid()){
									if(m_Behavior==1){//缩略显示模式
										int weight=cxi->GetWidth();
										int height=cxi->GetHeight();
										CRect rect;
										GetClientRect(&rect);

										CString sMessage;
										sMessage.Format("%d-%d-%d-%d",rect.Width(),rect.Height(),weight,height);
										//AfxMessageBox(sMessage);

										//if(GetBorder()){
											SIZE size;
											size.cx=1;
											size.cy=1;
											rect.DeflateRect(size);
										//}

										int prcWidth = rect.Width();
										int prcHeight = rect.Height();
										
										if(prcWidth>0 && prcHeight>0){
											if(weight>prcWidth || height>prcHeight){
												CxImage* newcxi = new CxImage();
												double dw = (double)prcWidth/weight;
												double dh = (double)prcHeight/height;
												double shrink = dw<dh?dw:dh;
												cxi->Resample(long(weight*shrink),(long)(height*shrink),0,newcxi);
												hCurBitmap = newcxi->MakeBitmap();
												newcxi->Destroy();
											}else{
												hCurBitmap = cxi->MakeBitmap();
											}
										}else{
											hCurBitmap = cxi->MakeBitmap();
										}
									}else{
										if(m_Shrink){
											if(m_AutoSize==1){
												int weight=cxi->GetWidth();
												int height=cxi->GetHeight();
												CRect rect;
												GetClientRect(&rect);
												//if(GetBorder()){
													SIZE size;
													size.cx=1;
													size.cy=1;
													rect.DeflateRect(size);
												//}

												int prcWidth = rect.Width();
												int prcHeight = rect.Height();
												
												int newWidth = 0;
												if(height>prcHeight){
													newWidth = weight*prcHeight/height;
													if(weight*prcHeight%height>0)
														newWidth += 1;
												}else{
													newWidth = weight;
												}
												newWidth += 2;
												::SendMessage(GetParent()->m_hWnd,WM_ADJUST_SIZE,m_AutoSize,newWidth);

											}else if(m_AutoSize==2){
												int weight=cxi->GetWidth();
												int height=cxi->GetHeight();
												CRect rect;
												GetClientRect(&rect);
												//if(GetBorder()){
													SIZE size;
													size.cx=1;
													size.cy=1;
													rect.DeflateRect(size);
												//}

												int prcWidth = rect.Width();
												int prcHeight = rect.Height();
												
												int newHeight = 0;
												if(weight>prcWidth){
													newHeight = height*prcWidth/weight;
													if(height*prcWidth%weight>0)
														newHeight += 1;
												}else{
													newHeight = height;
												}
												newHeight += 2;
												::SendMessage(GetParent()->m_hWnd,WM_ADJUST_SIZE,m_AutoSize,newHeight);
											}
										}
										hCurBitmap = cxi->MakeBitmap();
									}
								}
								cxi->Destroy();
							}
							m_RotateAngle = 0.0;
							m_ZoomScale = 1.0;
							m_BitmapTrace.Clear();
							ClearRect();
							m_IsBitmapType = TRUE;
						}else{
							Clear();
							char* pData = (char*)pCopyDataStruct->lpData;
							memcpy(&m_ITransHeader,pData,sizeof(IMAGETRANSHEADER));

							CString sPath;
							CString sFileName;
							sPath.Format("%s\\temp\\",m_CurrentPath);
							CreateDirectoryEx(sPath);
							sFileName.Format("%s%s-%s",sPath,m_ITransHeader.Id,m_ITransHeader.serialNumber);
							
							pData += sizeof(IMAGETRANSHEADER);
							SaveFile(sFileName,pData,m_ITransHeader.cbData);
							m_IsBitmapType = FALSE;
							SetDocIcon(pTransHeader->docType);
							return CMTS_SUCCESS;
							//AfxMessageBox(sFileName);
							//AfxMessageBox(sFileName);
						}
					}else{
						Clear();
						char* pData = (char*)pCopyDataStruct->lpData;
						memcpy(&m_ITransHeader,pData,sizeof(IMAGETRANSHEADER));
						if(pTransHeader->state==IA_NOERROR){
							//DOC
							//pTransHeader->docType
						}else{
							SetIcon(pTransHeader->state);
						}
						m_IsBitmapType = FALSE;
					}
					if(hCurBitmap){
						BuildOutward();
					}
					return CMTS_SUCCESS;
				}
			}
		}
		return CMTS_FAIL;
	}	
	return CDialog::OnCopyData(pWnd, pCopyDataStruct);
}


LRESULT CCViewProDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message){
		case WM_REDRAW:{
			if((HWND)wParam == m_Picture.m_hWnd){
				//BuildOutward();
				Invalidate();
			}
			break;
						}
		case WM_CHANGE_SCROLLPOS:{
			/*
			//if(wParam >0 || lParam>0){
				//BuildOutward();
				if(wParam>0){
					int hScrollPos = m_hScrollBar.GetScrollPos();
					m_hScrollBar.SetScrollPos(hScrollPos + wParam);
				}
				if(lParam>0){
					int vScrollPos = m_vScrollBar.GetScrollPos();
					m_vScrollBar.SetScrollPos(vScrollPos + lParam);
				}
				Invalidate();
				*/
				m_hScrollBar.SetScrollPos(m_Picture.GetHScrollPos());
				m_vScrollBar.SetScrollPos(m_Picture.GetVScrollPos());
			//}
			break;
						}
		case WM_MOUSEWHEEL_NOTIFY:{
			if(GetBehavior()==0){
				if(Filled() && IsBitmap()){
					short zDelta = (short)wParam;
					if(zDelta>=0){
						double zomm = m_ZoomScale;
						if(zomm>=1.0){
							zomm += 0.5;
						}
						if(zomm>2.0)
							zomm = 2.0;
						Zoom(zomm);
					}else{
						double zomm = m_ZoomScale;
						if(zomm>1.0){
							zomm -= 0.5;
						}

						if(zomm<1.0)
							zomm = 1.0;
						Zoom(zomm);
					}
				}
			}
			break;
						}
		default:
			break;
	}
	return CDialog::DefWindowProc(message, wParam, lParam);
}

short CCViewProDlg::GetIndex(){
	return m_Index;
}

void CCViewProDlg::SetIndex(short index){
	m_Index = index;
}

short CCViewProDlg::GetBehavior(){
	return m_Behavior;
}

void CCViewProDlg::SetBehavior(short behavior){
	m_Behavior = behavior;
}

short CCViewProDlg::GetShrink(){
	return m_Shrink;
}

void CCViewProDlg::SetShrink(short shrink){
	m_Shrink = shrink;
	BuildOutward();
}

short CCViewProDlg::GetAutoSize(){
	return m_AutoSize;
}

void CCViewProDlg::SetAutoSize(short autosize){
	m_AutoSize = autosize;
}

short CCViewProDlg::GetBorder(){
	return m_Picture.GetBorder();
}

void CCViewProDlg::SetBorder(short border){
	m_Picture.SetBorder(border);
	Invalidate();
}

short CCViewProDlg::GetHoverBorder(){
	return m_Picture.GetHoverBorder();
}

void CCViewProDlg::SetHoverBorder(short border){
	m_Picture.SetHoverBorder(border);
	Invalidate();
}

void CCViewProDlg::SetBorderColor(COLORREF color){
	m_BorderColor = color;
	m_Picture.SetBorderColor(m_BorderColor,m_BorderColor);
	Invalidate();
}

COLORREF CCViewProDlg::GetBorderColor(){
	return m_BorderColor;
}

void CCViewProDlg::SetHoverBorderColor(COLORREF color){
	m_HoverBorderColor = color;
	m_Picture.SetHoverBorderColor(m_HoverBorderColor,m_HoverBorderColor);
	Invalidate();
}

COLORREF CCViewProDlg::GetHoverBorderColor(){
	return m_HoverBorderColor;
}

void CCViewProDlg::SetBackgroundColor(COLORREF color){
	m_BackgroundColor = color;
	m_Picture.SetBackgroundColor(m_BackgroundColor);
	Invalidate();
}

COLORREF CCViewProDlg::GetBackgroundColor(){
	return m_BackgroundColor;
}

BOOL CCViewProDlg::Crop(){
	short result = FALSE;
	if(Filled() && IsBitmap()){
		if(hCurBitmap==NULL)
			return result;

		double dScale = m_ZoomScale*m_dShrink;
		CxImage* cxi = new CxImage();
		if(m_Picture.GetMode()==RECT_MODE){
			CRect rect;
			if(m_Picture.GetSelRect(&rect)){
				cxi->CreateFromHBITMAP(hCurBitmap);
				if(cxi->IsValid()){

					if(m_RotateAngle!=0.0){
						cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
					}

					if(dScale!=1.0){
						long dw = long(rect.Width()/dScale);
						long dh = long(rect.Height()/dScale);
						rect.left = long(rect.left/dScale);
						rect.right = rect.left + dw;
						rect.top = long(rect.top/dScale);
						rect.bottom = rect.top + dh;
					}

					cxi->SelectionClear();
					cxi->SelectionAddRect(rect);
					CRect box;
					cxi->SelectionGetBox(box);
					cxi->Crop(box);
					HBITMAP hNewBitmap= cxi->MakeBitmap();

					CBitmapTrace::TRACERECORD tr;
					tr.Type = CBitmapTrace::TYPE_CROP;
					tr.lRotateAngle = m_RotateAngle;
					tr.cRotateAngle = 0.0;
					tr.lZoomScale = m_ZoomScale;
					tr.cZoomScale = 1.0;
					m_BitmapTrace.AddTrace(TRUE,hNewBitmap,hCurBitmap,&tr);


					DeleteObject(hCurBitmap);
					hCurBitmap = hNewBitmap;
					m_RotateAngle = 0.0;
					m_Saved = false;
					BuildOutward();
					result = TRUE;
				}else{
					result = FALSE;
				}
				
			}
		}else if(m_Picture.GetMode()==POLYGON_MODE){
			CRect rect;
			int n = m_Picture.GetSelPolygonPonitNum();
			if(n>0){
				cxi->CreateFromHBITMAP(hCurBitmap);
				if(cxi->IsValid()){
					if(m_RotateAngle!=0.0){
						cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
					}

					POINT* p = (POINT*)malloc(n*sizeof(POINT));
					if(p){
						if(m_Picture.GetSelPolygon(p,n)){	
							if(dScale!=1.0){			
								for(int i=0;i<n;i++){
									p[i].x=(long)(p[i].x/dScale);
									p[i].y=(long)(p[i].y/dScale);
								}
							}
							cxi->SelectionClear();
							cxi->SelectionAddPolygon(p,n);
						}
						
						CRect box;
						cxi->SelectionGetBox(box);
						cxi->Crop(box);
						HBITMAP hNewBitmap= cxi->MakeBitmap();

						CBitmapTrace::TRACERECORD tr;
						tr.Type = CBitmapTrace::TYPE_CROP;
						tr.lRotateAngle = m_RotateAngle;
						tr.cRotateAngle = 0.0;
						tr.lZoomScale = m_ZoomScale;
						tr.cZoomScale = 1.0;
						m_BitmapTrace.AddTrace(TRUE,hNewBitmap,hCurBitmap,&tr);

						DeleteObject(hCurBitmap);
						hCurBitmap = hNewBitmap;
						m_RotateAngle = 0.0;
						m_Saved = false;
						BuildOutward();				
						free(p);
						result = TRUE;
					}
				}else{
					result = FALSE;
				}
			}
		}
		cxi->Destroy();
	}
	return result;
}

void CCViewProDlg::Zoom(double value){
	if(GetBehavior()==0){
		if(Filled() && IsBitmap()){
			if(m_Shrink==1){
				if(value<1.0)
					value = 1.0;

			}
			if(value>2.0)
				value = 2.0;
			if(value<0.2)
				value = 0.2;
			
			if(value!=m_ZoomScale){
				m_Saved = false;
				CBitmapTrace::TRACERECORD tr;
				tr.Type = CBitmapTrace::TYPE_ZOOM;
				tr.lRotateAngle = m_RotateAngle;
				tr.cRotateAngle = 0.0;
				tr.lZoomScale = m_ZoomScale;
				tr.cZoomScale = value;
				m_BitmapTrace.AddTrace(FALSE,hCurBitmap,NULL,&tr);
				m_ZoomScale = value;
				BuildOutward();
			}
		}
	}
}

void CCViewProDlg::Rotate(float angle){
	if(Filled() && IsBitmap()){
		if(angle!=0.0){
			m_Saved = false;
			CBitmapTrace::TRACERECORD tr;
			tr.Type = CBitmapTrace::TYPE_RORATE;
			tr.lRotateAngle = m_RotateAngle;
			tr.cRotateAngle = angle;
			tr.lZoomScale = m_ZoomScale;
			tr.cZoomScale = 0.0;
			m_BitmapTrace.AddTrace(FALSE,hCurBitmap,NULL,&tr);
		
			m_RotateAngle += angle;
			BuildOutward();
		}
	}
}

BOOL CCViewProDlg::Deskew()
{
	BOOL bResult = FALSE;
	
	if(Filled() && IsBitmap()){
		bResult = TRUE;
	}

	return bResult;
}

BOOL CCViewProDlg::Destain()
{
	BOOL bResult = FALSE;
	
	if(Filled() && IsBitmap()){
	
		bResult = TRUE;
	}

	return bResult;
}

BOOL CCViewProDlg::CanUndo(){
	return m_BitmapTrace.CanUndo();
}

BOOL CCViewProDlg::CanRedo()
{
	return m_BitmapTrace.CanRedo();
}

void CCViewProDlg::Undo()
{
	if(m_BitmapTrace.CanUndo()){
		m_BitmapTrace.Undo();

		CBitmapTrace::TRACERECORD tr;
		tr.Type = CBitmapTrace::TYPE_UNKNOW;
		m_BitmapTrace.GetTrace(&tr);

		if(tr.Type==CBitmapTrace::TYPE_ZOOM){
			m_Saved = false;
			HBITMAP hBitmap = m_BitmapTrace.GetTraceBitmap();
			DeleteObject(hCurBitmap);
			hCurBitmap = hBitmap;
			m_RotateAngle = (float)tr.lRotateAngle;
			m_ZoomScale = tr.lZoomScale;
		}else if(tr.Type==CBitmapTrace::TYPE_RORATE){
			m_Saved = false;
			HBITMAP hBitmap = m_BitmapTrace.GetTraceBitmap();
			DeleteObject(hCurBitmap);
			hCurBitmap = hBitmap;
			m_RotateAngle = (float)tr.lRotateAngle;
			m_ZoomScale = tr.lZoomScale;
		}else if(tr.Type==CBitmapTrace::TYPE_CROP || CBitmapTrace::TYPE_DESKEW || CBitmapTrace::TYPE_DESTAIN){
			m_Saved = false;
			HBITMAP hBitmap = m_BitmapTrace.GetTraceBitmap(1);
			DeleteObject(hCurBitmap);
			hCurBitmap = hBitmap;
			m_RotateAngle = (float)tr.lRotateAngle;
			m_ZoomScale = tr.lZoomScale;
		}
		BuildOutward();
	}
}

void CCViewProDlg::Redo()
{
	if(m_BitmapTrace.CanRedo()){
		CBitmapTrace::TRACERECORD tr;
		tr.Type = CBitmapTrace::TYPE_UNKNOW;
		m_BitmapTrace.GetTrace(&tr);

		if(tr.Type==CBitmapTrace::TYPE_ZOOM){
			m_Saved = false;
			HBITMAP hBitmap = m_BitmapTrace.GetTraceBitmap();
			DeleteObject(hCurBitmap);
			hCurBitmap = hBitmap;
			m_RotateAngle = (float)tr.lRotateAngle;
			m_ZoomScale = tr.cZoomScale;
		}else if(tr.Type==CBitmapTrace::TYPE_RORATE){
			m_Saved = false;
			HBITMAP hBitmap = m_BitmapTrace.GetTraceBitmap();
			DeleteObject(hCurBitmap);
			hCurBitmap = hBitmap;
			m_RotateAngle = (float)tr.lRotateAngle;
			m_RotateAngle += (float)tr.cRotateAngle;
			m_ZoomScale = tr.lZoomScale;
		}else if(tr.Type==CBitmapTrace::TYPE_CROP || CBitmapTrace::TYPE_DESKEW || CBitmapTrace::TYPE_DESTAIN){
			m_Saved = false;
			HBITMAP hBitmap = m_BitmapTrace.GetTraceBitmap(0);
			DeleteObject(hCurBitmap);
			hCurBitmap = hBitmap;
			m_RotateAngle = (float)tr.cRotateAngle;
			m_ZoomScale = tr.cZoomScale;
		}
		
		m_BitmapTrace.Redo();
		BuildOutward();
	}
}


short CCViewProDlg::GetScale(){
	double dScale = m_dShrink * m_ZoomScale;
	return short(dScale*100+0.5);
}


CString CCViewProDlg::Sign(CString szData){
	DWORD dwKey = 0xFE08A682;
	
	CString szDataForSign;
	szDataForSign.Format("%s-%x",szData,dwKey);

	CString szSignData = md5.MDString((char*)(LPCTSTR)szDataForSign);
	szSignData.MakeUpper();
	return szSignData;
}

BOOL CCViewProDlg::VerifySign(CString szData,CString szSign,int len){
	DWORD dwKey = 0xFE08A682;
	
	CString szDataForSign;
	szDataForSign.Format("%s-%x",szData,dwKey);
	CString szSignData = md5.MDString((char*)(LPCTSTR)szDataForSign);
	szSignData.MakeUpper();
	CString szRecvSign = szSign;
	szRecvSign.MakeUpper();
	if(szSignData ==szRecvSign && szSign.GetLength()==len){
		return TRUE;
	}else{
		return FALSE;
	}
}

void CCViewProDlg::TransHeaderSign(IMAGETRANSHEADER* pTransHeader,DWORD dwTransCommand,long nTransDataLength)
{
	CString szDataForSign;
	szDataForSign.Format("%x-%x-%x-%s-%s-%x-%x-%s-%x",dwTransCommand,nTransDataLength,pTransHeader->hwndFrom,pTransHeader->Id,pTransHeader->serialNumber,pTransHeader->recvHandle,pTransHeader->type,pTransHeader->deviceName,pTransHeader->cbData);
	CString szSignData = Sign(szDataForSign);
	pTransHeader->cbSign = szSignData.GetLength();
	memset(pTransHeader->lpSign,0,sizeof(pTransHeader->lpSign));
	memcpy(pTransHeader->lpSign,(char*)(LPCTSTR)szSignData,szSignData.GetLength());
}

BOOL CCViewProDlg::VerifyTransHeaderSign(IMAGETRANSHEADER* pTransHeader,DWORD dwTransCommand,long nTransDataLength)
{
	CString szDataForSign;
	szDataForSign.Format("%x-%x-%x-%s-%s-%x-%x-%s-%x",dwTransCommand,nTransDataLength,pTransHeader->hwndFrom,pTransHeader->Id,pTransHeader->serialNumber,pTransHeader->recvHandle,pTransHeader->type,pTransHeader->deviceName,pTransHeader->cbData);
	return VerifySign(szDataForSign,pTransHeader->lpSign,pTransHeader->cbSign);
	
}

BOOL CCViewProDlg::Save(){
	if(Filled() && IsBitmap() && m_Saved==false){
		if(m_ITransHeader.hwndFrom){
			if(Save(m_ITransHeader.hwndFrom)){
				m_Saved = true;
				return TRUE;
			}else{
				return FALSE;
			}
		}else{
			return FALSE;
		}
	}
	return FALSE;
}

BOOL CCViewProDlg::Remove(){

	BOOL Result = TRUE;
		if(m_ITransHeader.hwndFrom){
			if(Remove(m_ITransHeader.hwndFrom)){
				Result = TRUE;
			}else{
				Result = FALSE;
			}
		}else{
			Result = FALSE;
		}
	if(Result){
		Clear();
	}
	return Result;
}

void CCViewProDlg::Clear(){
	if(IsDoc()){
		CString sPath;
		CString sFileName;
		sPath.Format("%s\\temp\\",m_CurrentPath);
		CreateDirectoryEx(sFileName);
		sFileName.Format("%s%s-%s",sPath,m_ITransHeader.Id,m_ITransHeader.serialNumber);
		MyDeleteFile(sFileName);
	}
	memset(&m_ITransHeader,0,sizeof(IMAGETRANSHEADER));
	m_RotateAngle = 0.0;
	m_dShrink = 1.0;
	m_ZoomScale = 1.0;
	m_Saved = true;
	//m_Picture.SetAlign(1);
	//m_Picture.SetMode(DRAG_MODE);
	m_XDPI = 0;
	m_YDPI = 0;

	m_BitmapTrace.Clear();
	DeleteObject(hCurBitmap);
	hCurBitmap = NULL;
	BuildOutward();
}

BOOL CCViewProDlg::Copy(HWND handle) 
{
	BOOL Result = FALSE;
	if(Filled() && IsBitmap()){
		HWND hWnd = handle;
		if(hWnd!=NULL && hWnd!=m_hWnd){
			HBITMAP hBitmap = GetNewestBitmap();
			if(hBitmap){
				long nBitmapLength = GetBitmapHandleSize(hBitmap);
				if(nBitmapLength>0){
					DWORD dwTransCommand = CM_COPYTOVIEW_IMAGETRANS;
					long nTransDataLength = sizeof(IMAGETRANSHEADER) + nBitmapLength;
					HGLOBAL hTransData = GlobalAlloc(GPTR, nTransDataLength);
					if(hTransData){
						IMAGETRANSHEADER* pTransHeader = (IMAGETRANSHEADER*)hTransData;
						memcpy(pTransHeader,&m_ITransHeader,sizeof(IMAGETRANSHEADER));
						pTransHeader->recvHandle = handle;
						pTransHeader->cbData = nBitmapLength;
						pTransHeader->combine = 0;
						pTransHeader->compression = 0;
						TransHeaderSign(pTransHeader,dwTransCommand,nTransDataLength);

						char* pData = (char*)hTransData;
						pData = pData + sizeof(IMAGETRANSHEADER);
						//char* pBits = (char*)GlobalLock(hBits);
						//memcpy(pData,pBits,nBitmapLength);
						//GlobalUnlock(hBits);
						//GlobalFree(hBits);
						if(GetBitmapHandle(hBitmap,m_XDPI,m_YDPI,pData)){
							COPYDATASTRUCT cpd;
							cpd.dwData = dwTransCommand;
							cpd.cbData = nTransDataLength;
							cpd.lpData = hTransData;
							LRESULT lResult = ::SendMessage(hWnd,WM_COPYDATA,(WPARAM)this->m_hWnd,(LPARAM)&cpd);
							if(lResult == CMTS_SUCCESS){
								Result = TRUE;
							}else{
								Result = FALSE;
							}
						}
						GlobalFree(hTransData);
					}
				}
				DeleteObject(hBitmap);
			}
		}else{
			if(hWnd==m_hWnd)
				return TRUE;
		}
	}
	return Result;
}

BOOL CCViewProDlg::CreateTo(short mode,HWND handle){
	if(Filled() && IsBitmap()){
		if(m_ITransHeader.hwndFrom){
			if(CreateTo(mode,m_ITransHeader.hwndFrom,handle)){
				return TRUE;
			}else{
				return FALSE;
			}
		}else{
			return FALSE;
		}
	}
	return FALSE;
}

HBITMAP CCViewProDlg::GetNewestBitmap(){
	HBITMAP hResultBitmap = NULL;
	
	HBITMAP hBitmap = hCurBitmap;
	if(hBitmap){
		CxImage* cxi = new CxImage();
		cxi->CreateFromHBITMAP(hBitmap);
		if(cxi->IsValid()){
			if(m_RotateAngle!=0.0){
				cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
			}
			if(m_ZoomScale>1.0 || m_ZoomScale<1.0){
				CxImage* newcxi = new CxImage();
				int m_height=cxi->GetHeight();
				int m_weight=cxi->GetWidth();
				cxi->Resample(long(m_weight*(m_ZoomScale)),(long)(m_height*(m_ZoomScale)),0,newcxi);
				hResultBitmap = newcxi->MakeBitmap();
				newcxi->Destroy();
			}else{
				hResultBitmap = cxi->MakeBitmap();
			}
		}
		cxi->Destroy();
	}
	return hResultBitmap;
}

HBITMAP CCViewProDlg::GetNewestRgnBitmap(){
	HBITMAP hResultBitmap = NULL;
	HBITMAP hBitmap = hCurBitmap;
	if(hBitmap){
		double dScale = m_ZoomScale*m_dShrink;
		CxImage* cxi = new CxImage();
		if(m_Picture.GetMode()==RECT_MODE){
			CRect rect;
			if(m_Picture.GetSelRect(&rect)){
				cxi->CreateFromHBITMAP(hBitmap);
				if(cxi->IsValid()){
					if(m_RotateAngle!=0.0){
						cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
					}

					if(dScale!=1.0){
						long dw = long(rect.Width()/dScale);
						long dh = long(rect.Height()/dScale);
						rect.left = long(rect.left/dScale);
						rect.right = rect.left + dw;
						rect.top = long(rect.top/dScale);
						rect.bottom = rect.top + dh;
					}

					cxi->SelectionClear();
					cxi->SelectionAddRect(rect);
					CRect box;
					cxi->SelectionGetBox(box);
					cxi->Crop(box);
					hResultBitmap = cxi->MakeBitmap();
				}
			}
		}else if(m_Picture.GetMode()==POLYGON_MODE){
			CRect rect;
			int n = m_Picture.GetSelPolygonPonitNum();
			if(n>0){
				cxi->CreateFromHBITMAP(hBitmap);
				if(cxi->IsValid()){
					if(m_RotateAngle!=0.0){
						cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
					}

					POINT* p = (POINT*)malloc(n*sizeof(POINT));
					if(p){
						if(m_Picture.GetSelPolygon(p,n)){	
							if(dScale!=1.0){			
								for(int i=0;i<n;i++){
									p[i].x=(long)(p[i].x/dScale);
									p[i].y=(long)(p[i].y/dScale);
								}
							}
							cxi->SelectionClear();
							cxi->SelectionAddPolygon(p,n);
						}
						free(p);
						CRect box;
						cxi->SelectionGetBox(box);
						cxi->Crop(box);
						hResultBitmap = cxi->MakeBitmap();
					}
				}
			}
		}
		cxi->Destroy();
	}
	return hResultBitmap;
}

BOOL CCViewProDlg::Save(HWND handle) 
{
	BOOL Result = FALSE;
	HWND hWnd = handle;
	if(hWnd!=NULL && hWnd!=m_hWnd){
		HBITMAP hBitmap = GetNewestBitmap();
		if(hBitmap){
			long nBitmapLength = GetBitmapHandleSize(hBitmap);
			if(nBitmapLength>0){
				DWORD dwTransCommand = CM_SAVETOCENTER_IMAGETRANS;
				long nTransDataLength = sizeof(IMAGETRANSHEADER) + nBitmapLength;
				HGLOBAL hTransData = GlobalAlloc(GPTR, nTransDataLength);//hTransData 必须GPTR
				if(hTransData){
					IMAGETRANSHEADER* pTransHeader = (IMAGETRANSHEADER*)hTransData;
					memcpy(pTransHeader,&m_ITransHeader,sizeof(IMAGETRANSHEADER));
					pTransHeader->hwndFrom = m_hWnd;
					pTransHeader->cbData = nBitmapLength;
					pTransHeader->combine = 0;
					pTransHeader->compression = 0;
					TransHeaderSign(pTransHeader,dwTransCommand,nTransDataLength);
					char* pData = (char*)hTransData;
					pData = pData + sizeof(IMAGETRANSHEADER);
					if(GetBitmapHandle(hBitmap,m_XDPI,m_YDPI,pData)){
						COPYDATASTRUCT cpd;
						cpd.dwData = dwTransCommand;
						cpd.cbData = nTransDataLength;
						cpd.lpData = hTransData;
						LRESULT lResult = ::SendMessage(hWnd,WM_COPYDATA,(WPARAM)this->m_hWnd,(LPARAM)&cpd);
						if(lResult == CMTS_SUCCESS){
							Result = TRUE;
						}else{
							Result = FALSE;
						}
					}
					GlobalFree(hTransData);
				}
			}
			DeleteObject(hBitmap);
		}
	}
	return Result;
}

BOOL CCViewProDlg::Remove(HWND handle) 
{
	BOOL Result = FALSE;
	HWND hWnd = handle;
	if(hWnd!=NULL && hWnd!=m_hWnd){
		if(true){
			DWORD dwTransCommand = CM_REMOVETOCENTER_IMAGETRANS;
			long nTransDataLength = sizeof(IMAGETRANSHEADER);

			IMAGETRANSHEADER iTransHeader;
			memcpy(&iTransHeader,&m_ITransHeader,sizeof(IMAGETRANSHEADER));
			iTransHeader.hwndFrom = m_hWnd;
			iTransHeader.cbData = 0;
			iTransHeader.combine = 0;
			iTransHeader.compression = 0;
			TransHeaderSign(&iTransHeader,dwTransCommand,nTransDataLength);

			COPYDATASTRUCT cpd;
			cpd.dwData = dwTransCommand;
			cpd.cbData = nTransDataLength;
			cpd.lpData = &iTransHeader;
			LRESULT lResult = ::SendMessage(hWnd,WM_COPYDATA,(WPARAM)this->m_hWnd,(LPARAM)&cpd);
			if(lResult == CMTS_SUCCESS){
				Result = TRUE;
			}else{
				Result = FALSE;
			}
		}
	}
	return Result;
}

BOOL CCViewProDlg::CreateTo(short mode,HWND handle,HWND recvHandle) 
{
	BOOL Result = FALSE;
	HWND hWnd = handle;
	if(hWnd!=NULL && hWnd!=m_hWnd){
		HBITMAP hBitmap = NULL;
		if(mode==0){
			hBitmap = GetNewestBitmap();
		}else{
			hBitmap = GetNewestRgnBitmap();			
		}
		if(hBitmap){
			long nBitmapLength = GetBitmapHandleSize(hBitmap);
			if(nBitmapLength>0){
				
				DWORD dwTransCommand = CM_CREATETOCENTER_IMAGETRANS;
				long nTransDataLength = sizeof(IMAGETRANSHEADER) + nBitmapLength;
				HGLOBAL hTransData = GlobalAlloc(GPTR, nTransDataLength);
				if(hTransData){
					IMAGETRANSHEADER* pTransHeader = (IMAGETRANSHEADER*)hTransData;				
					memcpy(pTransHeader,&m_ITransHeader,sizeof(IMAGETRANSHEADER));

					pTransHeader->hwndFrom = m_hWnd;
					pTransHeader->recvHandle = recvHandle;
					pTransHeader->cbData = nBitmapLength;
					pTransHeader->combine = 0;
					pTransHeader->compression = 0;
					TransHeaderSign(pTransHeader,dwTransCommand,nTransDataLength);

					char* pData = (char*)hTransData;
					pData = pData + sizeof(IMAGETRANSHEADER);	
					if(GetBitmapHandle(hBitmap,m_XDPI,m_YDPI,pData)){
						COPYDATASTRUCT cpd;
						cpd.dwData = dwTransCommand;
						cpd.cbData = nTransDataLength;
						cpd.lpData = hTransData;
						LRESULT lResult = ::SendMessage(hWnd,WM_COPYDATA,(WPARAM)this->m_hWnd,(LPARAM)&cpd);
						if(lResult == CMTS_SUCCESS){
							Result = TRUE;
						}else{
							Result = FALSE;
						}
					}
					GlobalFree(hTransData);
				}
			}
			DeleteObject(hBitmap);
		}
	}
	return Result;
}

CString	CCViewProDlg::GetGId()
{
	CString sGId;
	int len = strlen(m_ITransHeader.Id);
	if(len>0 && len<=sizeof(m_ITransHeader.Id)-2){
		sGId = m_ITransHeader.Id;
	}else{
		return "";
	}
	return sGId;
}

CString	CCViewProDlg::GetIId()
{
	CString sIId;
	int len = strlen(m_ITransHeader.serialNumber);
	if(len>0 && len<=sizeof(m_ITransHeader.serialNumber)-2){
		sIId = m_ITransHeader.serialNumber;
	}else{
		return "";
	}
	return sIId;
}

CString	CCViewProDlg::GetDocType()
{
	CString sDocType;
	int len = strlen(m_ITransHeader.docType);
	if(len>0 && len<=sizeof(m_ITransHeader.docType)-2){
		sDocType = m_ITransHeader.docType;
	}else{
		return "";
	}
	return sDocType;
}


short CCViewProDlg::GetSourceType()
{
	return (int)m_ITransHeader.type;
}

CString CCViewProDlg::GetSourceDevice()
{
	return m_ITransHeader.deviceName;
}

BOOL CCViewProDlg::Filled()
{
	if(m_Picture.GetBitmap()!=NULL && strlen(m_ITransHeader.Id)>0 && strlen(m_ITransHeader.serialNumber)>0 && m_ITransHeader.state==IA_NOERROR){
		return TRUE;
	}else{
		return FALSE;
	}
}

BOOL CCViewProDlg::IsBitmap()
{
	return m_IsBitmapType;
}

BOOL CCViewProDlg::IsDoc()
{
	BOOL IsDoc = FALSE;
	if(Filled()){
		if(m_IsBitmapType==FALSE && strcmp(m_ITransHeader.docType,"BMP")!=0)
			IsDoc = TRUE;
	}
	return IsDoc;
}

void CCViewProDlg::OnDestroy(){
	HBITMAP hBitmap = m_Picture.GetBitmap();
	if(hBitmap)
		DeleteObject(hBitmap);

	Clear();
	CDialog::OnDestroy();		
}

short CCViewProDlg::GetIWidth(){
	BITMAP bmInfo;
	if(hCurBitmap){
		GetObject(hCurBitmap , sizeof(BITMAP), &bmInfo);
		return (short)bmInfo.bmWidth;
	}else{
		return 0;
	}
}

short CCViewProDlg::GetIHeight(){
	BITMAP bmInfo;
	if(hCurBitmap){
		GetObject(hCurBitmap , sizeof(BITMAP), &bmInfo);
		return (short)bmInfo.bmHeight;
	}else{
		return 0;
	}
}

long CCViewProDlg::GetFileSize(){
	long size = 0;
	if(Filled()){
		//size = sizeof(PICTUREFILEHEADER) + sizeof(PICTUREINFO) + m_ITransHeader.cbData的加密长度
		long encBlock = m_ITransHeader.cbData/16;
		size = 8 + 116 + (encBlock+1)*16;
	}

	return size;
}

void CCViewProDlg::SetDocIcon(CString sFilename)
{
	m_IsBitmapType = FALSE;

	//char systemPath[MAX_PATH];
	//::GetSystemDirectory(systemPath,MAX_PATH);
	//GetWindowsSystemDirectory(systemPath,MAX_PATH);

	CString sPath; 
	sPath.Format("%s\\%s\\",m_CurrentPath,RESOURCE_PATH);		

	CxImage* cxi = new CxImage();
	
	CString sFile; 
	sFile.Format("%s%s.%s",sPath,sFilename,"jpg");

	if(_access(sFile,0)!=0){//文件不存在
		sFile.Format("%s%s.%s",sPath,"default","jpg");
	}
	cxi->Load(sFile);

	if(cxi->IsValid()){
		hCurBitmap = cxi->MakeBitmap();
		m_XDPI = cxi->GetXDPI();
		m_YDPI = cxi->GetYDPI();

		//增加文档时显示的图标大小的自适应（与图片方式保持一致）
		if(m_Behavior==1){//缩略显示模式
			int weight=cxi->GetWidth();
			int height=cxi->GetHeight();
			CRect rect;
			GetClientRect(&rect);

			CString sMessage;
			sMessage.Format("%d-%d-%d-%d",rect.Width(),rect.Height(),weight,height);
			//AfxMessageBox(sMessage);

			//if(GetBorder()){
				SIZE size;
				size.cx=1;
				size.cy=1;
				rect.DeflateRect(size);
			//}

			int prcWidth = rect.Width();
			int prcHeight = rect.Height();
			
			if(prcWidth>0 && prcHeight>0){
				if(weight>prcWidth || height>prcHeight){
					CxImage* newcxi = new CxImage();
					double dw = (double)prcWidth/weight;
					double dh = (double)prcHeight/height;
					double shrink = dw<dh?dw:dh;
					cxi->Resample(long(weight*shrink),(long)(height*shrink),0,newcxi);
					hCurBitmap = newcxi->MakeBitmap();
					newcxi->Destroy();
				}else{
					hCurBitmap = cxi->MakeBitmap();
				}
			}else{
				hCurBitmap = cxi->MakeBitmap();
			}
		}else{
			if(m_Shrink){
				if(m_AutoSize==1){
					int weight=cxi->GetWidth();
					int height=cxi->GetHeight();
					CRect rect;
					GetClientRect(&rect);
					//if(GetBorder()){
						SIZE size;
						size.cx=1;
						size.cy=1;
						rect.DeflateRect(size);
					//}

					int prcWidth = rect.Width();
					int prcHeight = rect.Height();
					
					int newWidth = 0;
					if(height>prcHeight){
						newWidth = weight*prcHeight/height;
						if(weight*prcHeight%height>0)
							newWidth += 1;
					}else{
						newWidth = weight;
					}
					newWidth += 2;
					::SendMessage(GetParent()->m_hWnd,WM_ADJUST_SIZE,m_AutoSize,newWidth);

				}else if(m_AutoSize==2){
					int weight=cxi->GetWidth();
					int height=cxi->GetHeight();
					CRect rect;
					GetClientRect(&rect);
					//if(GetBorder()){
						SIZE size;
						size.cx=1;
						size.cy=1;
						rect.DeflateRect(size);
					//}

					int prcWidth = rect.Width();
					int prcHeight = rect.Height();
					
					int newHeight = 0;
					if(weight>prcWidth){
						newHeight = height*prcWidth/weight;
						if(height*prcWidth%weight>0)
							newHeight += 1;
					}else{
						newHeight = height;
					}
					newHeight += 2;
					::SendMessage(GetParent()->m_hWnd,WM_ADJUST_SIZE,m_AutoSize,newHeight);
				}
			}
			hCurBitmap = cxi->MakeBitmap();
		}
	}
	cxi->Destroy();
	if(hCurBitmap){
		BuildOutward();
	}
}

void CCViewProDlg::SetIcon(short icon)
{
	m_IsBitmapType = FALSE;

	//char systemPath[MAX_PATH];
	//::GetSystemDirectory(systemPath,MAX_PATH);
	//GetWindowsSystemDirectory(systemPath,MAX_PATH);

	CString sPath; 
	sPath.Format("%s\\%s\\",m_CurrentPath,RESOURCE_PATH);		

	CxImage* cxi = new CxImage();
	
	if(icon==IA_FILE_NOTEXIST){
		//hCurBitmap = LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_NOTEXIST_BITMAP));
		CString sFile; 
		sFile.Format("%s%s",sPath,"NotExist.jpg");
		cxi->Load(sFile);
	}else if(icon==IA_FILE_ERROR){
		//hCurBitmap = LoadBitmap(AfxGetApp()->m_hInstance,MAKEINTRESOURCE(IDB_ERROR_BITMAP));
		CString sFile; 
		sFile.Format("%s%s",sPath,"Error.jpg");
		cxi->Load(sFile);

	}

	if(cxi->IsValid()){
		hCurBitmap = cxi->MakeBitmap();
		m_XDPI = cxi->GetXDPI();
		m_YDPI = cxi->GetYDPI();
		
	}
	cxi->Destroy();
	if(hCurBitmap){
		BuildOutward();
	}
}

void CCViewProDlg::SetCurrentPath(LPCTSTR sPath)
{
	m_CurrentPath = sPath;
}

std::string GetBase64DataFromFile(char* filename)
{
	std::string base64Data = "";
	HANDLE hFile = CreateFile(filename,GENERIC_READ | GENERIC_WRITE,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_ARCHIVE,NULL);
	if(hFile!=INVALID_HANDLE_VALUE){
			DWORD dwFileSize = GetFileSize(hFile,NULL);
			DWORD dwBitsSize = 0;
			DWORD dwReadOfBytes = 0;
			char* buffer = (char*)malloc(dwFileSize);
			if(buffer){
				ReadFile(hFile,buffer,dwFileSize,&dwReadOfBytes,NULL);
				CloseHandle(hFile);
				Base64 base64;
				base64Data = base64.base64_encode((const unsigned char*)buffer,dwFileSize);
				free(buffer);
			}else{
				CloseHandle(hFile);
			}
	}
	return base64Data;
}

CString CCViewProDlg::GetImage(short quality,double scale){
	CString sBaseData = "";
	HBITMAP hBitmap = hCurBitmap;
	if(hBitmap){
		CxImage* cxi = new CxImage();
		cxi->CreateFromHBITMAP(hBitmap);
			if(cxi->IsValid()){

				if(m_RotateAngle!=0.0){
					cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
				}

				if(scale<=1.0){
					if(cxi->GetBpp()==1){//黑白图，不支持JPG(Encode之后nSize=0)
						cxi->GrayScale();
					}else if(cxi->GetBpp()<24){
						cxi->IncreaseBpp(24);
					}
					CxImage* newcxi = new CxImage();
					cxi->Resample((long)(cxi->GetWidth()*scale),(long)(cxi->GetHeight()*scale),0,newcxi);
					
					newcxi->SetJpegQuality((BYTE)quality);
					unsigned char* pBuffer = NULL;
					long nSize = 0;
					newcxi->Encode(pBuffer,nSize,CXIMAGE_FORMAT_JPG);
					if(pBuffer && nSize>0){
						Base64 base64;
						sBaseData = (base64.base64_encode((const unsigned char*)pBuffer,nSize)).c_str();
					}
					newcxi->FreeMemory(pBuffer);
					
					/*
					CString sFile;
					sFile.Format("%s\\temp",m_CurrentPath);
					newcxi->Save(sFile,CXIMAGE_FORMAT_BMP);
					sBaseData = GetBase64DataFromFile((char*)(LPCTSTR)sFile).c_str();
					MyDeleteFile(sFile);
					*/
					newcxi->Destroy();
			}
		}	
		cxi->Destroy();
	}
	return sBaseData;
}

void CCViewProDlg::MoveImage(int x,int y,int newX,int newY)
{
	int cx = x - newX;
	int cy = y - newY;


	BOOL Roll = FALSE;
	if(m_Picture.GetHScrollMax()>0){
		long hCurrentPos = m_Picture.GetHScrollPos();
		long hWillPos = cx;
		if(hWillPos > m_Picture.GetHScrollMax()){
			hWillPos = m_Picture.GetHScrollMax();
		}
		if(hWillPos < 0){
			hWillPos = 0;
		}
		if(hWillPos!=hCurrentPos){
			m_Picture.SetHScrollPos(hWillPos);
			Roll = TRUE;
		}
	}
	if(m_Picture.GetVScrollMax()>0){
		long hCurrentPos = m_Picture.GetVScrollPos();
		long hWillPos = cy;
		if(hWillPos > m_Picture.GetVScrollMax()){
			hWillPos = m_Picture.GetVScrollMax();
		}
		if(hWillPos < 0){
			hWillPos = 0;
		}
		if(hWillPos!=hCurrentPos){
			m_Picture.SetVScrollPos(hWillPos);
			Roll = TRUE;
		}
	}
	if(Roll){
		m_hScrollBar.SetScrollPos(m_Picture.GetHScrollPos());
		m_vScrollBar.SetScrollPos(m_Picture.GetVScrollPos());
		m_Picture.Invalidate();
	}
}

void CCViewProDlg::SetBackground(short background)
{
	if(background==0){
		m_Picture.SetBackgroundBitmap(NULL);
	}else{
		CBitmap BackgroundBitmap;
		BackgroundBitmap.LoadBitmap(IDB_BACKGROUND_BITMAP);
		m_Picture.SetBackgroundBitmap((HBITMAP)BackgroundBitmap.Detach());
		BackgroundBitmap.DeleteObject();
	}
}

void CCViewProDlg::CreateRect(int left,int top,int right,int bottom,short thickness,COLORREF color)
{
	LIGHT_RECT_INFO rectInfo;
	rectInfo.rect.left = left;
	rectInfo.rect.top = top;
	rectInfo.rect.right = right;
	rectInfo.rect.bottom = bottom;
	rectInfo.thickness = thickness;
	rectInfo.color = color;

	m_AllLightRect.push_back(rectInfo);

	double dScale = m_dShrink * m_ZoomScale;
	m_Picture.CreateRect(int(left*dScale+0.5),int(top*dScale+0.5),int(right*dScale+0.5),int(bottom*dScale+0.5),thickness,color);
}

void CCViewProDlg::ClearRect()
{
	m_AllLightRect.erase(m_AllLightRect.begin(),m_AllLightRect.end());
	m_Picture.ClearRect();
}

void CCViewProDlg::RestoreAllLightRect()
{
	if(m_AllLightRect.size()>0){
		unsigned int index = 0;
		for(index=0;index<m_AllLightRect.size();index++){
			LIGHT_RECT_INFO rectInfo = m_AllLightRect[index];
			RECT rect = rectInfo.rect;
			int left = rect.left;
			int top = rect.top;
			int right = rect.right;
			int bottom = rect.bottom;
			short thickness = rectInfo.thickness;
			COLORREF color = rectInfo.color;
			double dScale = m_dShrink * m_ZoomScale;
			m_Picture.CreateRect(int(left*dScale+0.5),int(top*dScale+0.5),int(right*dScale+0.5),int(bottom*dScale+0.5),thickness,color);
		}
	}
}
/*
void CCViewProDlg::AddWaterMark(CxImage* cxi){
	//白色背景BMP/JPG/PNG图片
	if(m_EnableWaterMark>0){
		CxImage waterMark;

		if(m_WaterMarkData.size()>0){
			waterMark.Decode((BYTE*)m_WaterMarkData.c_str(),m_WaterMarkData.size(),CXIMAGE_FORMAT_UNKNOWN);//使用CXIMAGE_FORMAT_PNG反倒不行，CxImage的BUG?
		}else{
			CString sModuleName;
			sModuleName.Format("%s\\%s",m_CurrentPath,"CViewPro.ocx");//DLL/OCX中必须使用相应的HINSTANCE，否则获得不了资源
			HINSTANCE hInst = GetModuleHandle(sModuleName); 
			HRSRC hRsrc = FindResource(hInst,MAKEINTRESOURCE(IDB_BACKGROUND_BITMAP),RT_BITMAP);
			if(hRsrc){
				if(waterMark.LoadResource(hRsrc,CXIMAGE_FORMAT_BMP,hInst)){
				}
			}

		}

		if(waterMark.IsValid()){
			//ImageOpType { OpAdd, OpAnd, OpXor, OpOr, OpMask, OpSrcCopy, OpDstCopy, OpSub, OpSrcBlend };
	
			if(m_WaterMarkPostion<=1){
				cxi->Mix(waterMark,CxImage::OpAnd,0,(long)(waterMark.GetHeight()-cxi->GetHeight()),true);
			}else if(m_WaterMarkPostion==2){
				cxi->Mix(waterMark,CxImage::OpAnd,(long)(waterMark.GetWidth()-cxi->GetWidth()),(long)(waterMark.GetHeight()-cxi->GetHeight()),true);
			}else if(m_WaterMarkPostion==3){
				cxi->Mix(waterMark,CxImage::OpAnd,0,0,true);
			}else if(m_WaterMarkPostion==4){
				cxi->Mix(waterMark,CxImage::OpAnd,(long)(waterMark.GetWidth()-cxi->GetWidth()),0,true);
			}else{
				cxi->Mix(waterMark,CxImage::OpAnd,(long)((waterMark.GetWidth()-cxi->GetWidth()))/2,(long)((waterMark.GetHeight()-cxi->GetHeight()))/2,true);
			}
		}
		waterMark.Destroy();
	}
}
*/
HBITMAP CCViewProDlg::CreateWaterMarkBitmap(HDC hdc, const char *pszText,const char *pszIP,const char *pszDate)
{
    if(pszText == NULL)
        return NULL;

	int width = 124+strlen(pszText)*14;
	int height = width;
    HBITMAP hBitmap = ::CreateCompatibleBitmap(hdc, width, height);
    HDC hMemDC = ::CreateCompatibleDC(hdc);

    SelectObject(hMemDC,hBitmap);
    SetBkMode(hMemDC, TRANSPARENT);
    RECT rc = {0, 0, width, height};
    HBRUSH hb = ::CreateSolidBrush(RGB(255, 255, 255));
    FillRect(hMemDC, &rc, hb);
	CFont font;
	font.CreateFont(
	40, // nHeight
	0, // nWidth
	450, // nEscapement
	450, // nOrientation
	0, // nWeight
	FALSE, // bItalic
	FALSE, // bUnderline
	0, // cStrikeOut
	ANSI_CHARSET, // nCharSet
	OUT_DEFAULT_PRECIS, // nOutPrecision
	CLIP_LH_ANGLES,//CLIP_DEFAULT_PRECIS, // nClipPrecision
	DEFAULT_QUALITY, // nQuality
	DEFAULT_PITCH | FF_SWISS,
	_T("宋体") // nPitchAndFamily Arial
	); 
	::SelectObject(hMemDC,font);
	::SetTextColor(hMemDC,RGB(192,192,192));

    //::DrawText(hMemDC, pszText, -1, &rc,  DT_CENTER|DT_SINGLELINE);
    ::TextOut(hMemDC, 0,rc.bottom-40,pszText,strlen(pszText));

	CFont fontIP;
	fontIP.CreateFont(
	16, // nHeight
	0, // nWidth
	450, // nEscapement
	450, // nOrientation
	0, // nWeight
	FALSE, // bItalic
	FALSE, // bUnderline
	0, // cStrikeOut
	ANSI_CHARSET, // nCharSet
	OUT_DEFAULT_PRECIS, // nOutPrecision
	CLIP_LH_ANGLES,//CLIP_DEFAULT_PRECIS, // nClipPrecision
	DEFAULT_QUALITY, // nQuality
	DEFAULT_PITCH | FF_SWISS,
	_T("宋体") // nPitchAndFamily Arial
	); 
	::SelectObject(hMemDC,fontIP);
	::SetTextColor(hMemDC,RGB(192,192,192));

    //::DrawText(hMemDC, pszText, -1, &rc,  DT_CENTER|DT_SINGLELINE);
    ::TextOut(hMemDC, 6+strlen(pszText)*14,rc.bottom-(40+strlen(pszText)*14),pszIP,strlen(pszIP));
    ::TextOut(hMemDC, 16+strlen(pszText)*14,rc.bottom-(28+strlen(pszText)*14),pszDate,strlen(pszDate));
    //::TextOut(hMemDC, 90,140,pszDate,strlen(pszDate));

	/*
	CPen pen;
	int PenStyle = PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE;
	pen.CreatePen(PenStyle, 6, RGB(192,192,192));
	SelectObject(hMemDC,pen);
	::MoveToEx(hMemDC,0,100,0);
	::LineTo(hMemDC,100,0);
	::LineTo(hMemDC,rc.right,rc.bottom-100);
	::LineTo(hMemDC,rc.bottom-100,rc.bottom);
	::LineTo(hMemDC,0,100);
    ::DeleteObject(pen);
*/

    ::DeleteDC(hMemDC);
    ::DeleteObject(font);
    ::DeleteObject(fontIP);
    ::DeleteObject(hb);
    return hBitmap;
}
HBITMAP CCViewProDlg::AddWaterMark(CxImage* cxi,CxImage* cxiWater,double opacity,int postion)
{
	HBITMAP hBitmap = NULL;
	if(cxi->IsValid() && cxiWater->IsValid()){
		if(cxi->GetBpp()<24){
			cxi->IncreaseBpp(24);
		}
		hBitmap = cxi->MakeBitmap();

		BITMAP Bmp;
		GetObject(hBitmap , sizeof(BITMAP), &Bmp);
		long cbBuffer = Bmp.bmWidthBytes * Bmp.bmHeight;
		BYTE* lpvBits = new BYTE[cbBuffer];
		::ZeroMemory(lpvBits, cbBuffer);
		if(GetBitmapBits(hBitmap,cbBuffer,lpvBits)>0){
			if(cxiWater->GetBpp()<24){
				cxiWater->IncreaseBpp(24);
			}
			HBITMAP hBitmapWater = cxiWater->MakeBitmap();

			BITMAP BmpWater;
			GetObject(hBitmapWater , sizeof(BITMAP), &BmpWater);
			long cbBufferWater = BmpWater.bmWidthBytes * BmpWater.bmHeight;
			BYTE* lpvBitsWater = new BYTE[cbBufferWater];
			::ZeroMemory(lpvBitsWater, cbBufferWater);
			if(GetBitmapBits(hBitmapWater,cbBufferWater,lpvBitsWater)>0){
				long startX = 0;
				long startXWater = 0;
				long startY = 0;
				long startYWater = 0;
				long doWidth = 0;
				long doHeight = 0;
				
				if(postion<=1){
					startX = 0;
					startXWater = 0;
					startY = 0;
					startYWater = 0;
				}else if(postion==2){
					startX = Bmp.bmWidth - BmpWater.bmWidth;
					startXWater = 0;
					if(startX<0){
						startXWater = -1*startX;
						startX = 0;
					}
					startY = 0;
					startYWater = 0;
				}else if(postion==3){
					startX = 0;
					startXWater = 0;
					
					startY = Bmp.bmHeight - BmpWater.bmHeight;
					startYWater = 0;
					if(startY<0){
						startYWater = -1*startY;
						startY = 0;
					}
				}else if(postion==4){
					startX = Bmp.bmWidth - BmpWater.bmWidth;
					startXWater = 0;
					if(startX<0){
						startXWater = -1*startX;
						startX = 0;
					}
					startY = Bmp.bmHeight - BmpWater.bmHeight;
					startYWater = 0;
					if(startY<0){
						startYWater = -1*startY;
						startY = 0;
					}
				}else{
					startX = (Bmp.bmWidth - BmpWater.bmWidth)/2;
					if(startX<0){
						startXWater = -1*startX;
						startX = 0;
					}

					startY = (Bmp.bmHeight - BmpWater.bmHeight)/2;
					if(startY<0){
						startYWater = -1*startY;
						startY = 0;
					}
				}

				doWidth = Bmp.bmWidth>BmpWater.bmWidth?BmpWater.bmWidth:Bmp.bmWidth;
				doHeight = Bmp.bmHeight>BmpWater.bmHeight?BmpWater.bmHeight:Bmp.bmHeight;

				double filp_opacity = 1.0 - opacity;
				int BytePixel = Bmp.bmBitsPixel/8;
				long i = 0;
				long j = 0;
				for(int m=startY;m<(startY+doHeight);m++){
					for(int n=startX;n<(startX+doWidth);n++){
						i=(m*((long)Bmp.bmWidth)+n)*BytePixel;
						j = ((m-startY+startYWater)*((long)BmpWater.bmWidth)+(n-startX+startXWater))*BytePixel;
						if(i<cbBuffer && j<cbBufferWater){
							if(lpvBitsWater[j]!=255 && lpvBitsWater[j+1]!=255 && lpvBitsWater[j+2]!=255){
								lpvBits[i+0] = (byte)(lpvBits[i+0]*filp_opacity + opacity*lpvBitsWater[j+0]);
								lpvBits[i+1] = (byte)(lpvBits[i+1]*filp_opacity + opacity*lpvBitsWater[j+1]);
								lpvBits[i+2] = (byte)(lpvBits[i+2]*filp_opacity + opacity*lpvBitsWater[j+2]);
							}
						}
					}
				}
			}
			delete[] lpvBitsWater;
			SetBitmapBits(hBitmap,cbBuffer, lpvBits);
			//SaveBitmap(hBitmap,0,0,"c:\\1-1.bmp");
		}
		delete[] lpvBits;
	}
	return hBitmap;
}

HBITMAP CCViewProDlg::AddWaterMark(CxImage* cxi){
	//白色背景BMP/JPG/PNG图片
	HBITMAP hWaterMarkBitmap = NULL;
	if(cxi->IsValid()){
		if(m_EnableWaterMark>0){
			if(m_WaterMarkClass==0){//文本水印
				if(m_TextWaterMarkData.size()>0){
					HDC hdc = ::GetDC(m_hWnd);
					HBITMAP hBitmap = CreateWaterMarkBitmap(hdc,m_TextWaterMarkData.c_str(),m_ClientIP.c_str(),m_ServerDate.c_str());
					if(hBitmap){
						CxImage* waterMark = new CxImage();
						waterMark->CreateFromHBITMAP(hBitmap);
						//waterMark->Rotate2(45,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
						DeleteObject(hBitmap);
						if(waterMark->IsValid()){
							hWaterMarkBitmap = AddWaterMark(cxi,waterMark,0.5,m_WaterMarkPostion);
						}
						waterMark->Destroy();
					}
				}
				if(hWaterMarkBitmap==NULL){
					hWaterMarkBitmap = cxi->MakeBitmap();
				}
			}else{//图片水印
				CxImage* waterMark = new CxImage();
				if(m_WaterMarkData.size()>0){
					waterMark->Decode((BYTE*)m_WaterMarkData.c_str(),m_WaterMarkData.size(),CXIMAGE_FORMAT_UNKNOWN);//使用CXIMAGE_FORMAT_PNG反倒不行，CxImage的BUG?
					if(waterMark->IsValid()){
						hWaterMarkBitmap = AddWaterMark(cxi,waterMark,0.5,m_WaterMarkPostion);
					}
				}else{
					CString sModuleName;
					sModuleName.Format("%s\\%s",m_CurrentPath,"CViewPro.ocx");//DLL/OCX中必须使用相应的HINSTANCE，否则获得不了资源
					HINSTANCE hInst = GetModuleHandle(sModuleName); 
					HRSRC hRsrc = FindResource(hInst,MAKEINTRESOURCE(IDB_BACKGROUND_BITMAP),RT_BITMAP);
					if(hRsrc){
						if(waterMark->LoadResource(hRsrc,CXIMAGE_FORMAT_BMP,hInst)){//默认水印仍然使用默认默片和处理方式
							if(m_WaterMarkPostion<=1){
								cxi->Mix(*waterMark,CxImage::OpAnd,0,(long)(waterMark->GetHeight()-cxi->GetHeight()),true);
							}else if(m_WaterMarkPostion==2){
								cxi->Mix(*waterMark,CxImage::OpAnd,(long)(waterMark->GetWidth()-cxi->GetWidth()),(long)(waterMark->GetHeight()-cxi->GetHeight()),true);
							}else if(m_WaterMarkPostion==3){
								cxi->Mix(*waterMark,CxImage::OpAnd,0,0,true);
							}else if(m_WaterMarkPostion==4){
								cxi->Mix(*waterMark,CxImage::OpAnd,(long)(waterMark->GetWidth()-cxi->GetWidth()),0,true);
							}else{
								cxi->Mix(*waterMark,CxImage::OpAnd,(long)((waterMark->GetWidth()-cxi->GetWidth()))/2,(long)((waterMark->GetHeight()-cxi->GetHeight()))/2,true);
							}
							hWaterMarkBitmap = cxi->MakeBitmap();
						}
					}
				}
				waterMark->Destroy();
			}
		}else{
			hWaterMarkBitmap = cxi->MakeBitmap();
		}
	}
	return hWaterMarkBitmap;
}

void CCViewProDlg::EnableWaterMark(short enable){
	m_EnableWaterMark = enable;
}

void CCViewProDlg::SetWaterMark(LPCTSTR sWaterMarkBase64,short nPostion)
{
	m_WaterMarkClass = 1;
	Base64 base64;
	m_WaterMarkData = base64.base64_decode((unsigned char*)sWaterMarkBase64,strlen(sWaterMarkBase64));
	m_WaterMarkPostion = nPostion;
	if(Filled() && IsBitmap()){
		BuildOutward();
	}
}

void CCViewProDlg::SetTextWaterMark(LPCTSTR sText,LPCTSTR sIP,LPCTSTR sDate,short nPostion)
{
	m_WaterMarkClass = 0;
	m_TextWaterMarkData = sText;
	m_ClientIP = sIP;
	m_ServerDate = sDate;
	m_WaterMarkPostion = nPostion;
	if(Filled() && IsBitmap()){
		BuildOutward();
	}
}

void CCViewProDlg::AddSignature(LPCTSTR sSignatureBase64,long xOffset,long yOffset)
{
	BOOL Success = FALSE;
	if(Filled() && IsBitmap()){
		if(strlen(sSignatureBase64)>0){
			Base64 base64;
			std::string sSignatureData = base64.base64_decode((unsigned char*)sSignatureBase64,strlen(sSignatureBase64));
			if(sSignatureData.size()>0){
				CxImage cxiSign;
				cxiSign.Decode((BYTE*)sSignatureData.c_str(),sSignatureData.size(),CXIMAGE_FORMAT_UNKNOWN);
				if(cxiSign.IsValid()){
					if(hCurBitmap){
						CxImage cxi;
						cxi.CreateFromHBITMAP(hCurBitmap);
						if(cxi.IsValid()){
							cxi.Mix(cxiSign,CxImage::OpAnd,(long)(0-xOffset),(long)(cxiSign.GetHeight()-cxi.GetHeight()+yOffset),true);
						}
						DeleteObject(hCurBitmap);
						hCurBitmap = cxi.MakeBitmap();
						Success = TRUE;
						//cxi.Save("d:\\XXX.jpg",CXIMAGE_FORMAT_JPG);
						cxi.Destroy();
					}
				}
				cxiSign.Destroy();
			}
		}

		if(Success){
			//保存
			if(m_ITransHeader.hwndFrom){
				if(Save(m_ITransHeader.hwndFrom)){
				}
			}
			BuildOutward();
		}
	}
}
/*
BOOL CCViewProDlg::ExportFile(LPCTSTR sText,LPCTSTR sIP,LPCTSTR sDate,short nPostion)
{
	CString sExportPath = "c:\\temp";
	BOOL Success = FALSE;
	if(Filled()){
		if(IsBitmap()){
			if(hCurBitmap){
				CxImage* cxi = new CxImage();
				cxi->CreateFromHBITMAP(hCurBitmap);
				if(cxi->IsValid()){

						//hResultBitmap = AddWaterMark(newcxi);
						//newcxi->Destroy();
						//return hResultBitmap;


				}
			}
		}else{
			CreateDirectoryEx(sExportPath);

			CString sFileName;
			sFileName.Format("%s\\temp\\%s-%s",m_CurrentPath,GetGId(),GetIId());

			CString sNewFileName;
			sNewFileName.Format("%s\\%s-%s",sExportPath,GetGId(),GetIId());
			Success = CopyFile(sFileName,sNewFileName,FALSE);
		}
	}

	return Success;
}
*/
/*
void CCViewProDlg::AddImageIdentify(LPCTSTR sSignatureBase64,long xOffset,long yOffset)
{
	BOOL Success = FALSE;
	if(Filled() && IsBitmap()){
		if(strlen(sSignatureBase64)>0){
			Base64 base64;
			std::string sSignatureData = base64.base64_decode((unsigned char*)sSignatureBase64,strlen(sSignatureBase64));
			if(sSignatureData.size()>0){
				CxImage cxiSign;
				cxiSign.Decode((BYTE*)sSignatureData.c_str(),sSignatureData.size(),CXIMAGE_FORMAT_UNKNOWN);
				if(cxiSign.IsValid()){
					if(hCurBitmap){
						CxImage cxi;
						cxi.CreateFromHBITMAP(hCurBitmap);
						if(cxi.IsValid()){
							cxi.Mix(cxiSign,CxImage::OpAnd,(long)(0-xOffset),(long)(cxiSign.GetHeight()-cxi.GetHeight()+yOffset),true);
						}
						DeleteObject(hCurBitmap);
						hCurBitmap = cxi.MakeBitmap();
						Success = TRUE;
						//cxi.Save("d:\\XXX.jpg",CXIMAGE_FORMAT_JPG);
						cxi.Destroy();
					}
				}
				cxiSign.Destroy();
			}
		}

		if(Success){
			BuildOutward();
		}
	}
}*/

CString		CCViewProDlg::GetSelRectInfo()
{
	CString sResult = "";
	if(Filled() && IsBitmap()){
		if(hCurBitmap==NULL)
			return sResult;

		double dScale = m_ZoomScale*m_dShrink;
		CxImage* cxi = new CxImage();
		if(m_Picture.GetMode()==RECT_MODE){
			CRect rect;
			if(m_Picture.GetSelRect(&rect)){
				cxi->CreateFromHBITMAP(hCurBitmap);
				if(cxi->IsValid()){
					//if(m_RotateAngle!=0.0){
					//	cxi->Rotate2(m_RotateAngle,NULL,CxImage::IM_BILINEAR,CxImage::OM_TRANSPARENT,0,false,false);
					//}
					if(dScale!=1.0){
						long dw = long(rect.Width()/dScale);
						long dh = long(rect.Height()/dScale);
						rect.left = long(rect.left/dScale);
						rect.right = rect.left + dw;
						rect.top = long(rect.top/dScale);
						rect.bottom = rect.top + dh;
					}

					sResult.Format("%f,%d,%d,%d,%d",m_RotateAngle,rect.left,rect.right,rect.top,rect.bottom);				
				}
			}
		}
		cxi->Destroy();
	}
	return sResult;
}
