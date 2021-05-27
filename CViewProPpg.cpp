// CViewProPpg.cpp : Implementation of the CCViewProPropPage property page class.

#include "stdafx.h"
#include "CViewPro.h"
#include "CViewProPpg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


IMPLEMENT_DYNCREATE(CCViewProPropPage, COlePropertyPage)


/////////////////////////////////////////////////////////////////////////////
// Message map

BEGIN_MESSAGE_MAP(CCViewProPropPage, COlePropertyPage)
	//{{AFX_MSG_MAP(CCViewProPropPage)
	// NOTE - ClassWizard will add and remove message map entries
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// Initialize class factory and guid

IMPLEMENT_OLECREATE_EX(CCViewProPropPage, "CViewPro.CViewProPropPage.1",
	0xd087b21, 0x5b96, 0x45f8, 0x86, 0xab, 0xba, 0xe4, 0x60, 0xf8, 0x60, 0x75)


/////////////////////////////////////////////////////////////////////////////
// CCViewProPropPage::CCViewProPropPageFactory::UpdateRegistry -
// Adds or removes system registry entries for CCViewProPropPage

BOOL CCViewProPropPage::CCViewProPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_CVIEWPRO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// CCViewProPropPage::CCViewProPropPage - Constructor

CCViewProPropPage::CCViewProPropPage() :
	COlePropertyPage(IDD, IDS_CVIEWPRO_PPG_CAPTION)
{
	//{{AFX_DATA_INIT(CCViewProPropPage)
	// NOTE: ClassWizard will add member initialization here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_INIT
}


/////////////////////////////////////////////////////////////////////////////
// CCViewProPropPage::DoDataExchange - Moves data between page and properties

void CCViewProPropPage::DoDataExchange(CDataExchange* pDX)
{
	//{{AFX_DATA_MAP(CCViewProPropPage)
	// NOTE: ClassWizard will add DDP, DDX, and DDV calls here
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA_MAP
	DDP_PostProcessing(pDX);
}


/////////////////////////////////////////////////////////////////////////////
// CCViewProPropPage message handlers
