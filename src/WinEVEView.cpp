// WinEVEView.cpp : implementation of the CWinEVEView class
//

#include "stdafx.h"
#include "WinEVE.h"

#include "WinEVEDoc.h"
#include "WinEVEView.h"
#include "MainFrm.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CWinEVEView

IMPLEMENT_DYNCREATE(CWinEVEView, CScrollView)

BEGIN_MESSAGE_MAP(CWinEVEView, CScrollView)
	//{{AFX_MSG_MAP(CWinEVEView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinEVEView construction/destruction

CWinEVEView::CWinEVEView()
{
	// TODO: add construction code here

}

CWinEVEView::~CWinEVEView()
{
}

BOOL CWinEVEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWinEVEView drawing

void CWinEVEView::OnDraw(CDC* pDC)
{
	unsigned int i,j,pos;
	WORD32 fourbytes;
	CWinEVEDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	const char *linetext;
	char txt[400];
//AfxMessageBox("Drawing");
	pDC->SelectObject(&font);

// Which lines are visible?
	CPoint ps;
	ps=GetScrollPosition();
	first=ps.y/14;

	// get size of View area
	
	CRect sz;
	GetClientRect(&sz);

	nlines=1+sz.Height()/14;

	for (i=0;i<pDoc->CODESIZE;i+=4)
	{
		pDC->SetBkColor(WHITE);
		if (i==pDoc->cpu.PC)	pDC->SetBkColor(YELLOW);
		if (pDoc->pipe.if_id.active && i==pDoc->pipe.if_id.IR)  
								pDC->SetBkColor(CYAN);
		if (pDoc->pipe.integer.active && i==pDoc->pipe.integer.IR)  
								pDC->SetBkColor(RED);
		if (pDoc->pipe.ex_mem.active && i==pDoc->pipe.ex_mem.IR) 
								pDC->SetBkColor(GREEN);
		if (pDoc->pipe.mem_wb.active && i==pDoc->pipe.mem_wb.IR) 
								pDC->SetBkColor(MAGENTA);
		for (j=0;j<pDoc->ADD_LATENCY;j++)
			if (pDoc->pipe.a[j].active && i==pDoc->pipe.a[j].IR) 
								pDC->SetBkColor(DGREEN);
		for (j=0;j<pDoc->MUL_LATENCY;j++)
			if (pDoc->pipe.m[j].active && i==pDoc->pipe.m[j].IR) 
								pDC->SetBkColor(DCYAN);
		if (pDoc->pipe.div.active && i==pDoc->pipe.div.IR)
								pDC->SetBkColor(DYELLOW);

		fourbytes=pack32(&pDoc->cpu.code[i]);
		linetext=LPCTSTR(pDoc->codelines[i/4]);

		pos=2*sprintnbits(txt,i,bits(pDoc->CODESIZE));


		if ((pDoc->cpu.cstat[i])==0) 
		{
			sprintf(&txt[pos]," %08x %s",fourbytes,linetext);
			pDC->TextOut(0,14*i/4,txt);
		}
		else 
		{
			if ((pDoc->cpu.cstat[i]&2)!=0)
				sprintf(&txt[pos],"%c%08x %s",171,fourbytes,linetext);
			else
			     sprintf(&txt[pos]," %08x %s",fourbytes,linetext);                    
			if ((pDoc->cpu.cstat[i]&1)!=0)
				pDC->SetTextColor(BLUE);
			else
			{
				if ((pDoc->cpu.cstat[i]&4)!=0)
					pDC->SetTextColor(RED);
				else
					pDC->SetTextColor(BLACK);
			}
			pDC->TextOut(0,14*i/4,txt);
			pDC->SetTextColor(BLACK);
		}

		
	}
	if (ps.y%14 != 0)
	{
		ps.y=14*(ps.y/14);
		ScrollToPosition(ps);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CWinEVEView diagnostics

#ifdef _DEBUG
void CWinEVEView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CWinEVEView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinEVEDoc* CWinEVEView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinEVEDoc)));
	return (CWinEVEDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWinEVEView message handlers

void CWinEVEView::OnInitialUpdate() 
{
	CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;

	CScrollView::OnInitialUpdate();

	CWinEVEDoc* pDoc=GetDocument();

//	AfxGetMainWnd()->SetWindowText("Hello mike");

	CSize sizeTotal;
	// TODO: calculate the total size of this view

	font.CreateFont(15,0,0,0,500,FALSE,FALSE,0,
					ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,"Courier New");	

 }	

void CWinEVEView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CWinEVEDoc* pDoc=GetDocument();
	WORD32 pcline=pDoc->cpu.PC/4;
	CPoint ps;

	CSize sizeTotal;
	
	if (lHint==2) return;

	sizeTotal.cx = 400;
	sizeTotal.cy = 14*pDoc->CODESIZE/4+14;
	SetScrollSizes(MM_TEXT, sizeTotal,CSize(100,56),CSize(100,14));


	ps=GetScrollPosition();

	// Is PC visible?
	if (lHint==1L)
	{
		if (pcline < first || pcline >= first+nlines-1)
		{ // PC is not visible, so scroll window
			ps.x=0;
			ps.y=(pcline-nlines+2)*14;
			ScrollToPosition(ps);
		}
	}
	else
	{
		if (ps.y%14 != 0)
		{
			ps.y=14*(ps.y/14);
			ScrollToPosition(ps);
		}
	}

	InvalidateRect(NULL);
	
}

void CWinEVEView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CWinEVEDoc* pDoc=GetDocument();

// set/reset a break-point
// Find which Instruction from Mouse co-ordinates
	CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;

	int bp=4*(first+point.y/14);

	if ((pDoc->cpu.cstat[bp]&1)==0)
	{
		pStatus->SetPaneText(0,"Setting Break-Point");
		pDoc->cpu.cstat[bp]|=1;
	}
	else
	{
		pStatus->SetPaneText(0,"Clearing Break-Point");
		pDoc->cpu.cstat[bp]&=0xfe;
	}
	OnUpdate(this,0L,NULL);
}

void CWinEVEView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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
