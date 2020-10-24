// CyclesChildFrm.cpp : implementation file
//

#include "stdafx.h"
#include "wineve.h"
#include "CyclesChildFrm.h"
#include "CyclesView.h"
#include "InstructView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCyclesChildFrm

IMPLEMENT_DYNCREATE(CCyclesChildFrm, CMDIChildWnd)

CCyclesChildFrm::CCyclesChildFrm()
{
}

CCyclesChildFrm::~CCyclesChildFrm()
{
}

static char *cyclestit="Cycles";

BEGIN_MESSAGE_MAP(CCyclesChildFrm, CMDIChildWnd)
	//{{AFX_MSG_MAP(CCyclesChildFrm)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCyclesChildFrm message handlers

void CCyclesChildFrm::ActivateFrame(int nCmdShow) 
{
	// TODO: Add your specialized code here and/or call the base class
	CMenu *pSysMenu=GetSystemMenu(FALSE);
	pSysMenu->EnableMenuItem(6,MF_BYPOSITION|MF_GRAYED);
	SetIcon(LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON4)),TRUE);		
	CMDIChildWnd::ActivateFrame(nCmdShow);
}

BOOL CCyclesChildFrm::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style=WS_OVERLAPPEDWINDOW|WS_MAXIMIZE|WS_CHILD;
	cs.lpszName=cyclestit;
	
	return CMDIChildWnd::PreCreateWindow(cs);
}

BOOL CCyclesChildFrm::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	BOOL outcome=split.CreateStatic(this,1,2,WS_CHILD | WS_VISIBLE | WS_HSCROLL | WS_VSCROLL);
	split.CreateView(0,0,RUNTIME_CLASS(CInstructView),CSize(0,0),pContext);
	split.CreateView(0,1,RUNTIME_CLASS(CCyclesView),CSize(0,0),pContext);
	split.SetColumnInfo(0,100,100);
	return outcome;
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}
