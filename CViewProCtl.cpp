// CViewProCtl.cpp : Implementation of the CCViewProCtrl ActiveX Control class.

#include "stdafx.h"
#include "CViewPro.h"
#include "CViewProCtl.h"
#include "CViewProPpg.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString GetActiveXRegisterPath(CString sCtrlId);
extern BOOL ChangeWindowMessage(UINT uMessageID, BOOL bAllow);

IMPLEMENT_DYNCREATE(CCViewProCtrl, COleControl)

#ifdef L_IMPL_OBJECTSAFETY
	BEGIN_INTERFACE_MAP(CCViewProCtrl, COleControl)
	INTERFACE_PART(CCViewProCtrl, IID_IObjectSafety, ObjectSafety)
	END_INTERFACE_MAP()
#endif 

/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCViewProCtrl, COleControl)
	//{{AFX_MSG_MAP(CCViewProCtrl)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//ON_WM_TIMER()
	//}}AFX_MSG_MAP
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Dispatch map

BEGIN_DISPATCH_MAP(CCViewProCtrl, COleControl)
	//{{AFX_DISPATCH_MAP(CCViewProCtrl)
	DISP_PROPERTY_EX(CCViewProCtrl, "Index", GetIndex, SetIndex, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "Behavior", GetBehavior, SetBehavior, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "Shrink", GetShrink, SetShrink, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "Align", GetAlign, SetAlign, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "Mode", GetMode, SetMode, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "InitWidth", GetInitWidth, SetInitWidth, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "InitHeight", GetInitHeight, SetInitHeight, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "ContainerScreenWidth", GetContainerScreenWidth, SetContainerScreenWidth, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "ContainerScreenHeight", GetContainerScreenHeight, SetContainerScreenHeight, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "AutoSize", GetAutoSize, SetAutoSize, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "Handle", GetHandle, SetNotSupported, VT_I4)
	DISP_PROPERTY_EX(CCViewProCtrl, "Border", GetBorder, SetBorder, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "BorderColor", GetBorderColor, SetBorderColor, VT_BSTR)
	DISP_PROPERTY_EX(CCViewProCtrl, "HoverBorder", GetHoverBorder, SetHoverBorder, VT_I2)
	DISP_PROPERTY_EX(CCViewProCtrl, "HoverBorderColor", GetHoverBorderColor, SetHoverBorderColor, VT_BSTR)
	DISP_PROPERTY_EX(CCViewProCtrl, "BackgroundColor", GetBackgroundColor, SetBackgroundColor, VT_BSTR)
	DISP_FUNCTION(CCViewProCtrl, "Init",Init, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "SetPosRect", SetPosRect, VT_EMPTY, VTS_I2 VTS_I2 VTS_I2 VTS_I2)
	DISP_FUNCTION(CCViewProCtrl, "Copy", Copy, VT_BOOL, VTS_I4)
	DISP_FUNCTION(CCViewProCtrl, "Crop", Crop, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Zoom", Zoom, VT_EMPTY, VTS_R8)
	DISP_FUNCTION(CCViewProCtrl, "Rotate", Rotate, VT_EMPTY, VTS_R4)
	DISP_FUNCTION(CCViewProCtrl, "Deskew", Deskew, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Destain", Destain, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "CanUndo", CanUndo, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "CanRedo", CanRedo, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Undo", Undo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Redo", Redo, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetScale", GetScale, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Clear", Clear, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Save", Save, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetGId", GetGId, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetIId", GetIId, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetSourceType", GetSourceType, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetSourceDevice", GetSourceDevice, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Version", Version, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Filled", Filled, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "Remove", Remove, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "CreateTo", CreateTo, VT_BOOL, VTS_I2 VTS_I4)
	DISP_FUNCTION(CCViewProCtrl, "GetScreenWidth", GetScreenWidth, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetScreenHeight", GetScreenHeight, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetLeft", GetLeft, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetTop", GetTop, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetWidth", GetWidth, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetHeight", GetHeight, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetIWidth", GetIWidth, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetIHeight", GetIHeight, VT_I2, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetFileSize", GetFileSize, VT_BSTR, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "IsDrag", IsDrag, VT_BOOL, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "GetImage", GetImage, VT_BSTR, VTS_I2 VTS_R8)
	DISP_FUNCTION(CCViewProCtrl, "MoveImage", MoveImage, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4)
	DISP_FUNCTION(CCViewProCtrl, "SetBackground", SetBackground, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CCViewProCtrl, "CreateRect", CreateRect, VT_EMPTY, VTS_I4 VTS_I4 VTS_I4 VTS_I4 VTS_I2 VTS_BSTR)
	DISP_FUNCTION(CCViewProCtrl, "ClearRect", ClearRect, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION(CCViewProCtrl, "EnableWaterMark", EnableWaterMark, VT_EMPTY, VTS_I2)
	DISP_FUNCTION(CCViewProCtrl, "SetWaterMark", SetWaterMark, VT_EMPTY, VTS_BSTR VTS_I2)
	DISP_FUNCTION(CCViewProCtrl, "SetTextWaterMark", SetTextWaterMark, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2)
	DISP_FUNCTION(CCViewProCtrl, "AddSignature", AddSignature, VT_EMPTY, VTS_BSTR VTS_I4 VTS_I4)
	//DISP_FUNCTION(CCViewProCtrl, "ExportFile", ExportFile, VT_EMPTY, VTS_BSTR VTS_BSTR VTS_BSTR VTS_I2)
	//DISP_FUNCTION(CCViewProCtrl, "AddImageIdentify", AddImageIdentify, VT_EMPTY, VTS_BSTR VTS_I4 VTS_I4)
	DISP_FUNCTION(CCViewProCtrl, "GetSelRectInfo", GetSelRectInfo, VT_BSTR, VTS_NONE)


	//}}AFX_DISPATCH_MAP
	DISP_FUNCTION_ID(CCViewProCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
END_DISPATCH_MAP()


/////////////////////////////////////////////////////////////////////////////
// Event map

BEGIN_EVENT_MAP(CCViewProCtrl, COleControl)
	//{{AFX_EVENT_MAP(CCViewProCtrl)
	EVENT_CUSTOM("OnMouseMove", FireOnMouseMove, VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM("OnMouseHover", FireOnMouseHover, VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM("OnMouseLeave", FireOnMouseLeave, VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM("OnDBClick", FireOnDBClick, VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM("OnDragStart", FireOnDragStart, VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM("OnDrag", FireOnDrag, VTS_I2  VTS_I2  VTS_I2)
	EVENT_CUSTOM("OnDragEnd", FireOnDragEnd, VTS_I2  VTS_I2  VTS_I2)
	//}}AFX_EVENT_MAP
END_EVENT_MAP()


/////////////////////////////////////////////////////////////////////////////
// Property pages

// TODO: Add more property pages as needed.  Remember to increase the count!
BEGIN_PROPPAGEIDS(CCViewProCtrl, 1)
	PROPPAGEID(CCViewProPropPage::guid)
END_PROPPAGEIDS(CCViewProCtrl)


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCViewProCtrl, "CVIEWPRO.CViewProCtrl.1",
	0xf599076a, 0x42dc, 0x4ab9, 0x8b, 0x39, 0x9b, 0x11, 0x7a, 0xb, 0xf, 0x36)


/////////////////////////////////////////////////////////////////////////////
// Type library ID and version

IMPLEMENT_OLETYPELIB(CCViewProCtrl, _tlid, _wVerMajor, _wVerMinor)

//自定义，对外提供获取CLSID函数
const CLSID CLSID_CViewProCtrl = { 0xf599076a, 0x42dc, 0x4ab9, { 0x8b, 0x39, 0x9b, 0x11, 0x7a, 0xb, 0xf, 0x36 } };

/////////////////////////////////////////////////////////////////////////////
// Interface IDs

const IID BASED_CODE IID_DCViewPro =
		{ 0x840d0ea2, 0x2a4e, 0x4d45, { 0xad, 0x21, 0x76, 0xac, 0x22, 0x20, 0xcf, 0xc1 } };
const IID BASED_CODE IID_DCViewProEvents =
		{ 0x960cf78b, 0x25f7, 0x4e35, { 0x8a, 0xda, 0x8f, 0xb4, 0x77, 0x10, 0xc3, 0x1f } };



/////////////////////////////////////////////////////////////////////////////
// Control type information

static const DWORD BASED_CODE _dwCViewProOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CCViewProCtrl, IDS_CVIEWPRO, _dwCViewProOleMisc)


/////////////////////////////////////////////////////////////////////////////
// CCViewProCtrl::CCViewProCtrlFactory::UpdateRegistry -
// Adds or removes system registry entries for CCViewProCtrl

BOOL CCViewProCtrl::CCViewProCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: Verify that your control follows apartment-model threading rules.
	// Refer to MFC TechNote 64 for more information.
	// If your control does not conform to the apartment-model rules, then
	// you must modify the code below, changing the 6th parameter from
	// afxRegApartmentThreading to 0.

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_CVIEWPRO,
			IDB_CVIEWPRO,
			afxRegApartmentThreading,
			_dwCViewProOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}


/////////////////////////////////////////////////////////////////////////////
// CCViewProCtrl::CCViewProCtrl - Constructor

CCViewProCtrl::CCViewProCtrl()
{
	m_Prop_Index = 0;
	m_Prop_Behavior = 0;
	m_Prop_Shrink = 1;
	m_Prop_Align = 1;
	m_Prop_Mode = 0;
	m_Prop_InitWidth = 0;
	m_Prop_InitHeight = 0;
	m_Prop_AutoSize = 0;
	m_Prop_Border = 0;
	m_Prop_HoverBorder = 0;
	m_Prop_BorderColor = "";
	m_Prop_HoverBorderColor = "";
	m_Prop_BackgroundColor = "";

	m_CurrentPath = "";
	m_CurrentPath = GetCurrentPath();
	m_ViewProDlg.SetCurrentPath(m_CurrentPath);

	InitializeIIDs(&IID_DCViewPro, &IID_DCViewProEvents);
	this->SetInitialSize(400, 300);
	ChangeWindowMessage(WM_COPYDATA,TRUE);

}

CLSID CCViewProCtrl::GetClsid()
{
	return CLSID_CViewProCtrl;
}

/////////////////////////////////////////////////////////////////////////////
// CCViewProCtrl::~CCViewProCtrl - Destructor

CCViewProCtrl::~CCViewProCtrl()
{
	// TODO: Cleanup your control's instance data here.
//AfxMessageBox("~CCViewProCtrl()");
}


/////////////////////////////////////////////////////////////////////////////
// CCViewProCtrl::OnDraw - Drawing function

void CCViewProCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	// TODO: Replace the following code with your own drawing code.
//	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
//	pdc->Ellipse(rcBounds);

	m_ViewProDlg.m_Picture.SetNotify(m_hWnd);//接收Picture NOTIFY窗口

	m_rcBounds = rcBounds;
	m_ViewProDlg.MoveWindow(rcBounds,TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// CCViewProCtrl::DoPropExchange - Persistence support

void CCViewProCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);
	PX_Short(pPX, "Index", m_Prop_Index);
	PX_Short(pPX, "Behavior", m_Prop_Behavior);
	PX_Short(pPX, "Shrink", m_Prop_Shrink);
	PX_Short(pPX, "Align", m_Prop_Align);
	PX_Short(pPX, "Mode", m_Prop_Mode);
	PX_Short(pPX, "InitWidth", m_Prop_InitWidth);
	PX_Short(pPX, "InitHeight", m_Prop_InitHeight);
	PX_Short(pPX, "ContainerScreenWidth", m_Prop_ContainerScreenWidth);
	PX_Short(pPX, "ContainerScreenHeight", m_Prop_ContainerScreenHeight);
	PX_Short(pPX, "AutoSize", m_Prop_AutoSize);
	PX_Short(pPX, "Border", m_Prop_Border);
	PX_Short(pPX, "HoverBorder", m_Prop_HoverBorder);
	PX_String(pPX, _T("BorderColor"), m_Prop_BorderColor, _T(""));
	PX_String(pPX, _T("HoverBorderColor"), m_Prop_HoverBorderColor, _T(""));
	PX_String(pPX, _T("BackgroundColor"), m_Prop_BackgroundColor, _T(""));
}


/////////////////////////////////////////////////////////////////////////////
// CCViewProCtrl::OnResetState - Reset control to default state

void CCViewProCtrl::OnResetState()
{
	COleControl::OnResetState();  // Resets defaults found in DoPropExchange

	// TODO: Reset any other control state here.
}


/////////////////////////////////////////////////////////////////////////////
// CCViewProCtrl message handlers

#ifdef L_IMPL_OBJECTSAFETY
// Implementation of IObjectSafety  
STDMETHODIMP CCViewProCtrl::XObjectSafety::GetInterfaceSafetyOptions(REFIID riid,DWORD __RPC_FAR *pdwSupportedOptions,DWORD __RPC_FAR *pdwEnabledOptions)
{
	METHOD_PROLOGUE_EX(CCViewProCtrl, ObjectSafety)
		if (!pdwSupportedOptions || !pdwEnabledOptions){
			return E_POINTER;
		}
	*pdwSupportedOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER | INTERFACESAFE_FOR_UNTRUSTED_DATA;
	*pdwEnabledOptions = 0;
	if(NULL == pThis->GetInterface(&riid)){
		//  TRACE("Requested interface is not supported.\n");
		return E_NOINTERFACE;
	}
	// What interface is being checked out anyhow?  
	OLECHAR szGUID[39];
	int i = StringFromGUID2(riid, szGUID, 39);
	if(riid == IID_IDispatch){
	// Client wants to know if object is safe for scripting   
		*pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_CALLER;
		return S_OK;
	}else if(riid == IID_IPersistPropertyBag || riid == IID_IPersistStreamInit || riid == IID_IPersistStorage || riid == IID_IPersistMemory){
		// Those are the persistence interfaces COleControl derived controls support
		// as indicated in AFXCTL.H
		// Client wants to know if object is safe for initializing from persistent data   
		*pdwEnabledOptions = INTERFACESAFE_FOR_UNTRUSTED_DATA;
		return S_OK;
	}else{
		// Find out what interface this is, and decide what options to enable 
		//  TRACE("We didn't account for the safety of this interface, and it's one we support...\n");
		return E_NOINTERFACE;
	}
}

STDMETHODIMP CCViewProCtrl::XObjectSafety::SetInterfaceSafetyOptions(REFIID riid,DWORD dwOptionSetMask,DWORD dwEnabledOptions){
	METHOD_PROLOGUE_EX(CCViewProCtrl, ObjectSafety)
		OLECHAR szGUID[39];
	// What is this interface anyway?  
	// We can do a quick lookup in the registry under HKEY_CLASSES_ROOT\Interface  
	int i = StringFromGUID2(riid, szGUID, 39);
	if (0 == dwOptionSetMask && 0 == dwEnabledOptions){
		// the control certainly supports NO requests through the specified interface 
		// so it's safe to return S_OK even if the interface isn't supported.   
		return S_OK;
	}
	// Do we support the specified interface?  
	if (NULL == pThis->GetInterface(&riid)){
		TRACE1("%s is not support.\n", szGUID);
		return E_FAIL;
	}
	if (riid == IID_IDispatch){
		//  TRACE("Client asking if it's safe to call through IDispatch.\n");
		//  TRACE("In other words, is the control safe for scripting?\n"); 
		if (INTERFACESAFE_FOR_UNTRUSTED_CALLER == dwOptionSetMask && INTERFACESAFE_FOR_UNTRUSTED_CALLER == dwEnabledOptions){
			return S_OK;
		}else{
			return E_FAIL;
		}
	}else if(riid == IID_IPersistPropertyBag || riid == IID_IPersistStreamInit || riid == IID_IPersistStorage || riid == IID_IPersistMemory){
		//  TRACE("Client asking if it's safe to call through IPersist*.\n");
		//  TRACE("In other words, is the control safe for initializing from persistent data?\n");
		if(INTERFACESAFE_FOR_UNTRUSTED_DATA == dwOptionSetMask && INTERFACESAFE_FOR_UNTRUSTED_DATA == dwEnabledOptions){
			return NOERROR;
		}else{
			return E_FAIL;
		}
	}else{
		//TRACE1("We didn't account for the safety of %s, and it's one we support...\n", szGUID);
		return E_FAIL;
	}
}

STDMETHODIMP_(ULONG) CCViewProCtrl::XObjectSafety::AddRef(){
	METHOD_PROLOGUE_EX_(CCViewProCtrl, ObjectSafety)
		return (ULONG)pThis->ExternalAddRef();
}

STDMETHODIMP_(ULONG) CCViewProCtrl::XObjectSafety::Release(){
	METHOD_PROLOGUE_EX_(CCViewProCtrl, ObjectSafety)
		return (ULONG)pThis->ExternalRelease();
}

STDMETHODIMP CCViewProCtrl::XObjectSafety::QueryInterface(REFIID iid, LPVOID* ppvObj){
	METHOD_PROLOGUE_EX_(CCViewProCtrl, ObjectSafety)
		return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}
#endif


int CCViewProCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (COleControl::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_ViewProDlg.Create(IDD_MAIN_DIALOG,this);

	if(m_ViewProDlg.GetShrink()!=m_Prop_Shrink)
		m_ViewProDlg.SetShrink(m_Prop_Shrink);

	if(m_ViewProDlg.GetIndex()!=m_Prop_Index)
		m_ViewProDlg.SetIndex(m_Prop_Index);

	if(m_ViewProDlg.GetBehavior()!=m_Prop_Behavior)
		m_ViewProDlg.SetBehavior(m_Prop_Behavior);

	if(m_ViewProDlg.m_Picture.GetAlign()!=m_Prop_Align)
		m_ViewProDlg.m_Picture.SetAlign(m_Prop_Align);

	if(m_ViewProDlg.m_Picture.GetMode()!=m_Prop_Mode)
		m_ViewProDlg.m_Picture.SetMode(m_Prop_Mode);


	if(m_ViewProDlg.GetAutoSize()!=m_Prop_AutoSize){
		m_ViewProDlg.SetAutoSize(m_Prop_AutoSize);
	}

	if(m_ViewProDlg.GetBorder()!=m_Prop_Border)
		m_ViewProDlg.SetBorder(m_Prop_Border);

	if(m_ViewProDlg.GetHoverBorder()!=m_Prop_HoverBorder)
		m_ViewProDlg.SetHoverBorder(m_Prop_HoverBorder);


	if(GetBorderColor()!=m_Prop_BorderColor && m_Prop_BorderColor!=""){
		SetBorderColor(m_Prop_BorderColor);
	}
	if(GetHoverBorderColor()!=m_Prop_HoverBorderColor && m_Prop_HoverBorderColor!=""){
		SetHoverBorderColor(m_Prop_HoverBorderColor);
	}
	if(GetBackgroundColor()!=m_Prop_BackgroundColor && m_Prop_BackgroundColor!=""){
		SetBackgroundColor(m_Prop_BackgroundColor);
	}


	//CRect rect;
	//rect = this->m_rcPos;

	//rect.right = rect.left + m_Prop_InitWidth;
	//rect.bottom = rect.top + m_Prop_InitHeight;

	if(m_Prop_ContainerScreenWidth>0){	
		m_ViewProDlg.MoveWindow(0,0,m_Prop_InitWidth*GetSystemMetrics(SM_CXSCREEN)/m_Prop_ContainerScreenWidth,m_Prop_InitHeight*GetSystemMetrics(SM_CXSCREEN)/m_Prop_ContainerScreenWidth,TRUE);
	}else if(m_Prop_ContainerScreenHeight>0){
		m_ViewProDlg.MoveWindow(0,0,m_Prop_InitWidth*GetSystemMetrics(SM_CYSCREEN)/m_Prop_ContainerScreenHeight,m_Prop_InitHeight*GetSystemMetrics(SM_CYSCREEN)/m_Prop_ContainerScreenHeight,TRUE);		
	}else{
		m_ViewProDlg.MoveWindow(0,0,m_Prop_InitWidth,m_Prop_InitHeight);		
	}
	return 0;
}

CString CCViewProCtrl::GetCurrentPath()
{
	if(m_CurrentPath.GetLength()>0){
		return m_CurrentPath;
	}else{
		CLSID clsid = GetClsid();
		CString sid = "";
		CString lastid = "";
		lastid.Format("%02X%02X-%02X%02X%02X%02X%02X%02X",clsid.Data4[0],clsid.Data4[1],clsid.Data4[2],clsid.Data4[3],clsid.Data4[4],clsid.Data4[5],clsid.Data4[6],clsid.Data4[7]);
		sid.Format("%08X-%04X-%04X-",clsid.Data1,clsid.Data2,clsid.Data3);
		sid += lastid;
		m_CurrentPath = GetActiveXRegisterPath(sid);
	}
	return m_CurrentPath;
}

BSTR CCViewProCtrl::Version() 
{
	CString strResult;	
	strResult = ((CCViewProApp*)AfxGetApp())->GetVersion();
	return strResult.AllocSysString();
}

void CCViewProCtrl::AboutBox()
{
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


BOOL CCViewProCtrl::Init() 
{
	return TRUE;
}

void CCViewProCtrl::OnSize(UINT nType, int cx, int cy) 
{
	COleControl::OnSize(nType, cx, cy);	
}

/*
BOOL CCViewProCtrl::Test(short index, const VARIANT FAR& function) 
{
	if(function.vt == VT_DISPATCH)
		m_JSCallback = function.pdispVal;
	CComVariant avarParams[1];
	avarParams[0] = "ActiveX 回调！"; //指定回调函数的参数
	DISPPARAMS params = {avarParams, NULL, 1, 0};
	if(m_JSCallback)
		m_JSCallback->Invoke(0,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&params, NULL, NULL, NULL);
	return TRUE;
}*/

short CCViewProCtrl::GetIndex() 
{
	return m_ViewProDlg.GetIndex();
}

void CCViewProCtrl::SetIndex(short nNewValue) 
{
	m_ViewProDlg.SetIndex(nNewValue);
	SetModifiedFlag();
}

short CCViewProCtrl::GetBehavior() 
{
	return m_ViewProDlg.GetBehavior();
}

void CCViewProCtrl::SetBehavior(short nNewValue) 
{
	m_ViewProDlg.SetBehavior(nNewValue);
	SetModifiedFlag();
}

short CCViewProCtrl::GetShrink() 
{
	return m_ViewProDlg.GetShrink();
}

void CCViewProCtrl::SetShrink(short nNewValue) 
{
	m_ViewProDlg.SetShrink(nNewValue);
	SetModifiedFlag();
}

short CCViewProCtrl::GetAlign()
{
	return m_ViewProDlg.m_Picture.GetAlign();
}

void CCViewProCtrl::SetAlign(short nNewValue) 
{
	m_ViewProDlg.m_Picture.SetAlign(nNewValue);
	SetModifiedFlag();
}


short CCViewProCtrl::GetMode() 
{
	return m_ViewProDlg.m_Picture.GetMode();
}

void CCViewProCtrl::SetMode(short nNewValue) 
{
	if(m_ViewProDlg.IsBitmap()){
		m_ViewProDlg.m_Picture.SetMode(nNewValue);
		SetModifiedFlag();
	}
}

void CCViewProCtrl::SetInitWidth(short width)
{
	m_Prop_InitWidth = width;
}

short CCViewProCtrl::GetInitWidth()
{
	return m_Prop_InitWidth;
}

void CCViewProCtrl::SetInitHeight(short height)
{
	m_Prop_InitHeight = height;
}

short CCViewProCtrl::GetInitHeight()
{
	return m_Prop_InitHeight;
}

void CCViewProCtrl::SetContainerScreenWidth(short width)
{
	m_Prop_ContainerScreenWidth = width;
}

short CCViewProCtrl::GetContainerScreenWidth()
{
	return m_Prop_ContainerScreenWidth;
}

void CCViewProCtrl::SetContainerScreenHeight(short height)
{
	m_Prop_ContainerScreenHeight = height;
}

short CCViewProCtrl::GetContainerScreenHeight()
{
	return m_Prop_ContainerScreenHeight;
}

void CCViewProCtrl::SetAutoSize(short autosize) 
{
	m_ViewProDlg.SetAutoSize(autosize);
	SetModifiedFlag();
}

short CCViewProCtrl::GetAutoSize() 
{
	return m_ViewProDlg.GetAutoSize();
}

short CCViewProCtrl::GetBorder() 
{
	return m_ViewProDlg.GetBorder();
}

void CCViewProCtrl::SetBorder(short nNewValue) 
{
	m_ViewProDlg.SetBorder(nNewValue);
	SetModifiedFlag();
}
short CCViewProCtrl::GetHoverBorder() 
{
	return m_ViewProDlg.GetHoverBorder();
}

void CCViewProCtrl::SetHoverBorder(short nNewValue) 
{
	m_ViewProDlg.SetHoverBorder(nNewValue);
	SetModifiedFlag();
}
BSTR CCViewProCtrl::GetBorderColor() 
{
	CString sBorderColor;
	COLORREF color = m_ViewProDlg.GetBorderColor();
	BYTE Red = GetRValue(color);
	BYTE Green = GetGValue(color);
	BYTE Blue = GetBValue(color);
	sBorderColor.Format("#%02X%02X%02X",Red,Green,Blue);
	return sBorderColor.AllocSysString();
}

void CCViewProCtrl::SetBorderColor(LPCTSTR lpszNewValue) 
{
	CString sBorderColor = lpszNewValue;
	DWORD r,g,b;
	sscanf(sBorderColor,"#%02X%02X%02X",&r,&g,&b);
	m_ViewProDlg.SetBorderColor(RGB(r,g,b));
	SetModifiedFlag();
}

BSTR CCViewProCtrl::GetHoverBorderColor() 
{
	CString sHoverBorderColor;
	COLORREF color = m_ViewProDlg.GetHoverBorderColor();
	BYTE Red = GetRValue(color);
	BYTE Green = GetGValue(color);
	BYTE Blue = GetBValue(color);
	sHoverBorderColor.Format("#%02X%02X%02X",Red,Green,Blue);
	return sHoverBorderColor.AllocSysString();
}

void CCViewProCtrl::SetHoverBorderColor(LPCTSTR lpszNewValue) 
{
	CString sHoverBorderColor = lpszNewValue;
	DWORD r,g,b;
	sscanf(sHoverBorderColor,"#%02X%02X%02X",&r,&g,&b);
	m_ViewProDlg.SetHoverBorderColor(RGB(r,g,b));
	SetModifiedFlag();
}

BSTR CCViewProCtrl::GetBackgroundColor() 
{
	CString sBackgroundColor;
	COLORREF color = m_ViewProDlg.GetBackgroundColor();
	BYTE Red = GetRValue(color); //得到红颜色
	BYTE Green = GetGValue(color); //得到绿颜色
	BYTE Blue = GetBValue(color); //得到兰颜色
	sBackgroundColor.Format("#%02X%02X%02X",Red,Green,Blue);
	return sBackgroundColor.AllocSysString();
}

void CCViewProCtrl::SetBackgroundColor(LPCTSTR lpszNewValue) 
{
	CString sBackgroundColor = lpszNewValue;
	DWORD r,g,b;
	sscanf(sBackgroundColor,"#%02X%02X%02X",&r,&g,&b);
	m_ViewProDlg.SetBackgroundColor(RGB(r,g,b));
	SetModifiedFlag();
}

BOOL CCViewProCtrl::OnSetObjectRects(LPCRECT lpRectPos, LPCRECT lpRectClip) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return COleControl::OnSetObjectRects(lpRectPos, lpRectClip);
}

void CCViewProCtrl::OnTimer(UINT_PTR nIDEvent)
{
	/*
	if(nIDEvent==1){
		CRect newRc;
		newRc.left = 0;
		newRc.top = 0;
		newRc.right = 600;
		newRc.bottom = 600;

		CString sMessage;

		
		sMessage.Format("%d-%d-%d-%d\n%d-%d-%d-%d",this->m_rcBounds.left,this->m_rcBounds.top,this->m_rcBounds.Width(),this->m_rcBounds.Height(),this->m_rcPos.left,this->m_rcPos.top,this->m_rcPos.Width(),this->m_rcPos.Height());

		int X= this->m_pInPlaceSite->OnPosRectChange(&newRc);

	}else{
		CCViewProCtrl::OnTimer(nIDEvent);
	}
	*/
	//CCViewProCtrl::OnTimer(nIDEvent);
}

void CCViewProCtrl::SetPosRect(short left, short top, short right, short bottom) 
{
	CRect newRc;
	newRc.left = left;
	newRc.top = top;
	newRc.right = right;
	newRc.bottom = bottom;

	//CString sMessage;
	//sMessage.Format("%d-%d-%d-%d\n%d-%d-%d-%d",this->m_rcBounds.left,this->m_rcBounds.top,this->m_rcBounds.Width(),this->m_rcBounds.Height(),this->m_rcPos.left,this->m_rcPos.top,this->m_rcPos.Width(),this->m_rcPos.Height());

	this->m_pInPlaceSite->OnPosRectChange(&newRc);
}

void CCViewProCtrl::OnClose() 
{
	CWnd::OnClose();
}

long CCViewProCtrl::GetHandle() 
{
	return (long)(m_ViewProDlg.m_hWnd);
}


BOOL CCViewProCtrl::Copy(long handle) 
{
	return m_ViewProDlg.Copy((HWND)handle);
}

BOOL CCViewProCtrl::Crop() 
{
	return m_ViewProDlg.Crop();
}

void CCViewProCtrl::Zoom(double value) 
{
	m_ViewProDlg.Zoom(value);
}

void CCViewProCtrl::Rotate(float angle) 
{
	m_ViewProDlg.Rotate(angle);
}

BOOL CCViewProCtrl::Deskew() 
{
	return m_ViewProDlg.Deskew();
}

BOOL CCViewProCtrl::Destain() 
{
	return m_ViewProDlg.Destain();
}

BOOL CCViewProCtrl::CanUndo() 
{
	return m_ViewProDlg.CanUndo();
}

BOOL CCViewProCtrl::CanRedo() 
{
	return m_ViewProDlg.CanRedo();
}

void CCViewProCtrl::Undo() 
{
	m_ViewProDlg.Undo();

}

void CCViewProCtrl::Redo() 
{
	m_ViewProDlg.Redo();

}

void CCViewProCtrl::Clear() 
{
	m_ViewProDlg.Clear();
}


LRESULT CCViewProCtrl::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message){
		case PNM_MOUSEMOVE:{
			if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
				//CComVariant avarParams[2];
				if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
					PNM_NOTIFY_INFO* pni = (PNM_NOTIFY_INFO*)lParam;
					short index = m_ViewProDlg.GetIndex();
					FireOnMouseMove(index,(short)pni->x,(short)pni->y);
					break;
				}

			}
			break;
						}
		case PNM_MOUSEHOVER:{
			if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
				//CComVariant avarParams[2];
				if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
					PNM_NOTIFY_INFO* pni = (PNM_NOTIFY_INFO*)lParam;
					short index = m_ViewProDlg.GetIndex();
					FireOnMouseHover(index,(short)pni->x,(short)pni->y);
					break;
				}

			}
			break;
						}
		case PNM_MOUSELEAVE:{
			if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
				//CComVariant avarParams[2];
				if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
					PNM_NOTIFY_INFO* pni = (PNM_NOTIFY_INFO*)lParam;
					short index = m_ViewProDlg.GetIndex();
					FireOnMouseLeave(index,(short)pni->x,(short)pni->y);
					break;
				}

			}
			break;
						}
		case PNM_DBLCLK:{
			if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
				//CComVariant avarParams[2];
				//指定回调函数的参数
				//参数顺序与JS中的顺序相反
				//avarParams[0] = m_JSDBClickCallbackAttachData;
				//avarParams[1] = m_ViewProDlg.GetIndex();
				//DISPPARAMS params = {avarParams, NULL, 2, 0};
				//if(m_JSDBClickCallback){
				//	m_JSDBClickCallback->Invoke(0,IID_NULL,LOCALE_USER_DEFAULT,DISPATCH_METHOD,&params, NULL, NULL, NULL);
				//}
				
				if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
					PNM_NOTIFY_INFO* pni = (PNM_NOTIFY_INFO*)lParam;
					short index = m_ViewProDlg.GetIndex();
					FireOnDBClick(index,(short)pni->x,(short)pni->y);
					if(m_ViewProDlg.Filled() && m_ViewProDlg.IsDoc()){//打开文件
						CString sFileName;
						sFileName.Format("%s\\temp\\%s-%s",m_CurrentPath,m_ViewProDlg.GetGId(),m_ViewProDlg.GetIId());


						//WIN7下当文件实际目录为C:\Users\用户\AppData\Local\VirtualStore\Windows\SysWOW64\IAPro\cache时,如果还是使用的C:\Windows\SysWOW64\IAPro\cache传给命令行的IE时，IE找不到文件
						TCHAR sAppDataPath[MAX_PATH];
						if(SHGetSpecialFolderPath(NULL,sAppDataPath,CSIDL_LOCAL_APPDATA,false)){
							CString sFile = sFileName;
							int nPos = sFile.Find("\\");
							if(nPos>0){
								CString sRFile = sFile.Right(sFile.GetLength()-nPos);
								if(sRFile.GetLength()>0){
									CString sAppDataFile;
									sAppDataFile.Format("%s\\VirtualStore%s",sAppDataPath,sRFile);
									if(_access(sAppDataFile,0)==0){
										sFileName = sAppDataFile;
									}
								}
							}
						}


						int nRet = (int)ShellExecute(GetDesktopWindow()->m_hWnd,"open",sFileName,NULL,NULL,SW_SHOW);
						if(nRet==SE_ERR_NOASSOC){//SE_ERR_NOASSOC = 31; {没有相关联的应用程序}
							CString sParameters;
							sParameters.Format("shell32.dll,OpenAs_RunDLL %s",sFileName);
							ShellExecute(GetDesktopWindow()->m_hWnd,"open","rundll32.exe",sParameters,"",SW_SHOW);
						}

						/*
						SHELLEXECUTEINFO sei = {0};
						sei.cbSize = sizeof(sei);
						// SEE_MASK_INVOKEIDLIST is needed for the "properties" verb to work reliably.
						sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI | SEE_MASK_INVOKEIDLIST;
						sei.lpDirectory = NULL; // OK if NULL or blank; that will cause current dir to be used.
						sei.nShow = SW_SHOWNORMAL;
						sei.lpVerb = _T("open");
						sei.lpFile = sFileName;
						sei.lpParameters = NULL;
						sei.hwnd = GetDesktopWindow()->m_hWnd;
						BOOL bRet = ShellExecuteEx(&sei);

						*/
					}
					break;
				}

			}
			break;
						}
		case PNM_DRAGSTART:{
			if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
				PNM_NOTIFY_INFO* pni = (PNM_NOTIFY_INFO*)lParam;
				short index = m_ViewProDlg.GetIndex();
				FireOnDragStart(index,(short)pni->x,(short)pni->y);
				break;
			}

						}
		case PNM_DRAG:{			
			if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
				PNM_NOTIFY_INFO* pni = (PNM_NOTIFY_INFO*)lParam;
				short index = m_ViewProDlg.GetIndex();
				FireOnDrag(index,(short)pni->x,(short)pni->y);
				break;
			}

						}
		case PNM_DRAGEND:{
			if((HWND)wParam == m_ViewProDlg.m_Picture.m_hWnd){
				PNM_NOTIFY_INFO* pni = (PNM_NOTIFY_INFO*)lParam;
				short index = m_ViewProDlg.GetIndex();
				FireOnDragEnd(index,(short)pni->x,(short)pni->y);
				break;
			}

						}
		case WM_ADJUST_SIZE:{
			if(wParam == 1){
				/*
				RECT rectOcx;  
				this->GetRectInContainer(&rectOcx);  
				rectOcx.bottom=lParam;  
				//rectOcx.right=v_height;  
				this->SetRectInContainer(&rectOcx);
				*/
				CRect rc;
				//GetWindowRect(&rc);
				this->GetRectInContainer(&rc);



				if(rc.Width()==0){
					rc.right = rc.left + m_Prop_InitWidth;
				}
				if(rc.Height()==0){
					rc.bottom = rc.top + m_Prop_InitHeight;
				}

				
				rc.right = rc.left + lParam;

				//CString sMessage;
				//sMessage.Format("%d-%d-%d-%d-%d",rc.left,rc.top,rc.Width(),rc.Height(),lParam);
				//AfxMessageBox(sMessage);

				this->m_pInPlaceSite->OnPosRectChange(&rc);
				//RedrawWindow(&rc,NULL,RDW_INVALIDATE | RDW_UPDATENOW);
				break;
			}else if(wParam == 2){
				CRect rc;
				//GetWindowRect(&rc);
				this->GetRectInContainer(&rc);

				if(rc.Width()==0){
					rc.right = rc.left + m_Prop_InitWidth;
				}
				if(rc.Height()==0){
					rc.bottom = rc.top + m_Prop_InitHeight;
				}

				//CString sMessage;
				//sMessage.Format("%d-%d-%d-%d-%d",rc.left,rc.top,rc.Width(),rc.Height(),lParam);
				//AfxMessageBox(sMessage);

				rc.bottom = rc.top + lParam;
				this->m_pInPlaceSite->OnPosRectChange(&rc);
				break;
			}

						}
						 
		default:
			break;
	}
	return COleControl::DefWindowProc(message, wParam, lParam);
}

