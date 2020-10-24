// PipeChildFrm.cpp : implementation file
//

#include "stdafx.h"
#include "wineve.h"
#include "PipeChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPipeChildFrm

IMPLEMENT_DYNCREATE(CPipeChildFrm, CMDIChildWnd)

CPipeChildFrm::CPipeChildFrm()
{
}

CPipeChildFrm::~CPipeChildFrm()
{
}

static char *pipetit="Pipeline";

BEGIN_MESSAGE_MAP(CPipeChildFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CPipeChildFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPipeChildFrm message handlers

void CPipeChildFrm::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMenu *pSysMenu=GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(6,MF_BYPOSITION|MF_GRAYED);
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON3)),TRUE);	
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CPipeChildFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style=WS_OVERLAPPEDWINDOW|WS_MAXIMIZE|WS_CHILD;
	cs.lpszName=pipetit;

	return CMDIChildWnd::PreCreateWindow(cs);
}
