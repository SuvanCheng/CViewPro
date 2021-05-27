#if !defined(AFX_CViewProCTL_H__61D031DA_EFB5_42CB_96C4_7B9CA30101E3__INCLUDED_)
#define AFX_CViewProCTL_H__61D031DA_EFB5_42CB_96C4_7B9CA30101E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <io.h>

#include <atlbase.h>

#include "CViewProDlg.h"

#define L_IMPL_OBJECTSAFETY 1

// CViewProCtl.h : Declaration of the CCViewProCtrl ActiveX Control class.

/////////////////////////////////////////////////////////////////////////////
// CCViewProCtrl : See CViewProCtl.cpp for implementation.

//实现 IObjectSafety 接口的控件 在CircleCtl.h加入  
#ifdef L_IMPL_OBJECTSAFETY
#include <objsafe.h>
#endif

class CCViewProCtrl : public COleControl
{
	DECLARE_DYNCREATE(CCViewProCtrl)

// Constructor
public:
	CCViewProCtrl();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCViewProCtrl)
	public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL OnSetObjectRects(LPCRECT lpRectPos, LPCRECT lpRectClip);
	protected:
	virtual LRESULT DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	~CCViewProCtrl();

	DECLARE_OLECREATE_EX(CCViewProCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CCViewProCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CCViewProCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CCViewProCtrl)		// Type name and misc status

// Message maps
	//{{AFX_MSG(CCViewProCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


public:
#ifdef L_IMPL_OBJECTSAFETY
	BEGIN_INTERFACE_PART(ObjectSafety, IObjectSafety)
	STDMETHOD(GetInterfaceSafetyOptions)(REFIID riid, DWORD __RPC_FAR *pdwSupportedOptions, DWORD __RPC_FAR *pdwEnabledOptions);
	STDMETHOD(SetInterfaceSafetyOptions)(REFIID riid, DWORD dwOptionSetMask, DWORD dwEnabledOptions);
	END_INTERFACE_PART(ObjectSafety)
	DECLARE_INTERFACE_MAP(); 
#endif