HRESULT GetArrayLength(IDispatch* pDisp, int* pLength)
{
         BSTR varName = L"length";
         VARIANT varValue;
         DISPPARAMS noArgs = {NULL, NULL, 0, 0};
         DISPID dispId;
         HRESULT hr = 0;
         hr = pDisp->GetIDsOfNames(IID_NULL, &varName, 1, LOCALE_USER_DEFAULT, &dispId);
         if (FAILED(hr))
                  return hr;
         hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET, &noArgs, &varValue, NULL, NULL);
         if (SUCCEEDED(hr))
         {
                  *pLength = varValue.intVal;
                  return hr;
         }else{
                  return hr;

         }
}

HRESULT GetArrayNumberOfIndex(IDispatch* pDisp, int index, int * pValue)
{

         CComVariant varName(index, VT_I4);   // 数组下标
         DISPPARAMS noArgs = {NULL, NULL, 0, 0};
         DISPID dispId;
         VARIANT varValue;
         HRESULT hr = 0;       
         varName.ChangeType(VT_BSTR);         // 将数组下标转为数字型，以进行GetIDsOfNames

         //
         // 获取通过下标访问数组的过程，将过程名保存在dispId中

         hr = pDisp->GetIDsOfNames(IID_NULL, &varName.bstrVal, 1, LOCALE_USER_DEFAULT, &dispId);
         if (FAILED(hr))
                  return hr;

         // 调用COM过程，访问指定下标数组元素，根据dispId 将元素值保存在varValue中
		 hr = pDisp->Invoke(dispId, IID_NULL, LOCALE_SYSTEM_DEFAULT, DISPATCH_PROPERTYGET , &noArgs, &varValue, NULL, NULL);

         if (SUCCEEDED(hr))
         {
                  *pValue = varValue.intVal;    // 将数组元素按int类型取出
                  return hr;
         }else{
			 return hr;
         }
}


