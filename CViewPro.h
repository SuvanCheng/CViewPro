#if !defined(AFX_CVIEWPRO_H__44D3252F_D443_439B_8E39_0924BFF63B00__INCLUDED_)
#define AFX_CVIEWPRO_H__44D3252F_D443_439B_8E39_0924BFF63B00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CViewPro.h : main header file for CVIEWPRO.DLL

#if !defined( __AFXCTL_H__ )
	#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CCViewProApp : See CViewPro.cpp for implementation.

class CCViewProApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
	CString GetVersion();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CVIEWPRO_H__44D3252F_D443_439B_8E39_0924BFF63B00__INCLUDED)
