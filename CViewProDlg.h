#if !defined(AFX_CVIEWPRODLG_H__A7634680_83CF_4174_A166_3F8673DEDA3E__INCLUDED_)
#define AFX_CVIEWPRODLG_H__A7634680_83CF_4174_A166_3F8673DEDA3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CViewProDlg.h : header file
//

#include "MD5.h"
#include "SelectPicture.h"
#include "BitmapTrace.h"
#include "Base64.h"


#include "include\CxImage\ximage.h"
#ifdef    _DEBUG   
	#pragma comment(lib,"lib\\CxImage\\Debug\\cximage.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\jasper.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\jbig.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\Jpeg.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\libdcr.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\mng.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\png.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\Tiff.lib")
	#pragma comment(lib,"lib\\CxImage\\Debug\\zlib.lib")
#else   
	#pragma comment(lib,"lib\\CxImage\\Release\\cximage.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\jasper.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\jbig.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\Jpeg.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\libdcr.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\mng.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\png.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\Tiff.lib")
	#pragma comment(lib,"lib\\CxImage\\Release\\zlib.lib")
#endif

/////////////////////////////////////////////////////////////////////////////
// CCViewProDlg dialog
#define CMTS_FAIL					0x0000
#define CMTS_SUCCESS				0xFF01


#define CM_FROMCENTER_IMAGETRANS			0x8000
#define CM_SAVETOCENTER_IMAGETRANS			0x8001
#define CM_REMOVETOCENTER_IMAGETRANS		0x8002
#define CM_CREATETOCENTER_IMAGETRANS		0x8003


#define CM_COPYFROMVIEW_IMAGETRANS			0x4000
#define CM_COPYTOVIEW_IMAGETRANS			0x4000

#define RESOURCE_PATH		"Res\\CViewPro"

#define IA_NOERROR				0
#define IA_FILE_NOTEXIST		1
#define IA_FILE_ERROR			2

#define WM_ADJUST_SIZE	WM_USER+119

typedef char    SC_STR32[34],     FAR *PSC_STR32;
typedef char    SC_STR8[10],	  FAR *PSC_STR8;

typedef struct {
		HWND		hwndFrom;
		SC_STR32	Id;
		SC_STR32	serialNumber;
		HWND		recvHandle;
		DWORD		type;
		SC_STR32	deviceName;
		int			totalCount;
		int			currentNumber;
		int			combine;
		int			state;
		SC_STR8		docType;
		int			compression;
		int			cbSign;
		SC_STR32	lpSign;
		long		cbData;
}IMAGETRANSHEADER, FAR * PIMAGETRANSHEADER;


