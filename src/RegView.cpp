// RegView.cpp : implementation file
//

#include "stdafx.h"
#include "WinEVE.h"
#include "RegDialog.h"

#include "RegView.h"
#include "FRegDialog.h"
#include "WinEVEDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CRegView

IMPLEMENT_DYNCREATE(CRegView, CScrollView)

CRegView::CRegView()
{
}

CRegView::~CRegView()
{
}


BEGIN_MESSAGE_MAP(CRegView, CScrollView)
	//{{AFX_MSG_MAP(CRegView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegView drawing

void CRegView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	font.CreateFont(15,0,0,0,400,FALSE,FALSE,0,
					ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,"Courier New");
}

void CRegView::OnDraw(CDC* pDC)
{
	unsigned int i;
	DOUBLE64 db;
	SIGNED32 source;

	CWinEVEDoc* pDoc = GetDocument();
	char txt[200];

	pDC->SelectObject(&font);

	// R[] registers
	for (i=0;i<32;i++)
	{
		pDC->SetTextColor(BLACK);
		source=pDoc->cpu.rreg[i].source;
		if (source==FROM_ID) 
			pDC->SetTextColor(CYAN);
		if (source==FROM_EX) 
			pDC->SetTextColor(RED);
		if (source==FROM_MEM) 
			pDC->SetTextColor(GREEN);
		if (source<=NOT_AVAILABLE) 
			pDC->SetTextColor(GREY);

		sprintf(txt,"R%d=  ",i);
		sprintword(&txt[5],pDoc->cpu.rreg[i].val);

		pDC->TextOut(0,i*14,txt);

	}
	// F[] registers
	for (i=0;i<32;i++)
	{
		pDC->SetTextColor(BLACK);
		source=pDoc->cpu.rreg[i+32].source;
		if (source==FROM_ID) 
			pDC->SetTextColor(CYAN);
		if (source==FROM_EX) 
			pDC->SetTextColor(RED);
		if (source==FROM_MEM) 
			pDC->SetTextColor(GREEN);
		if (source==FROM_ADD)
			pDC->SetTextColor(DGREEN);
		if (source==FROM_MUL)
			pDC->SetTextColor(DCYAN);
		if (source==FROM_DIV)
			pDC->SetTextColor(DYELLOW);
		if (source<=NOT_AVAILABLE) 
			pDC->SetTextColor(GREY);

		db.s=pDoc->cpu.rreg[i+32].val;
		sprintf(txt,"F%d=  ",i);
		sprintdouble(&txt[5],db.d);
	
		pDC->TextOut(160,i*14,txt);
	}

	CPoint pt;
	pt=GetScrollPosition();
	first=pt.y/14;

	if (pt.y%14 != 0)
	{
		pt.y=14*first;
		ScrollToPosition(pt);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRegView diagnostics

#ifdef _DEBUG
void CRegView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CRegView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinEVEDoc* CRegView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinEVEDoc)));
	return (CWinEVEDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRegView message handlers

void CRegView::OnLButtonDblClk(UINT /* nFlags */, CPoint point) 
{
	DOUBLE64 db;
	CRegDialog dlg;
	CFRegDialog fdlg;

	BOOL fr=FALSE;
	int reg=first+point.y/14;
	char txt[1000];
	char txt1[1000];
	CWinEVEDoc* pDoc = GetDocument();
	
	if (reg>31) return;

	if (point.x>160) fr=TRUE;

	if (point.x>310) return;
	
	if (fr) 
	{
		if (pDoc->cpu.rreg[reg+32].source!=FROM_REGISTER) return;
		db.s=pDoc->cpu.rreg[reg+32].val;
		sprintdouble(txt1,db.d);
		fdlg.m_freg=txt1;
		sprintf(txt,"F%d=",reg);
		fdlg.m_fname=txt;
		fdlg.DoModal();
		if (strcmp(txt1,LPCTSTR(fdlg.m_freg))!=0)
		{
			strcpy(txt,LPCTSTR(fdlg.m_freg));
			db.d=atof(txt);
			pDoc->cpu.rreg[reg+32].val=db.u;
			pDoc->cpu.wreg[reg+32].val=db.u;
		}

	}
	else	
	{
		if (reg==0) return;
		if (pDoc->cpu.rreg[reg].source!=FROM_REGISTER) return;
//		sprintf(txt,"%16I64x",pDoc->cpu.rreg[reg].val);
		sprintword(txt,pDoc->cpu.rreg[reg].val);
		dlg.m_reg=txt;
		sprintf(txt,"R%d=",reg);
		dlg.m_name=txt;
		dlg.DoModal();
		strcpy(txt,LPCTSTR(dlg.m_reg));
		pDoc->cpu.rreg[reg].val=strtoint64(txt,NULL,16);
		pDoc->cpu.wreg[reg].val=strtoint64(txt,NULL,16);
	}

	OnUpdate(NULL,0,NULL);
}

void CRegView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
	case VK_HOME:
		OnVScroll(SB_TOP,0,NULL);
		break;
	case VK_END:
		OnVScroll(SB_BOTTOM,0,NULL);
		break;
	case VK_UP:
		OnVScroll(SB_LINEUP,0,NULL);
		break;
	case VK_DOWN:
		OnVScroll(SB_LINEDOWN,0,NULL);
		break;
	default:
		break;
		
	}	
	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRegView::OnUpdate(CView* /* pSender */, LPARAM lHint, CObject* /* pHint */) 
{

	CSize sizeTotal;
	if (lHint==2) return;
	sizeTotal.cx = 310;
	sizeTotal.cy = 32*14+7;
	SetScrollSizes(MM_TEXT, sizeTotal,CSize(100,56),CSize(100,14));
	InvalidateRect(NULL);
}