short CCViewProCtrl::GetScale() 
{
	return m_ViewProDlg.GetScale();
}

/*
BOOL CCViewProCtrl::SetCallback(short eventIndex,const VARIANT FAR& function,LPCTSTR lpszAttachData) 
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(eventIndex==0){
		if(function.vt == VT_DISPATCH){
			m_JSDBClickCallback = function.pdispVal;
			m_JSDBClickCallbackAttachData = lpszAttachData;
		}	
	}
	return TRUE;
}
*/
BOOL CCViewProCtrl::Save() 
{
	return m_ViewProDlg.Save();
}

BOOL CCViewProCtrl::CreateTo(short mode,long handle) 
{
	return m_ViewProDlg.CreateTo(mode,(HWND)handle);
}


BSTR CCViewProCtrl::GetGId() 
{
	CString strResult;
	strResult = m_ViewProDlg.GetGId();
	return strResult.AllocSysString();
}

BSTR CCViewProCtrl::GetIId() 
{
	CString strResult;
	strResult = m_ViewProDlg.GetIId();
	return strResult.AllocSysString();
}


short CCViewProCtrl::GetSourceType() 
{
	return m_ViewProDlg.GetSourceType();
}

BSTR CCViewProCtrl::GetSourceDevice() 
{
	CString strResult;
	strResult = m_ViewProDlg.GetSourceDevice();
	return strResult.AllocSysString();
}

