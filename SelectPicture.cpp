// SelectPicture.cpp : implementation file
//

#include "stdafx.h"
#include "SelectPicture.h"
#include "CViewPro.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" WINUSERAPI BOOL WINAPI TrackMouseEvent (LPTRACKMOUSEEVENT lpEventTrack);

/////////////////////////////////////////////////////////////////////////////
// CSelectPicture

CSelectPicture::CSelectPicture()
{	
	m_hFocusWnd = 0;
	handCursor = LoadCursor(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_MOVECURSOR));
	arrowCursor = LoadCursor(NULL,IDC_ARROW);
	crossCursor = LoadCursor(NULL,IDC_CROSS);
	sizeAllCursor = LoadCursor(NULL,IDC_SIZEALL);
	curCursor = arrowCursor;
	m_Notify_hWnd = 0;
	m_hBitmap = NULL;
	m_MouseTracking = FALSE;
	m_Draging = FALSE;
	m_MousePoint.x =- 9999;
	m_MousePoint.y =- 9999;
}

CSelectPicture::~CSelectPicture()
{
	if(m_hBitmap)
	{
		DeleteObject(m_hBitmap);
	}
	if(handCursor){
		DeleteObject(handCursor);
	}

	m_AllLightRect.erase(m_AllLightRect.begin(),m_AllLightRect.end());
}


BEGIN_MESSAGE_MAP(CSelectPicture, CStatic)
	//{{AFX_MSG_MAP(CSelectPicture)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		ON_WM_PAINT()
		ON_WM_MOUSEWHEEL()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_SETCURSOR()
		//ON_WM_VSCROLL()
		//ON_WM_HSCROLL()
		ON_WM_TIMER()
		ON_WM_ERASEBKGND()
		ON_WM_CTLCOLOR_REFLECT()
		ON_WM_LBUTTONDBLCLK()
		ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
		ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


BOOL CSelectPicture::Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID)
{
	return CStatic::Create(lpszCaption, dwStyle, rect, pParentWnd, nID);
}

void CSelectPicture::PreSubclassWindow() 
{
    DWORD dwStyle = GetStyle();
	dwStyle |= SS_NOTIFY;
	dwStyle |= SS_BITMAP;
	//dwStyle |= BS_OWNERDRAW;
	dwStyle |= SS_OWNERDRAW;

	
    ::SetWindowLong(GetSafeHwnd(), GWL_STYLE, dwStyle);

	m_tracker.m_rect = CRect(0,0,0,0);
	m_tracker.m_nStyle = 0;
	m_tracker.m_nStyle |= CRectTracker::dottedLine;
	//m_tracker.m_nStyle |= CRectTracker::resizeOutside;
	m_tracker.m_nStyle |= CRectTracker::resizeInside;


	m_vScrollPos = 0;
	m_hScrollPos = 0;

	m_bitmapX0 = 0;
	m_bitmapY0 = 0;

	m_bitmapX01 = 0;
	m_bitmapY01 = 0;


	m_trackerRect = CRect(0,0,0,0);
	m_BgErase = false;

	CClientDC dc(this);
	m_dcMem.CreateCompatibleDC(&dc);

	m_switchPointCount = 0;
	m_Mode = ROLL_MODE;
	m_Align = 0;
	m_Border = 0;
	m_HoverBorder = 0;
	m_Hover = 0;
	m_LineStyle = BS_FLAT;
	m_BackgroundColor = GetSysColor(COLOR_3DFACE);
  	// set disabled box colors
	m_boxPrimaryColor_disabled = ::GetSysColor(COLOR_3DSHADOW);
	m_boxHighlightColor_disabled = ::GetSysColor(COLOR_3DHILIGHT);

	// set default box colors
	m_boxPrimaryColor = m_boxPrimaryColor_disabled;
	m_boxHighlightColor = m_boxHighlightColor_disabled;

	m_boxHoverPrimaryColor = m_boxPrimaryColor_disabled;
	m_boxHoverHighlightColor = m_boxHighlightColor_disabled;

	CStatic::PreSubclassWindow();
}

void CALLBACK LineDDAProc(int x, int y, CSelectPicture* hPic)
{
	CDC* pDC = hPic->GetDC();
	if(pDC){
		hPic->m_switchPointCount = hPic->m_switchPointCount%(2*SWITCHCOUNT);
		if(hPic->m_switchPointCount < SWITCHCOUNT)
			pDC->SetPixelV(x,y,RGB(255,255,255));
		else
			pDC->SetPixelV(x,y,RGB(0,0,0));
		hPic->m_switchPointCount++;
	}
	hPic->ReleaseDC(pDC);
}

void CSelectPicture::SetNotify(HWND hWnd){
	m_Notify_hWnd = hWnd;
}

