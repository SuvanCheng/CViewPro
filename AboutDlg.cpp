// AboutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CViewPro.h"
#include "AboutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog


CAboutDlg::CAboutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAboutDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg message handlers


BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	int x = 0;
	int y = 0;
	int width = 0;
	int height = 0;

	CRect rect;
	GetWindowRect(&rect);
	width = rect.Width();
	height = rect.Height();

	CWnd *pWnd = GetParent();
	if(pWnd && pWnd->m_hWnd){
		::GetWindowRect(pWnd->m_hWnd,&rect);
		x = rect.left+(rect.Width()-width)/2;
		y = rect.top+(rect.Height()-height)/2;
		MoveWindow(x,y,width,height);
		return TRUE;
	}

	int xScrn = GetSystemMetrics(SM_CXSCREEN);  
    int yScrn = GetSystemMetrics(SM_CYSCREEN);
	x = (xScrn-width)/2;
	y = (yScrn-height)/2;
	MoveWindow(x,y,width,height);
	UpdateData(FALSE);
	return TRUE;
}
