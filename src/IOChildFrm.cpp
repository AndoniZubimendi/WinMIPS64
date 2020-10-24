// IOChildFrm.cpp : implementation file
//

#include "stdafx.h"
#include "wineve.h"
#include "IOChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIOChildFrm

IMPLEMENT_DYNCREATE(CIOChildFrm, CMDIChildWnd)

CIOChildFrm::CIOChildFrm()
{
}

CIOChildFrm::~CIOChildFrm()
{
}

static char *IOtitle="Terminal";

BEGIN_MESSAGE_MAP(CIOChildFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CIOChildFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIOChildFrm message handlers

void CIOChildFrm::ActivateFrame(int nCmdShow) 
{
	CMenu *pSysMenu=GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(6,MF_BYPOSITION|MF_GRAYED);
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON7)),TRUE);	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CIOChildFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style=WS_OVERLAPPEDWINDOW|WS_MINIMIZE|WS_CHILD;
	cs.lpszName=IOtitle;
	
	return CMDIChildWnd::PreCreateWindow(cs);
}