void CSelectPicture::DrawSelection()
{
	if(m_Mode==POLYGON_MODE && m_hBitmap){
		int x = m_bitmapX0-m_bitmapX01;
		int y = m_bitmapY0-m_bitmapY01;

		CRect rect;
		GetClientRect(&rect);
		HRGN rc1 = CreateRectRgn(m_bitmapX0,m_bitmapY0,m_bitmapX0+m_bmInfo.bmWidth,m_bitmapY0+m_bmInfo.bmHeight);
		HRGN rc2 = CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
		CombineRgn(rc1,rc1,rc2,RGN_AND);
		GetRgnBox(rc1,&rect);

		DeleteObject(rc1);
		DeleteObject(rc2);


		for(int i=1;i<m_SelectPointCount;i++){		
			if(m_SelectPoint[i-1].x+x>=rect.left && m_SelectPoint[i-1].x+x<=rect.right && m_SelectPoint[i].x+x>=rect.left && m_SelectPoint[i].x+x<=rect.right){
				if(m_SelectPoint[i-1].y+y>=rect.top && m_SelectPoint[i-1].y+y<=rect.bottom && m_SelectPoint[i].y+y>=rect.top && m_SelectPoint[i].y+y<=rect.bottom){
					LineDDA((int)(m_SelectPoint[i-1].x+x),
						(int)(m_SelectPoint[i-1].y+y),
						(int)(m_SelectPoint[i].x+x),
						(int)(m_SelectPoint[i].y+y),
						(LINEDDAPROC)LineDDAProc,(LPARAM)this);
				}
			}
		}
	}
}

void CSelectPicture::DrawTracker(CDC* dc)
{
	if(m_trackerRect.Width()>0 && m_trackerRect.Height()>0){	
		m_tracker.m_rect = m_trackerRect;
		m_tracker.m_rect.left+=(m_bitmapX0-m_bitmapX01);
		m_tracker.m_rect.right+=(m_bitmapX0-m_bitmapX01);
		m_tracker.m_rect.top+=(m_bitmapY0-m_bitmapY01);
		m_tracker.m_rect.bottom+=(m_bitmapY0-m_bitmapY01);

		if(m_tracker.m_rect.left<0)
			m_tracker.m_rect.left = 0;
		if(m_tracker.m_rect.right<0)
			m_tracker.m_rect.right = 0;
		if(m_tracker.m_rect.top<0)
			m_tracker.m_rect.top = 0;
		if(m_tracker.m_rect.bottom<0)
			m_tracker.m_rect.bottom = 0;

		if(m_tracker.m_rect.Width()>0 && m_tracker.m_rect.Height()>0){
			m_tracker.Draw(dc);
		}
	}
}

BOOL CSelectPicture::IsDrag()
{
	return m_Draging;
}

