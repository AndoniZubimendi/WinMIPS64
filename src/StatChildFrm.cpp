// StatChildFrm.cpp : implementation file
//

#include "stdafx.h"
#include "WinEVE.h"
#include "StatChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatChildFrm

IMPLEMENT_DYNCREATE(CStatChildFrm, CMDIChildWnd)

CStatChildFrm::CStatChildFrm()
{
}

CStatChildFrm::~CStatChildFrm()
{
}

static char *stattit="Statistics";

BEGIN_MESSAGE_MAP(CStatChildFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CStatChildFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatChildFrm message handlers

BOOL CStatChildFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style=WS_OVERLAPPEDWINDOW|WS_MAXIMIZE|WS_CHILD;
	cs.lpszName=stattit;
	
	return CMDIChildWnd::PreCreateWindow(cs);
}

void CStatChildFrm::ActivateFrame(int nCmdShow) 
{
	CMenu *pSysMenu=GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(6,MF_BYPOSITION|MF_GRAYED);
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON5)),TRUE);		
	CMDIChildWnd::ActivateFrame(nCmdShow);
}