class CCViewProDlg : public CDialog
{
// Construction
public:
	CCViewProDlg(CWnd* pParent = NULL);   // standard constructor
	short		GetIndex();
	void		SetIndex(short index);
	short		GetBehavior();
	void		SetBehavior(short behavior);
	short		GetShrink();
	void		SetShrink(short shrink);
	short		GetAutoSize();
	void		SetAutoSize(short autosize);
	void		SetBorder(short border);
	short		GetBorder();
	void		SetBorderColor(COLORREF color);
	COLORREF	GetBorderColor();
	void		SetHoverBorder(short border);
	short		GetHoverBorder();
	void		SetHoverBorderColor(COLORREF color);
	COLORREF	GetHoverBorderColor();
	void		SetBackgroundColor(COLORREF color);
	COLORREF	GetBackgroundColor();
	BOOL		Crop();
	void		Zoom(double value);
	void		Rotate(float angle);
	BOOL		Deskew();
	BOOL		Destain();
	BOOL		CanUndo();
	BOOL		CanRedo();
	void		Undo();
	void		Redo();
	short		GetScale();
	BOOL		Save();
	BOOL		Remove();
	void		Clear();
	BOOL		Copy(HWND handle);
	BOOL		CreateTo(short mode,HWND handle);
	CString		GetGId();
	CString		GetIId();
	CString		GetDocType();
	short		GetSourceType();
	CString		GetSourceDevice();
	BOOL		Filled();
	BOOL		IsBitmap();
	BOOL		IsDoc();
	void		BuildOutward();
	short		GetIWidth();
	short		GetIHeight();
	long		GetFileSize();
	void		SetDocIcon(CString sFilename);
	void		SetIcon(short icon);
	void		SetCurrentPath(LPCTSTR sPath);
	CString		GetImage(short quality,double scale);
	void		MoveImage(int x,int y,int newX,int newY); 
	void		SetBackground(short background);
	void		CreateRect(int left,int top,int right,int bottom,short thickness,COLORREF color);
	void		ClearRect();
	HBITMAP		AddWaterMark(CxImage* cxi);
	void		EnableWaterMark(short enable);
	void		SetWaterMark(LPCTSTR sWaterMarkBase64,short nPostion);
	void		SetTextWaterMark(LPCTSTR sText,LPCTSTR sIP,LPCTSTR sDate,short nPostion);
	void		AddSignature(LPCTSTR sSignatureBase64,long xOffset,long yOffset);
	//BOOL		ExportFile(LPCTSTR sText,LPCTSTR sIP,LPCTSTR sDate,short nPostion);
	//void		AddImageIdentify(LPCTSTR sSignatureBase64,long xOffset,long yOffset);
	CString		GetSelRectInfo();



private:
	//HBITMAP DoRotate(HBITMAP hBitmap);
	HBITMAP DoRotateAndShrink(HBITMAP hBitmap);

	CString Sign(CString szData);
	BOOL VerifySign(CString szDataForSign,CString szSign,int len);
	void TransHeaderSign(IMAGETRANSHEADER* pTransHeader,DWORD dwTransCommand,long nTransDataLength);
	BOOL VerifyTransHeaderSign(IMAGETRANSHEADER* pTransHeader,DWORD dwTransCommand,long nTransDataLength);
	HBITMAP GetNewestBitmap();
	HBITMAP GetNewestRgnBitmap();
	BOOL Save(HWND handle);
	BOOL Remove(HWND handle);
	BOOL CreateTo(short mode,HWND handle,HWND recvHandle);
	void RestoreAllLightRect();
	HBITMAP CreateWaterMarkBitmap(HDC hdc, const char *pszText,const char *pszIP,const char *pszDate);
	HBITMAP AddWaterMark(CxImage* cxi,CxImage* cxiWater,double opacity,int postion);


// Dialog Data
	//{{AFX_DATA(CViewProDlg)
	enum { IDD = IDD_MAIN_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCViewProDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCViewProDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
public:
	CScrollBar	m_vScrollBar;
	CScrollBar	m_hScrollBar;
	CSelectPicture	m_Picture;

private:
	IMAGETRANSHEADER	m_ITransHeader;
	CMd5 md5;
	SCROLLINFO horz,vert;
	CBitmapTrace m_BitmapTrace;
	float m_RotateAngle;
	int m_Index;
	int m_Behavior;
	int m_Shrink;
	int m_AutoSize;

	double m_ZoomScale;
	double m_dShrink;
	HBITMAP hCurBitmap;
	int		m_XDPI;
	int		m_YDPI;
	bool	m_Saved;
	COLORREF m_BorderColor;
	COLORREF m_HoverBorderColor;
	COLORREF m_BackgroundColor;

	BOOL	m_IsBitmapType;
	CString m_CurrentPath;

	std::vector<LIGHT_RECT_INFO> m_AllLightRect;

	short m_EnableWaterMark;
	short m_WaterMarkClass;
	std::string m_TextWaterMarkData;
	std::string m_ClientIP;
	std::string m_ServerDate;
	std::string m_WaterMarkData;
	short m_WaterMarkPostion;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPRODLG_H__5F8939DD_E0B2_4169_93B4_8206169D6F9F__INCLUDED_)