void CSelectPicture::OnPaint() 
{
	PAINTSTRUCT ps;
    CDC* dc = BeginPaint(&ps);

	//CPaintDC dc(this);

	CRect rect;
	GetClientRect(&rect);

	DWORD dwStyle = GetStyle();
	DWORD dwExStyle = GetExStyle();

	if(m_Border || (m_HoverBorder && m_Hover)){
		//if(m_LineStyle==BS_FLAT){
			GetClientRect(&rect);
			rect.InflateRect(GetSystemMetrics(SM_CXEDGE)-1,GetSystemMetrics(SM_CYEDGE)-1,GetSystemMetrics(SM_CXEDGE)-1,GetSystemMetrics(SM_CYEDGE)-1);
			//rect.InflateRect(GetSystemMetrics(SM_CXEDGE),GetSystemMetrics(SM_CYEDGE),GetSystemMetrics(SM_CXEDGE),GetSystemMetrics(SM_CYEDGE));
			COLORREF brushColor = m_boxPrimaryColor;
			if(m_HoverBorder && m_Hover){
				brushColor = m_boxHoverPrimaryColor;
			}
			CBrush brush (brushColor);
			dc->FrameRect(rect,&brush);
			DeleteObject(brush);
		/*
		}else{
			if(dwExStyle & WS_EX_DLGMODALFRAME){
				rect.InflateRect(GetSystemMetrics(SM_CXEDGE),GetSystemMetrics(SM_CYEDGE),GetSystemMetrics(SM_CXEDGE),GetSystemMetrics(SM_CYEDGE));
				dc.Draw3dRect(&rect,m_boxHighlightColor,m_boxPrimaryColor);
			}else if(dwExStyle & WS_EX_CLIENTEDGE){
				rect.InflateRect(GetSystemMetrics(SM_CXEDGE),GetSystemMetrics(SM_CYEDGE),GetSystemMetrics(SM_CXEDGE),GetSystemMetrics(SM_CYEDGE));
				dc.Draw3dRect(&rect,m_boxPrimaryColor, m_boxHighlightColor);
			}if(dwExStyle & WS_EX_STATICEDGE){
				rect.InflateRect(GetSystemMetrics(SM_CXEDGE)-1,GetSystemMetrics(SM_CYEDGE)-1,GetSystemMetrics(SM_CXEDGE)-1,GetSystemMetrics(SM_CYEDGE)-1);
				dc.Draw3dRect(&rect,m_boxPrimaryColor, m_boxHighlightColor);
			}
		}*/
	}else{
		/*
		GetClientRect(&rect);
		rect.InflateRect(GetSystemMetrics(SM_CXEDGE)-1,GetSystemMetrics(SM_CYEDGE)-1,GetSystemMetrics(SM_CXEDGE)-1,GetSystemMetrics(SM_CYEDGE)-1);
		COLORREF brushColor = m_boxPrimaryColor;
		CBrush brush (brushColor);
		dc->FrameRect(rect,&brush);
		DeleteObject(brush);
		*/
	}
	
	
	GetClientRect(&rect);


		if(m_hBackgroundBitmap){
			CDC dcMemory;
			dcMemory.CreateCompatibleDC(dc);
			CBitmap bmpBg;
			bmpBg.CreateCompatibleBitmap(dc,rect.Width(),rect.Height());
			CBitmap* pOldBitmap = dcMemory.SelectObject(&bmpBg);
			/*
			//dcMem.FillSolidRect(&rect, RGB(0,0,0));
			HBRUSH hBrush = ::CreateSolidBrush(RGB(255,255,255));
			CBrush br;
			br.Attach(hBrush);
			dcMemory.FillRect(rect,&br);
			br.Detach();
			*/

			
			CBitmap pBrushBimap;
			pBrushBimap.Attach(m_hBackgroundBitmap);
			CBrush brush;
			brush.CreatePatternBrush(&pBrushBimap);
			dcMemory.FillRect(&rect,&brush);
			brush.DeleteObject();
			pBrushBimap.Detach();
			pBrushBimap.DeleteObject();
			//brush.Attach(
			
			///if(pBackgroundBitmap){
/*
				BITMAP bmpInfo;  
				GetObject(m_hBackgroundBitmap , sizeof(BITMAP), &bmpInfo);
				CDC dcDraw;
				dcDraw.CreateCompatibleDC(dc);
				HBITMAP oldDrawBmp = (HBITMAP)SelectObject(dcDraw,m_hBackgroundBitmap);

				//dcMemory.StretchBlt(0,0,rect.Width(),rect.Height(),&dcDraw,0,0,bmpInfo.bmWidth,bmpInfo.bmHeight,SRCCOPY);
				//dcDraw.SelectObject(oldDrawBmp);
				SelectObject(dcDraw,oldDrawBmp);
				*/

				if(m_hBitmap && m_dcMem){

					if(m_Align==0){
						dcMemory.BitBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,m_hScrollPos,m_vScrollPos,SRCCOPY);
						m_bitmapX0 = -1*m_hScrollPos;
						m_bitmapY0 = -1*m_vScrollPos;
					}else{
						int left = (rect.Width()-m_bmInfo.bmWidth)/2>0?(rect.Width()-m_bmInfo.bmWidth)/2:0;
						int top = (rect.Height()-m_bmInfo.bmHeight)/2>0?(rect.Height()-m_bmInfo.bmHeight)/2:0;
						dcMemory.BitBlt(left,top,rect.Width(),rect.Height(),&m_dcMem,m_hScrollPos,m_vScrollPos,SRCCOPY);
						m_bitmapX0 = left + (-1)*m_hScrollPos;
						m_bitmapY0 = top + (-1)*m_vScrollPos;
					}
				}
				dc->BitBlt(0,0,rect.Width(),rect.Height(),&dcMemory,0,0,SRCCOPY);
				dcMemory.SelectObject(&pOldBitmap);
				bmpBg.DeleteObject();
				dcMemory.DeleteDC();
		}else{
			if(m_hBitmap && m_dcMem){
				if(m_Align==0){
					dc->BitBlt(0,0,rect.Width(),rect.Height(),&m_dcMem,m_hScrollPos,m_vScrollPos,SRCCOPY);
					m_bitmapX0 = -1*m_hScrollPos;
					m_bitmapY0 = -1*m_vScrollPos;
				}else{
					int left = (rect.Width()-m_bmInfo.bmWidth)/2>0?(rect.Width()-m_bmInfo.bmWidth)/2:0;
					int top = (rect.Height()-m_bmInfo.bmHeight)/2>0?(rect.Height()-m_bmInfo.bmHeight)/2:0;
					dc->BitBlt(left,top,rect.Width(),rect.Height(),&m_dcMem,m_hScrollPos,m_vScrollPos,SRCCOPY);
					m_bitmapX0 = left + (-1)*m_hScrollPos;
					m_bitmapY0 = top + (-1)*m_vScrollPos;
				}
			}
		}
	
	DrawSelection();
	DrawTracker(dc);
	DrawCreateRect(dc);
	
	/* debug error
	dc.DeleteDC();
	dc.ReleaseAttribDC();
	dc.ReleaseOutputDC();
	*/
	EndPaint(&ps);
}
bool CSelectPicture::AdjustTrackerRect(){
	bool adjustFlag = FALSE;
	if(m_hBitmap && (m_tracker.m_rect.Width()>0) && (m_tracker.m_rect.Height()>0)){
		CRect rect;
		GetClientRect(&rect);
		HRGN rc1 = CreateRectRgn(m_bitmapX0,m_bitmapY0,m_bitmapX0+m_bmInfo.bmWidth,m_bitmapY0+m_bmInfo.bmHeight);
		HRGN rc2 = CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
		HRGN rc3 = CreateRectRgn(m_tracker.m_rect.left,m_tracker.m_rect.top,m_tracker.m_rect.right,m_tracker.m_rect.bottom);
		CombineRgn(rc1,rc1,rc2,RGN_AND);
		CombineRgn(rc1,rc1,rc3,RGN_AND);

		GetRgnBox(rc1,&rect);
		if(!rect.EqualRect(m_tracker.m_rect)){	
			GetRgnBox(rc1,&m_tracker.m_rect);
			adjustFlag = TRUE;
		}
		DeleteObject(rc1);
		DeleteObject(rc2);
		DeleteObject(rc3);
	}else{
		CRect rect = CRect(0,0,0,0);
		if(!rect.EqualRect(m_tracker.m_rect)){
			adjustFlag = TRUE;
			m_tracker.m_rect = CRect(0,0,0,0);
		}
	}
	m_bitmapX01 = m_bitmapX0;
	m_bitmapY01 = m_bitmapY0;
	return adjustFlag;
}
BOOL CSelectPicture::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	HWND hParent = ::GetParent(m_hWnd);
	if(hParent){
		::PostMessage(hParent,WM_MOUSEWHEEL_NOTIFY,(WPARAM)zDelta,0);
	}
