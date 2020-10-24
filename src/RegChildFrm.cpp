// RegChildFrm.cpp : implementation file
//

#include "stdafx.h"
#include "WinEVE.h"
#include "RegChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegChildFrm

IMPLEMENT_DYNCREATE(CRegChildFrm, CMDIChildWnd)

CRegChildFrm::CRegChildFrm()
{
}

CRegChildFrm::~CRegChildFrm()
{
}


static char *regtit="Registros";

BEGIN_MESSAGE_MAP(CRegChildFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CRegChildFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegChildFrm message handlers

BOOL CRegChildFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	cs.style=WS_OVERLAPPEDWINDOW|WS_MAXIMIZE|WS_CHILD;
	cs.lpszName=regtit;
	return CMDIChildWnd::PreCreateWindow(cs);
}

void CRegChildFrm::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMenu *pSysMenu=GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(6,MF_BYPOSITION|MF_GRAYED);
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON2)),TRUE);	
	CMDIChildWnd::ActivateFrame(nCmdShow);

}
