// DataChildFrm.cpp : implementation file
//

#include "stdafx.h"
#include "WinEVE.h"
#include "DataChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataChildFrm

IMPLEMENT_DYNCREATE(CDataChildFrm, CMDIChildWnd)

CDataChildFrm::CDataChildFrm()
{
}

CDataChildFrm::~CDataChildFrm()
{
}

static char *datatit="Data";

BEGIN_MESSAGE_MAP(CDataChildFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CDataChildFrm)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataChildFrm message handlers

BOOL CDataChildFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style=WS_OVERLAPPEDWINDOW|WS_MAXIMIZE|WS_CHILD;
	cs.lpszName=datatit;


	return CMDIChildWnd::PreCreateWindow(cs);
}

void CDataChildFrm::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMenu *pSysMenu=GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(6,MF_BYPOSITION|MF_GRAYED);
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON1)),TRUE);
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CDataChildFrm::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CMDIFrameWnd* pParentWnd, CCreateContext* pContext) 
{
	return CMDIChildWnd::Create(lpszClassName,lpszWindowName, dwStyle, rect, pParentWnd, pContext);
}

BOOL CDataChildFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{

	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