/*
	if(zDelta>0){		
		if(m_Notify_hWnd){
			::PostMessage(m_Notify_hWnd,PNM_MOUSEWHEEL,WPARAM(zDelta),0);
		}
	}else{
	
	}
	*/
	return TRUE;
}
LRESULT CSelectPicture::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	POINT pt;
	GetCursorPos(&pt);
	CRect rect;
	GetWindowRect(&rect);
	CPoint point;
	point.x = pt.x;
	point.y = pt.y;

	if(!rect.PtInRect(point)){//OnMouseHover之后回调IE,如果JS在OnMouseHover回调函数中弹出的层距离鼠标太近,即便鼠标没有移出窗口,也会触发OnMouseLeave,因此在此要判断鼠标是否真的离开窗口区域
	
		
		if(m_hWnd == ::GetFocus()){
			if((m_hFocusWnd!=m_hWnd) && ::IsWindow(m_hFocusWnd)){
				::SetFocus(m_hFocusWnd);//使之前拥有焦点的窗体重新获得焦点
				m_hFocusWnd = 0;
			}
		}
	
		if(m_Hover==1){
			m_Hover = 0;
			//::SendMessage(GetParent()->m_hWnd,WM_REDRAW,(LPARAM)m_hWnd,0);
			GetParent()->Invalidate();
		}

		if(m_Draging==FALSE){
			Event(PNM_MOUSELEAVE);
		}
	}

	m_MouseTracking=FALSE;
	return TRUE;
}

LRESULT CSelectPicture::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if(m_Draging==FALSE){
		Event(PNM_MOUSEHOVER);
	}
	m_MouseTracking=FALSE;
	return TRUE;
}