BOOL CCViewProCtrl::Filled() 
{
	return m_ViewProDlg.Filled();
}

BOOL CCViewProCtrl::Remove() 
{	
	return m_ViewProDlg.Remove();
}

short CCViewProCtrl::GetScreenWidth() 
{
	return GetSystemMetrics(SM_CXSCREEN);
}

short CCViewProCtrl::GetScreenHeight() 
{
	return GetSystemMetrics(SM_CYSCREEN);
}

short CCViewProCtrl::GetLeft() 
{
	CRect rc;
	GetWindowRect(&rc);
	return (short)rc.left;
}

short CCViewProCtrl::GetTop() 
{
	CRect rc;
	GetWindowRect(&rc);
	return (short)rc.top;
}

short CCViewProCtrl::GetWidth() 
{
	CRect rc;
	GetWindowRect(&rc);
	return rc.Width();
}

short CCViewProCtrl::GetHeight() 
{
	CRect rc;
	GetWindowRect(&rc);
	return rc.Height();
}

short CCViewProCtrl::GetIWidth() 
{
	return m_ViewProDlg.GetIWidth();
}

short CCViewProCtrl::GetIHeight() 
{
	return m_ViewProDlg.GetIHeight();
}

BSTR CCViewProCtrl::GetFileSize() 
{
	double Size = __int64(m_ViewProDlg.GetFileSize()/1024*100+0.5)/100.0;
	CString sSize;
	sSize.Format("%.2f",Size);//取两位小数
	return sSize.AllocSysString();
}

