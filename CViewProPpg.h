#if !defined(AFX_CViewProPPG_H__24C93F38_DA9B_426E_98E3_DBC1FCD0DD2E__INCLUDED_)
#define AFX_CViewProPPG_H__24C93F38_DA9B_426E_98E3_DBC1FCD0DD2E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// CViewProPpg.h : Declaration of the CCViewProPropPage property page class.

////////////////////////////////////////////////////////////////////////////
// CCViewProPropPage : See CViewProPpg.cpp.cpp for implementation.

class CCViewProPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CCViewProPropPage)
	DECLARE_OLECREATE_EX(CCViewProPropPage)

// Constructor
public:
	CCViewProPropPage();

// Dialog Data
	//{{AFX_DATA(CCViewProPropPage)
	enum { IDD = IDD_PROPPAGE_CVIEWPRO};
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA
// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Message maps
protected:
	//{{AFX_MSG(CCViewProPropPage)
		// NOTE - ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CViewProPPG_H__24C93F38_DA9B_426E_98E3_DBC1FCD0DD2E__INCLUDED)