void CSelectPicture::OnMouseMove(UINT nFlags, CPoint point)
{
	if(point.x==m_MousePoint.x && point.y==m_MousePoint.y){
		return;
	}else{
		m_MousePoint.x = point.x;
		m_MousePoint.y = point.y;
	}
	CRect rect;
	GetClientRect(&rect);
	if(rect.PtInRect(point))
	{
		HWND hFocus = ::GetFocus();
		if(m_hWnd != hFocus){
			m_hFocusWnd = hFocus;
			SetFocus();//获取焦点
		}
		Event(PNM_MOUSEMOVE);
		
		if(m_Hover==0){
			if(m_HoverBorder){
				m_Hover = 1;
				//::SendMessage(GetParent()->m_hWnd,WM_REDRAW,(LPARAM)m_hWnd,0);
				GetParent()->Invalidate();
			}
		}
	 
		if(!m_MouseTracking){
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(tme);
				tme.hwndTrack = m_hWnd;
				tme.dwFlags = TME_HOVER|TME_LEAVE;
				tme.dwHoverTime = 1500;
				if(TrackMouseEvent(&tme)){
					m_MouseTracking = TRUE;
				}
		}

		long x = point.x;
		long y = point.y;

		if(m_Mode==ROLL_MODE){
			if((nFlags & MK_LBUTTON) && m_hBitmap){
				//Event(PNM_DRAG);
				if(m_Rolling){
					int cx = (x-m_RollStartPoint.x);
					int cy = (y-m_RollStartPoint.y);
					if(abs(cx)>0 || abs(cy)>0){
						m_RollStartPoint.x = x;
						m_RollStartPoint.y = y;
						

						BOOL Roll = FALSE;
						if(GetHScrollMax()>0){
							long hCurrentPos = m_hScrollPos;
							long hWillPos = m_hScrollPos - cx;
							if(hWillPos > GetHScrollMax()){
								hWillPos = GetHScrollMax();
							}
							if(hWillPos < 0){
								hWillPos = 0;
							}
							if(hWillPos!=hCurrentPos){
								m_hScrollPos = hWillPos;
								Roll = TRUE;
							}
						}

						if(GetVScrollMax()>0){
							long vCurrentPos = m_vScrollPos;
							long vWillPos = m_vScrollPos - cy;
							if(vWillPos > GetVScrollMax()){
								vWillPos = GetVScrollMax();
							}
							if(vWillPos < 0){
								vWillPos = 0;
							}
							if(vWillPos!=vCurrentPos){
								m_vScrollPos = vWillPos;
								Roll = TRUE;
							}
						}

						if(Roll){
							CWnd* pParent = GetParent();
							if(pParent){
								::PostMessage(pParent->m_hWnd,WM_CHANGE_SCROLLPOS,cx,cy);
							}
							Invalidate(true);
						}
					}
				}
			}
		}else if(m_Mode==DRAG_MODE){
			if((nFlags & MK_LBUTTON) && m_hBitmap){
				Event(PNM_DRAG);
			}
		}else if(m_Mode==POLYGON_MODE){
			if((nFlags & MK_LBUTTON) && m_hBitmap){
				if(curCursor != crossCursor){
					curCursor = crossCursor;
					::SetCursor(curCursor);
				}

				CRect rect;
				GetClientRect(&rect);
				HRGN rc1 = CreateRectRgn(m_bitmapX0,m_bitmapY0,m_bitmapX0+m_bmInfo.bmWidth,m_bitmapY0+m_bmInfo.bmHeight);
				HRGN rc2 = CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
				CombineRgn(rc1,rc1,rc2,RGN_AND);
				GetRgnBox(rc1,&rect);
				if(point.x>=rect.left && point.x<=rect.right){
					if(point.y>=rect.top && point.y<=rect.bottom){
						if(m_SelectPointCount==0){
							m_bitmapX01 = m_bitmapX0;
							m_bitmapY01 = m_bitmapY0;
						}
						if(m_SelectPointCount<MAX_SELECT_POINTS-1){
							m_SelectPoint[m_SelectPointCount].x = x;
							m_SelectPoint[m_SelectPointCount].y = y;
							m_SelectPointCount++;
						}else{
							m_SelectPoint[m_SelectPointCount].x = m_SelectPoint[0].x;
							m_SelectPoint[m_SelectPointCount].y = m_SelectPoint[0].y;
							m_SelectPointCount++;
						}
						Invalidate(false);
					}
				}
				DeleteObject(rc1);
				DeleteObject(rc2);
			}
		}else{
			//curCursor = crossCursor;
			/*
			if(nFlags & MK_LBUTTON){
				if(m_SelectPointCount==0){
					m_SelectPoint[0].x = x;
					m_SelectPoint[0].y = y;
					m_SelectPointCount = 1;
				}else{
					m_SelectPoint[1].x = x;
					m_SelectPoint[1].y = m_SelectPoint[0].y;
					m_SelectPoint[2].x = x;
					m_SelectPoint[2].y = y;
					m_SelectPoint[3].x = m_SelectPoint[0].x;
					m_SelectPoint[3].y = y;
					m_SelectPoint[4].x = m_SelectPoint[0].x;
					m_SelectPoint[4].y = m_SelectPoint[0].y;
					m_SelectPointCount = 5;
					Invalidate(false);
				}
			}*/
		}
	}
	CStatic::OnMouseMove(nFlags, point);

}
void CSelectPicture::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_SelectPointCount = 0;
	if(m_Mode==ROLL_MODE){
		if(curCursor != handCursor){
			curCursor = handCursor;
			::SetCursor(curCursor);
		}
		::GetCursorPos(&m_RollStartPoint);
		ScreenToClient(&m_RollStartPoint);  
		m_Rolling = TRUE;
		if(m_Notify_hWnd){
			SetCapture();
			Event(PNM_DRAGSTART);
		}
	}else if(m_Mode==DRAG_MODE){
		if(curCursor != sizeAllCursor){
			curCursor = sizeAllCursor;
			::SetCursor(curCursor);
		}
		m_Draging = TRUE;
		if(m_Notify_hWnd){
			SetCapture();
			Event(PNM_DRAGSTART);
		}
	}else if(m_Mode==POLYGON_MODE){//不规则区域
		KillTimer(1);
	}else{
		if(m_hBitmap){
			bool erase = m_BgErase;
			if(m_tracker.HitTest(point) < 0){//point不在tracker的矩形区域内（按下鼠标跟踪）
				CRectTracker track;
				if(track.TrackRubberBand(this, point, true)) {
					track.m_rect.NormalizeRect();
					m_tracker.m_rect = track.m_rect;
					m_BgErase = AdjustTrackerRect();
				}else{
					m_tracker.m_rect = CRect(0,0,0,0);
					m_BgErase = false;
				}
			}else{//point在tracker的矩形区域内（按下鼠标移动）
				if(m_tracker.Track(this, point, true)){
					m_tracker.m_rect.NormalizeRect();
					m_BgErase = AdjustTrackerRect();
				}else{
					m_tracker.m_rect = CRect(0,0,0,0);
					m_BgErase = false;
				}
			}
			m_trackerRect = m_tracker.m_rect;
			Invalidate(erase);//确保tracker边框部分在图像外部时，图像部分重绘时能完全擦掉边框
		}
	}
}
void CSelectPicture::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//m_tracker在LButton中屏蔽了OnLButtonUp的调用，因此m_selMode==RECT_MODE时将不能执行该方法
	if(m_Mode==ROLL_MODE){
		if(curCursor != arrowCursor){
			curCursor = arrowCursor;
			::SetCursor(curCursor);
		}
		if(m_Notify_hWnd){
			ReleaseCapture();
			Event(PNM_DRAGEND);
		}
		m_Rolling = FALSE;
	}else if(m_Mode==DRAG_MODE){
		if(curCursor != arrowCursor){
			curCursor = arrowCursor;
			::SetCursor(curCursor);
		}
		if(m_Notify_hWnd){
			ReleaseCapture();
			Event(PNM_DRAGEND);
		}
		m_Draging = FALSE;
		ResetMousePoint();//尽可能产生MouseMove事件,使得能再次获得MouseHover
	}else if(m_Mode==POLYGON_MODE){
		if(curCursor != arrowCursor){
			curCursor = arrowCursor;
			::SetCursor(curCursor);
		}
		if(m_SelectPointCount<MAX_SELECT_POINTS-1){
			m_SelectPoint[m_SelectPointCount].x = m_SelectPoint[0].x;
			m_SelectPoint[m_SelectPointCount].y = m_SelectPoint[0].y;
			m_SelectPointCount++;
		}
		m_switchPointCount = 0;

		Invalidate(false);
		if(m_SelectPointCount>0){
			SetTimer(1,300,NULL);
		}
	}
	CStatic::OnLButtonUp(nFlags, point);
}