BOOL CCViewProCtrl::IsDrag() 
{
	return m_ViewProDlg.m_Picture.IsDrag();
}

BSTR CCViewProCtrl::GetImage(short quality,double scale) 
{
	CString strResult;
	strResult = m_ViewProDlg.GetImage(quality,scale);
	return strResult.AllocSysString();
}

void CCViewProCtrl::MoveImage(int x,int y,int newX,int newY) 
{
	return m_ViewProDlg.MoveImage(x,y,newX,newY);
}

void CCViewProCtrl::SetBackground(short background) 
{
	return m_ViewProDlg.SetBackground(background);
}

void CCViewProCtrl::CreateRect(int left,int top,int right,int bottom,short thickness,LPCTSTR lpszColorValue)
{
	CString sLineColor = lpszColorValue;
	DWORD r,g,b;
	sscanf(sLineColor,"#%02X%02X%02X",&r,&g,&b);
	m_ViewProDlg.CreateRect(left,top,right,bottom,thickness,RGB(r,g,b));
}
void CCViewProCtrl::ClearRect()
{
	m_ViewProDlg.ClearRect();
}

void CCViewProCtrl::EnableWaterMark(short enable){
	m_ViewProDlg.EnableWaterMark(enable);
}

void CCViewProCtrl::SetWaterMark(LPCTSTR sWaterMarkBase64,short nPostion)
{
	m_ViewProDlg.SetWaterMark(sWaterMarkBase64,nPostion);
}

void CCViewProCtrl::SetTextWaterMark(LPCTSTR sText,LPCTSTR sIP,LPCTSTR sDate,short nPostion)
{
	m_ViewProDlg.SetTextWaterMark(sText,sIP,sDate,nPostion);
}

void CCViewProCtrl::AddSignature(LPCTSTR sSignatureBase64,long xOffset,long yOffset)
{
	m_ViewProDlg.AddSignature(sSignatureBase64,xOffset,yOffset);
}

/*
void CCViewProCtrl::ExportFile(LPCTSTR sText,LPCTSTR sIP,LPCTSTR sDate,short nPostion)
{
	m_ViewProDlg.ExportFile(sText,sIP,sDate,nPostion);
}
*/

/*
void CCViewProCtrl::AddImageIdentify(LPCTSTR sSignatureBase64,long xOffset,long yOffset)
{
	m_ViewProDlg.AddImageIdentify(sSignatureBase64,xOffset,yOffset);
}*/

BSTR CCViewProCtrl::GetSelRectInfo()
{
	CString strResult;
	strResult = m_ViewProDlg.GetSelRectInfo();
	return strResult.AllocSysString();
}