// Dispatch maps
	//{{AFX_DISPATCH(CCViewProCtrl)
	afx_msg short GetIndex();
	afx_msg void SetIndex(short nNewValue);
	afx_msg short GetBehavior();
	afx_msg void SetBehavior(short nNewValue);
	afx_msg short GetShrink();
	afx_msg void SetShrink(short nNewValue);
	afx_msg short GetAlign();
	afx_msg void SetAlign(short nNewValue);
	afx_msg short GetMode();
	afx_msg void SetMode(short nNewValue);
	afx_msg void SetInitWidth(short width);
	afx_msg short GetInitWidth();
	afx_msg void SetInitHeight(short height);
	afx_msg short GetInitHeight();
	afx_msg void SetContainerScreenWidth(short width);
	afx_msg short GetContainerScreenWidth();
	afx_msg void SetContainerScreenHeight(short height);
	afx_msg short GetContainerScreenHeight();
	afx_msg void SetAutoSize(short autosize);
	afx_msg short GetAutoSize();
	afx_msg long GetHandle();
	afx_msg short GetBorder();
	afx_msg void SetBorder(short nNewValue);
	afx_msg BSTR GetBorderColor();
	afx_msg void SetBorderColor(LPCTSTR lpszNewValue);
	afx_msg short GetHoverBorder();
	afx_msg void SetHoverBorder(short nNewValue);
	afx_msg BSTR GetHoverBorderColor();
	afx_msg void SetHoverBorderColor(LPCTSTR lpszNewValue);
	afx_msg BSTR GetBackgroundColor();
	afx_msg void SetBackgroundColor(LPCTSTR lpszNewValue);
	afx_msg BOOL Init();
	afx_msg void SetPosRect(short left, short top, short right, short bottom);
	afx_msg BOOL Copy(long handle);
	afx_msg BOOL Crop();
	afx_msg void Zoom(double value);
	afx_msg void Rotate(float angle);
	afx_msg BOOL Deskew();
	afx_msg BOOL Destain();
	afx_msg BOOL CanUndo();
	afx_msg BOOL CanRedo();
	afx_msg void Undo();
	afx_msg void Redo();
	afx_msg short GetScale();
	afx_msg void Clear();
	afx_msg BOOL Save();
	afx_msg BSTR GetGId();
	afx_msg BSTR GetIId();
	afx_msg short GetSourceType();
	afx_msg BSTR GetSourceDevice();
	afx_msg BSTR Version();
	afx_msg BOOL Filled();
	afx_msg BOOL Remove();
	afx_msg BOOL CreateTo(short mode,long handle);
	afx_msg short GetScreenWidth();
	afx_msg short GetScreenHeight();
	afx_msg short GetLeft();
	afx_msg short GetTop();
	afx_msg short GetWidth();
	afx_msg short GetHeight();
	afx_msg short GetIWidth();
	afx_msg short GetIHeight();
	afx_msg BSTR GetFileSize();
	afx_msg BOOL IsDrag();
	afx_msg BSTR GetImage(short quality,double scale);
	afx_msg void MoveImage(int x,int y,int newX,int newY);
	afx_msg void SetBackground(short background);
	afx_msg void CreateRect(int left,int top,int right,int bottom,short thickness,LPCTSTR lpszColorValue);
	afx_msg void ClearRect();
	afx_msg void EnableWaterMark(short enable);
	afx_msg void SetWaterMark(LPCTSTR sWaterMarkBase64,short nPostion);
	afx_msg void SetTextWaterMark(LPCTSTR sText,LPCTSTR sIP,LPCTSTR sDate,short nPostion);
	afx_msg void AddSignature(LPCTSTR sSignatureBase64,long xOffset,long yOffset);
	//afx_msg void ExportFile(LPCTSTR sText,LPCTSTR sIP,LPCTSTR sDate,short nPostion);
	//afx_msg void AddImageIdentify(LPCTSTR sSignatureBase64,long xOffset,long yOffset);

	afx_msg BSTR GetSelRectInfo();







	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	//{{AFX_EVENT(CCViewProCtrl)
	void FireOnMouseMove(short index, short x, short y)
		{FireEvent(eventidOnMouseMove,EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2), index, x, y);}
	void FireOnMouseHover(short index, short x, short y)
		{FireEvent(eventidOnMouseHover,EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2), index, x, y);}
	void FireOnMouseLeave(short index, short x, short y)
		{FireEvent(eventidOnMouseLeave,EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2), index, x, y);}
	void FireOnDBClick(short index, short x, short y)
		{FireEvent(eventidOnDBClick,EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2), index, x, y);}
	void FireOnDragStart(short index, short x, short y)
		{FireEvent(eventidOnDragStart,EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2), index, x, y);}
	void FireOnDrag(short index, short x, short y)
		{FireEvent(eventidOnDrag,EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2), index, x, y);}
	void FireOnDragEnd(short index, short x, short y)
		{FireEvent(eventidOnDragEnd,EVENT_PARAM(VTS_I2  VTS_I2  VTS_I2), index, x, y);}
	//}}AFX_EVENT
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	//{{AFX_DISP_ID(CCViewProCtrl)
	dispidIndex = 1L,
	dispidBehavior = 2L,
	dispidShrink = 3L,
	dispidAlign = 4L,
	dispidMode = 5L,
	dispidInitWidth = 6L,
	dispidInitHeight = 7L,
	dispidInitContainerScreenWidth = 8L,
	dispidInitContainerScreenHeight = 9L,
	dispidAutoSize = 10L,
	dispidHandle = 11L,
	dispidBorder = 12L,
	dispidBorderColor = 13L,
	dispidHoverBorder = 14L,
	dispidHoverBorderColor = 15L,
	dispidBackgroundColor = 16L,
	dispidInit = 17L,
	dispidSetPosRect = 18L,
	dispidCopy = 19L,
	dispidCrop = 20L,
	dispidZoom = 21L,
	dispidRotate = 22L,
	dispidDeskew = 23L,
	dispidDestain = 24L,
	dispidCanUndo = 25L,
	dispidCanRedo = 26L,
	dispidUndo = 27L,
	dispidRedo = 28L,
	dispidGetScale = 29L,
	dispidClear = 30L,
	dispidSave = 31L,
	dispidGetGId = 32L,
	dispidGetIId = 33L,
	dispidGetSourceType = 34L,
	dispidGetSourceDevice = 35L,
	dispidVersion = 36L,
	dispidFilled = 37L,
	dispidRemove = 38L,
	dispidCreateTo = 39L,
	dispidGetScreenWidth = 40L,
	dispidGetScreenHeight = 41L,
	dispidGetLeft = 42L,
	dispidGetTop = 43L,
	dispidGetWidth = 44L,
	dispidGetHeight = 45L,
	dispidGetIWidth = 46L,
	dispidGetIHeight = 47L,
	dispidGetFileSize = 48L,
	dispidIsDrag = 49L,
	dispidGetImage = 50L,
	dispidMoveImage = 51L,
	dispidSetBackground = 52L,
	dispidCreateRect = 53L,
	dispidClearRect = 54L,
	dispidEnableWaterMark = 55L,
	dispidSetWaterMark = 56L,
	dispidSetTextWaterMark = 57L,
	dispidAddSignature = 58L,
	//dispidExportFile = 59L,
	//dispidAddImageIdentify = 59L,
	dispidGetSelRectInfo = 59L,



	eventidOnMouseMove = 1L,
	eventidOnMouseHover = 2L,
	eventidOnMouseLeave = 3L,
	eventidOnDBClick = 4L,
	eventidOnDragStart = 5L,
	eventidOnDrag = 6L,
	eventidOnDragEnd = 7L,
	//}}AFX_DISP_ID
	};


public:
	CLSID GetClsid();
	CString GetCurrentPath();

public:
	CCViewProDlg m_ViewProDlg;
	/*
	CComPtr<IDispatch> m_JSDBClickCallback;
	CString m_JSDBClickCallbackAttachData;
	
	CComPtr<IDispatch> m_JSDragCallback;
	CString m_JSDragCallbackAttachData;
*/
private:
	short m_Prop_Index;
	short m_Prop_Behavior;
	short m_Prop_Shrink;
	short m_Prop_Align;
	short m_Prop_Mode;
	short m_Prop_InitWidth;
	short m_Prop_InitHeight;
	short m_Prop_ContainerScreenWidth;
	short m_Prop_ContainerScreenHeight;
	short m_Prop_AutoSize;
	short m_Prop_Border;
	short m_Prop_HoverBorder;
	CString m_Prop_BorderColor;
	CString m_Prop_HoverBorderColor;
	CString m_Prop_BackgroundColor;


	CString m_CurrentPath;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CViewProCTL_H__61D031DA_EFB5_42CB_96C4_7B9CA30101E3__INCLUDED)