void CSelectPicture::OnTimer(UINT nIDEvent){
	if(nIDEvent == 1){
		long m = m_switchPointCount%(2*SWITCHCOUNT);
		DrawSelection();
		if(m==(m_switchPointCount%(2*SWITCHCOUNT)))
			m_switchPointCount++;
	}
	CStatic::OnTimer(nIDEvent);
}

BOOL CSelectPicture::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_tracker.SetCursor(pWnd, nHitTest)) return TRUE;

	::SetCursor(curCursor);
	return TRUE;
	
	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}


short CSelectPicture::GetAlign(){
	return m_Align;
}
void CSelectPicture::SetAlign(int align){
	m_Align = align;
	Invalidate();
}

void CSelectPicture::SetMode(int mode){
	if(m_Mode!=mode){
		if(curCursor != arrowCursor){
			curCursor = arrowCursor;
			::SetCursor(curCursor);
		}

		m_tracker.m_rect = CRect(0,0,0,0);
		m_trackerRect = m_tracker.m_rect;
		m_bitmapX01 = 0;
		m_bitmapY01 = 0;

		m_SelectPointCount = 0;
		m_switchPointCount = 0;
		KillTimer(1);
		bool erase = m_BgErase;
		Invalidate(erase);
		m_BgErase = false;
		m_Mode = mode;
	}
}

short CSelectPicture::GetMode(){
	return m_Mode;
}


short CSelectPicture::GetBorder(){
	return m_Border || (m_HoverBorder && m_Hover);
}

void CSelectPicture::SetBorder(short border){
	m_Border = border;
//	Invalidate();//边线和边线颜色与父窗口对的背景擦除相关，因此由父窗口调用Invalidate，待父窗口完成背景擦除后再进行重绘
}
short CSelectPicture::GetHoverBorder(){
	return m_HoverBorder;
}

void CSelectPicture::SetHoverBorder(short border){
	m_HoverBorder = border;
}
void CSelectPicture::SetBorderColor(COLORREF primaryColor,COLORREF highlightColor){
	m_boxPrimaryColor = primaryColor;
	m_boxHighlightColor = highlightColor;
//	Invalidate();
}
void CSelectPicture::SetHoverBorderColor(COLORREF primaryColor,COLORREF highlightColor){
	m_boxHoverPrimaryColor = primaryColor;
	m_boxHoverHighlightColor = highlightColor;
//	Invalidate();
}
void CSelectPicture::SetBackgroundColor(COLORREF color){
	m_BackgroundColor = color;
	Invalidate();
}

void CSelectPicture::SetBitmap(HBITMAP hBmp){
	m_AllLightRect.erase(m_AllLightRect.begin(),m_AllLightRect.end());
	m_tracker.m_rect = CRect(0,0,0,0);
	m_trackerRect = m_tracker.m_rect;
	m_SelectPointCount = 0;
	m_switchPointCount = 0;
	KillTimer(1);
	/*
	if(m_hBitmap)
	{
		DeleteObject(m_hBitmap);
	}*/

	m_hBitmap = hBmp;
	if(m_hBitmap){
		GetObject(m_hBitmap , sizeof(BITMAP), &m_bmInfo);
		(HBITMAP)SelectObject(m_dcMem, m_hBitmap);
	}else{
		memset(&m_bmInfo,0,sizeof(BITMAP));
	}
	
	m_vScrollPos = 0;
	m_hScrollPos = 0;

	m_bitmapX0 = 0;
	m_bitmapY0 = 0;

	m_bitmapX01 = 0;
	m_bitmapY01 = 0;

	m_BgErase = false;
	Invalidate();
}
void CSelectPicture::SetBackgroundBitmap(HBITMAP hBmp)
{
	if(m_hBackgroundBitmap){
		DeleteObject(m_hBackgroundBitmap);
	}
	m_hBackgroundBitmap = hBmp;
}
HBITMAP CSelectPicture::GetBitmap(){
	return m_hBitmap;
}

