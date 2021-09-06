// InstructView.cpp : implementation file
//

#include "stdafx.h"
#include "WinMIPS64.h"
#include "InstructView.h"
#include "utils.h"
#include "WinMIPS64Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInstructView

IMPLEMENT_DYNCREATE(CInstructView, CScrollView)

CInstructView::CInstructView()
{
}

CInstructView::~CInstructView()
{
}


BEGIN_MESSAGE_MAP(CInstructView, CScrollView)
	//{{AFX_MSG_MAP(CInstructView)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInstructView drawing

void CInstructView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CSize sizeTotal;
	// TODO: calculate the total size of this view
//	sizeTotal.cx = 101;
//	sizeTotal.cy = 140;
//	SetScrollSizes(MM_TEXT, sizeTotal);

	font.CreateFont(14,0,0,0,400,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH| FF_SWISS,"Arial");


//	font.CreateFont(10,0,0,0,400,FALSE,FALSE,0,
//				ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
//				DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,"Courier");	
}

void CInstructView::OnDraw(CDC* pDC)
{
	CWinMIPS64Doc* pDoc = GetDocument();
	CPoint ps;
	int cause,now;
	
	
	ps=GetScrollPosition();
	first=ps.y/14;

	CRect sz;
	GetClientRect(&sz);
	nlines=1+sz.Height()/14;

	pDC->SelectObject(&font);
	for (unsigned int i=0;i<pDoc->entries;i++)
	{
		now=pDoc->cycles-pDoc->history[i].start_cycle;
		cause=pDoc->history[i].status[now].cause;
		if (cause==RAW || cause==WAW || cause==STRUCTURAL)	pDC->SetTextColor(BLUE);
		else if (cause==STALLED) pDC->SetTextColor(GREY);
		else pDC->SetTextColor(BLACK);
		pDC->TextOut(5,14*i,pDoc->assembly[pDoc->history[i].IR/4]);
	}
	
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CInstructView diagnostics

#ifdef _DEBUG
void CInstructView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CInstructView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinMIPS64Doc* CInstructView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinMIPS64Doc)));
	return (CWinMIPS64Doc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CInstructView message handlers

void CInstructView::OnUpdate(CView* /* pSender */, LPARAM lHint, CObject* /* pHint */) 
{
	// TODO: Add your specialized code here and/or call the base class
	CPoint ps;
	CWinMIPS64Doc* pDoc=GetDocument();
	CSize sizeTotal;
	WORD32 entries=pDoc->entries;
	if (lHint==2) return;
	sizeTotal.cx=101;
	sizeTotal.cy=14*pDoc->entries+14;
	SetScrollSizes(MM_TEXT,sizeTotal,CSize(100,56),CSize(100,14));

	if (lHint==1L)
	{
		if (entries < first || entries >= first+nlines-1)
		{ 
			ps.x=0;
			ps.y=(entries-nlines+2)*14;
			ScrollToPosition(ps);
		}
	}

	InvalidateRect(NULL);
}
