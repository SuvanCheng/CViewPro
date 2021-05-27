#if !defined(AFX_SELECTPICTURE_H__162272F8_8855_4376_89E3_F05CC50A9806__INCLUDED_)
#define AFX_SELECTPICTURE_H__162272F8_8855_4376_89E3_F05CC50A9806__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectPicture.h : header file
//
#include <vector>

#define MAX_SELECT_POINTS 8000


#define ROLL_MODE 0
#define DRAG_MODE 1
#define RECT_MODE 2
#define POLYGON_MODE 3

#define SWITCHCOUNT 4

#define BS_RECT   11
#define BS_3D     12
#define BS_NOBORDER 13


/////////////////////////////////////////////////////////////////////////////
// CSelectPicture window

#define PNM_MOUSEMOVE	WM_USER+100
#define PNM_MOUSEHOVER	WM_USER+101
#define PNM_MOUSELEAVE	WM_USER+102
#define PNM_DBLCLK		WM_USER+103
#define PNM_DRAGSTART	WM_USER+104
#define PNM_DRAG		WM_USER+105
#define PNM_DRAGEND		WM_USER+106

#define WM_REDRAW		WM_USER+116
#define WM_CHANGE_SCROLLPOS	WM_USER+117
#define WM_MOUSEWHEEL_NOTIFY	WM_USER+118




struct PNM_NOTIFY_INFO{
		DWORD code;
		DWORD id;
		DWORD hWnd;
		long  x;
		long  y;
    };

struct LIGHT_RECT_INFO{
		RECT rect;
		short thickness;
		COLORREF color; 
    };

class CSelectPicture : public CStatic
{
// Construction
public:
	CSelectPicture();

// Attributes
public:
	BOOL Create(LPCTSTR lpszCaption, DWORD dwStyle, const RECT &rect, CWnd *pParentWnd, UINT nID);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectPicture)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSelectPicture();
	void	SetNotify(HWND hWnd);
	void	DrawSelection();
	void	DrawTracker(CDC* dc);
	short	GetAlign();
	void	SetAlign(int align);
	short	GetMode();
	void	SetMode(int mode);
	short	GetBorder();
	void	SetBorder(short border);
	void	SetBorderColor(COLORREF primaryColor,COLORREF highlightColor);
	short	GetHoverBorder();
	void	SetHoverBorder(short border);
	void	SetHoverBorderColor(COLORREF primaryColor,COLORREF highlightColor);
	void	SetBackgroundColor(COLORREF color);
	void	SetBitmap(HBITMAP hBmp);
	void	SetBackgroundBitmap(HBITMAP hBmp);
	HBITMAP GetBitmap();
	void	Adjust();
	BOOL	VScroll();
	void	OnVScroll(UINT nSBCode, UINT nPos);
	BOOL	HScroll();
	void	OnHScroll(UINT nSBCode, UINT nPos);
	long	GetVScrollMax();
	long	GetHScrollMax();
	long	GetVScrollPos();
	long	GetHScrollPos();
	void	SetVScrollPos(int nPos);
	void	SetHScrollPos(int nPos);
	bool	GetSelRect(LPRECT lpRect);
	int		GetSelPolygonPonitNum();
	bool	GetSelPolygon(POINT *points,int npoints);
	void	ResetMousePoint();
	void	Event(DWORD event);

	BOOL	IsDrag();
	void	DrawCreateRect(CDC* dc);
	void	CreateRect(int left,int top,int right,int bottom,short thickness,COLORREF color);
	void	ClearRect();



	// Generated message map functions
protected:
	//{{AFX_MSG(CSelectPicture)
		// NOTE - the ClassWizard will add and remove member functions here.
		afx_msg void OnPaint();
		afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
		afx_msg LRESULT OnMouseLeave(WPARAM,LPARAM);
		afx_msg LRESULT OnMouseHover(WPARAM,LPARAM);
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
//		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
//		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

		


	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:

	HCURSOR  handCursor;
	HCURSOR  arrowCursor;
	HCURSOR  crossCursor;
	HCURSOR  sizeAllCursor;
	HCURSOR  curCursor;

public:
	int m_switchPointCount;

private:
	bool AdjustTrackerRect();
private:

	CDC m_dcMem;

	HBITMAP m_hBitmap;
	HBITMAP m_hBackgroundBitmap;
	BITMAP m_bmInfo;

	long m_vScrollPos;
	long m_hScrollPos;



	//图像左上角相对客户区的位置
	long m_bitmapX0;
	long m_bitmapY0;


	//tracker建立时图像左上角相对客户区的位置
	long m_bitmapX01;
	long m_bitmapY01;
	
	CRect m_trackerRect;//tracker建立后记录tracker的RECT

	bool m_BgErase;

	CRectTracker m_tracker;
	POINT m_SelectPoint[MAX_SELECT_POINTS];
	long m_SelectPointCount;



private:
	HWND m_hFocusWnd;
	CPoint m_MousePoint;//IE浏览器中有IFRAME时，鼠标不动也会有大量的MOuseMove事件，因此引入m_MousePoint记录上次鼠标位置，来判断鼠标位置是否发生变化
	BOOL m_MouseTracking;
	BOOL m_Rolling;
	CPoint m_RollStartPoint;
	BOOL m_Draging;
	HWND m_Notify_hWnd;
	int m_LineStyle;
	short m_Align;
	short m_Mode;
	short m_Border;
	short m_HoverBorder;
	short m_Hover;
	COLORREF m_BackgroundColor;
	COLORREF m_boxPrimaryColor;
	COLORREF m_boxHighlightColor;
	COLORREF m_boxHoverPrimaryColor;
	COLORREF m_boxHoverHighlightColor;
	COLORREF m_boxPrimaryColor_disabled;
	COLORREF m_boxHighlightColor_disabled;

	std::vector<LIGHT_RECT_INFO> m_AllLightRect;

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTPICTURE_H__162272F8_8855_4376_89E3_F05CC50A9806__INCLUDED_)
