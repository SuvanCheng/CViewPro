// CViewPro.cpp : Implementation of CCViewProApp and DLL registration.

#include "stdafx.h"
#include "CViewPro.h"

#include "CatHelp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CCViewProApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x2b6904dd, 0x5765, 0x4ae8, { 0xb2, 0x11, 0x93, 0xbf, 0x83, 0xda, 0x18, 0xe } };

const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;


////////////////////////////////////////////////////////////////////////////
// CCViewProApp::InitInstance - DLL initialization

BOOL CCViewProApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}


////////////////////////////////////////////////////////////////////////////
// CCViewProApp::ExitInstance - DLL termination

int CCViewProApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}

CString CCViewProApp::GetVersion()
{
	CString sVesion;
	sVesion.Format("%d.%d",_wVerMajor,_wVerMinor);
	return sVesion;
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry
/*
STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}*/


STDAPI DllRegisterServer(void)
{
	HRESULT hr = NOERROR;
	AFX_MANAGE_STATE(_afxModuleAddrThis);
    if(!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);
	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);
#ifdef L_USE_COMCAT
	hr = CreateComponentCategory(CATID_SafeForScripting, L"Controls that are safely scriptable");
	if(FAILED(hr)){
		OutputDebugString(_T("Failed to create component category (scriptable)!\n"));
	}
	hr = CreateComponentCategory(CATID_SafeForInitializing, L"Controls safely initializable from persistent data");
	if(FAILED(hr)){
		OutputDebugString(_T("Failed to create component category (persistence)!\n"));
	}
	hr = RegisterCLSIDInCategory(CCircleCtrl::guid, CATID_SafeForScripting);
	if(FAILED(hr)){
		OutputDebugString(_T("Failed to register control as safe for scripting!\n"));
	}
	hr = RegisterCLSIDInCategory(CCircleCtrl::guid, CATID_SafeForInitializing);
	if(FAILED(hr)){
		OutputDebugString(_T("Failed to register control as safe for initializing!\n"));
	}
#endif // L_USE_COMCAT
	return hr;
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry
/*
STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);
	return NOERROR;
}*/

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

#ifdef L_USE_COMCAT
	// This removes the Implemented Categories from the control's registration.
	// Only need to unregister them if registered above.  
	// WARNING: Unregister the control before removing the L_USE_COMCAT definition.  
	HRESULT hr = NOERROR;
	hr = UnRegisterCLSIDInCategory(CCircleCtrl::guid, CATID_SafeForScripting);
	hr = UnRegisterCLSIDInCategory(CCircleCtrl::guid, CATID_SafeForInitializing);
#endif

	return NOERROR;
}