void CSelectPicture::Adjust(){
	/*
	m_tracker.m_rect = CRect(0,0,0,0);
	m_SelectPointCount = 0;
	m_switchPointCount = 0;
	KillTimer(1);
	}*/

	m_vScrollPos = 0;
	m_hScrollPos = 0;

	m_bitmapX0 = 0;
	m_bitmapY0 = 0;
	CRect rect;
	GetClientRect(&rect);
	Invalidate();
}

BOOL CSelectPicture::VScroll(){

	if(m_hBitmap!=NULL){
		CRect rect;
		GetClientRect(&rect);
		if(m_bmInfo.bmHeight>rect.Height()){
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CSelectPicture::HScroll(){
	if(m_hBitmap!=NULL){
		CRect rect;
		GetClientRect(&rect);
		if(m_bmInfo.bmWidth>rect.Width()){
			return TRUE;
		}
	}
	return FALSE;
}

long CSelectPicture::GetVScrollMax()
{
	CRect rect;
	GetClientRect(&rect);
	if(m_bmInfo.bmHeight>rect.Height()){
		return m_bmInfo.bmHeight-rect.Height();
	}
	return 0;
}
long CSelectPicture::GetHScrollMax()
{
	CRect rect;
	GetClientRect(&rect);
	if(m_bmInfo.bmWidth>rect.Height()){
		return m_bmInfo.bmWidth-rect.Width();
	}
	return 0;
}


long CSelectPicture::GetVScrollPos()
{
	return m_vScrollPos;
}
long CSelectPicture::GetHScrollPos()
{
	return m_hScrollPos;
}

void CSelectPicture::SetVScrollPos(int nPos)
{
	m_vScrollPos = nPos;
}
void CSelectPicture::SetHScrollPos(int nPos)
{
	m_hScrollPos = nPos;
}

//水平方向
void CSelectPicture::OnHScroll(UINT nSBCode, UINT nPos)
{
	switch (nSBCode)
	{
		case SB_TOP:
			m_hScrollPos = 0;
			break;
		case SB_BOTTOM:
			m_hScrollPos = GetHScrollMax();
			break;
		case SB_THUMBTRACK:
			m_hScrollPos = nPos;
			break;
		case SB_LINEUP:
			if (m_hScrollPos > 0)
				m_hScrollPos-=4;
			if(m_hScrollPos<0)
				m_hScrollPos = 0;

			break;
		case SB_LINEDOWN:
			if (m_hScrollPos < GetHScrollMax())
				m_hScrollPos+=4;
			if(m_hScrollPos>GetHScrollMax())
				m_hScrollPos = GetHScrollMax();
			break;
	}
	Invalidate(false);
}

void CSelectPicture::OnVScroll(UINT nSBCode, UINT nPos)
{
	switch (nSBCode)
	{
		case SB_TOP:
			m_vScrollPos = 0;
			break;
		case SB_BOTTOM:
			m_vScrollPos = GetVScrollMax();
			break;
		case SB_THUMBTRACK:
			m_vScrollPos = nPos;
			break;
		case SB_LINEUP:
			if (m_vScrollPos > 0)
				m_vScrollPos-=4;
			if(m_vScrollPos<0)
				m_vScrollPos = 0;
			break;
		case SB_LINEDOWN:
			if (m_vScrollPos < GetVScrollMax())
				m_vScrollPos+=4;
			if(m_vScrollPos>GetVScrollMax())
				m_vScrollPos = GetVScrollMax();
			break;
	}
	Invalidate(false);
}

bool CSelectPicture::GetSelRect(LPRECT lpRect)
{
	if(lpRect!=NULL && (m_Mode==RECT_MODE)){
		if(m_trackerRect && m_trackerRect.Width()>0 && m_trackerRect.Height()>0){
			lpRect->left = m_trackerRect.left-m_bitmapX01;
			lpRect->right = m_trackerRect.right-m_bitmapX01;
			lpRect->top = m_trackerRect.top-m_bitmapY01;
			lpRect->bottom = m_trackerRect.bottom-m_bitmapY01;
			return true;
		}
	}
	return false;
}

int CSelectPicture::GetSelPolygonPonitNum()
{
	return m_SelectPointCount;
}
bool CSelectPicture::GetSelPolygon(POINT *points,int npoints)
{
	if((m_Mode==POLYGON_MODE) && m_SelectPointCount>0 && npoints==m_SelectPointCount){
		for(int i=0;i<m_SelectPointCount;i++){
			points[i].x = m_SelectPoint[i].x-m_bitmapX01;
			points[i].y = m_SelectPoint[i].y-m_bitmapY01;
		}
		return true;
	}
	return false;
}

void CSelectPicture::ResetMousePoint()
{
	m_MousePoint.x = -9999;
	m_MousePoint.y = -9999;
}

void CSelectPicture::Event(DWORD event)
{
	if(m_Notify_hWnd){
		POINT pt;
		GetCursorPos(&pt);
		PNM_NOTIFY_INFO pni;
		pni.code = event;
		pni.hWnd = (DWORD)m_hWnd;
		pni.id = GetDlgCtrlID();
		pni.x = pt.x;
		pni.y = pt.y;
		CWnd* pParent = GetParent();
		::SendMessage(m_Notify_hWnd,event,(LPARAM)m_hWnd,(LPARAM)&pni);
	}	
}

BOOL CSelectPicture::OnEraseBkgnd(CDC* pDC)
{
	//OutputDebugString("OnEraseBkgnd");
	CRect rect;
	GetClientRect(&rect);
	if(m_hBitmap){

		if(m_Align==0){
			m_bitmapX0 = -1*m_hScrollPos;
			m_bitmapY0 = -1*m_vScrollPos;
		}else{
			int left = (rect.Width()-m_bmInfo.bmWidth)/2>0?(rect.Width()-m_bmInfo.bmWidth)/2:0;
			int top = (rect.Height()-m_bmInfo.bmHeight)/2>0?(rect.Height()-m_bmInfo.bmHeight)/2:0;
			m_bitmapX0 = left + (-1)*m_hScrollPos;
			m_bitmapY0 = top + (-1)*m_vScrollPos;
		}


		CRect rect;
		GetClientRect(&rect);
		
		HRGN rc1 = CreateRectRgn(m_bitmapX0,m_bitmapY0,m_bitmapX0+m_bmInfo.bmWidth,m_bitmapY0+m_bmInfo.bmHeight);
		HRGN rc2 = CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
		HRGN rc3 = CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
		CombineRgn(rc1,rc1,rc2,RGN_AND);
		CombineRgn(rc1,rc1,rc3,RGN_XOR);

		CRgn rc;
		rc.Attach(rc1);
		CBrush br;
		br.CreateSolidBrush(m_BackgroundColor);
		pDC->FillRgn(&rc,&br);

		br.DeleteObject();
		rc.DeleteObject();

		DeleteObject(rc1);
		DeleteObject(rc2);
		DeleteObject(rc3);
	}else{
		pDC->FillSolidRect(&rect,m_BackgroundColor);
	}

	return true;


//	if(erase)
//		return false;
		//return CStatic::OnEraseBkgnd(pDC);
//	else
//		return CDialog::OnEraseBkgnd(pDC);
}

HBRUSH CSelectPicture::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	GetParent()->Invalidate();
	return NULL;
}
void CSelectPicture::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(m_Notify_hWnd){
		PNM_NOTIFY_INFO pni;
		pni.code = PNM_DBLCLK;
		pni.hWnd = (DWORD)m_hWnd;
		pni.id = GetDlgCtrlID();

		CWnd* pParent = GetParent();
		::SendMessage(m_Notify_hWnd,PNM_DBLCLK,(LPARAM)m_hWnd,(LPARAM)&pni);
	}
}

void CSelectPicture::CreateRect(int left,int top,int right,int bottom,short thickness,COLORREF color)
{
	LIGHT_RECT_INFO rectInfo;
	rectInfo.rect.left = left;
	rectInfo.rect.top = top;
	rectInfo.rect.right = right;
	rectInfo.rect.bottom = bottom;
	rectInfo.thickness = thickness;
	rectInfo.color = color;

	m_AllLightRect.push_back(rectInfo);
	Invalidate();
}

void CSelectPicture::ClearRect()
{
	m_AllLightRect.erase(m_AllLightRect.begin(),m_AllLightRect.end());
	Invalidate();
}


void CSelectPicture::DrawCreateRect(CDC* dc)
{
	if(m_hBitmap && m_AllLightRect.size()>0){
		unsigned int index = 0;
		for(index=0;index<m_AllLightRect.size();index++){
			LIGHT_RECT_INFO rectInfo = m_AllLightRect[index];
			RECT rect = rectInfo.rect;

			CPen darkPen;
			CPen lightPen;
			CPen *pOldPen;
			CBrush darkBrush;
			LOGBRUSH darkLogBrush;

			int PenStyle = PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE;
			darkBrush.CreateSolidBrush(rectInfo.color);
			darkBrush.GetLogBrush(&darkLogBrush);
			darkPen.CreatePen(PenStyle, rectInfo.thickness, &darkLogBrush);
			pOldPen = dc->SelectObject(&darkPen);


			int x = m_bitmapX0-m_bitmapX01;
			int y = m_bitmapY0-m_bitmapY01;

			
			CRect rc;
			GetClientRect(&rc);
			//HRGN rc1 = CreateRectRgn(m_bitmapX0,m_bitmapY0,m_bitmapX0+m_bmInfo.bmWidth,m_bitmapY0+m_bmInfo.bmHeight);
			HRGN rc1 = CreateRectRgn(rect.left+x,rect.top+y,rect.right+x,rect.bottom+y);
			HRGN rc2 = CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);
			CombineRgn(rc1,rc1,rc2,RGN_AND);
			GetRgnBox(rc1,&rect);

			DeleteObject(rc1);
			DeleteObject(rc2);
			

			dc->MoveTo(rect.left,rect.top);
			dc->LineTo(rect.right,rect.top);

			if(rect.right<rc.right)
				dc->LineTo(rect.right,rect.bottom);
			else
				dc->MoveTo(rect.right,rect.bottom);

			
			if(rect.bottom<rc.bottom)
				dc->LineTo(rect.left,rect.bottom);
			else
				dc->MoveTo(rect.left,rect.bottom);

			dc->LineTo(rect.left,rect.top);

			dc->SelectObject(pOldPen);
			DeleteObject(darkPen);
			DeleteObject(darkBrush);
		}
	}
}