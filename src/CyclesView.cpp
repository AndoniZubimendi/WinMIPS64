// CyclesView.cpp : implementation file
//

#include "stdafx.h"
#include "winEVE.h"
#include "CyclesView.h"
#include "WinEVEDoc.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCyclesView

IMPLEMENT_DYNCREATE(CCyclesView, CScrollView)

CCyclesView::CCyclesView()
{
}

CCyclesView::~CCyclesView()
{
}


BEGIN_MESSAGE_MAP(CCyclesView, CScrollView)
	//{{AFX_MSG_MAP(CCyclesView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCyclesView drawing

void CCyclesView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	yellow.CreateSolidBrush(YELLOW);
	cyan.CreateSolidBrush(CYAN);
	red.CreateSolidBrush(RED);
	green.CreateSolidBrush(GREEN);
	magenta.CreateSolidBrush(MAGENTA);
	dgreen.CreateSolidBrush(DGREEN);
	dyellow.CreateSolidBrush(DYELLOW);
	dcyan.CreateSolidBrush(DCYAN);

	font.CreateFont(12,0,0,0,400,FALSE,FALSE,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH| FF_SWISS,"Arial");
}

void CCyclesView::OnDraw(CDC* pDC)
{
	int stage,substage,cause,last_stage,last_substage;
	unsigned int i,j,k,very_start,my_start,tx,ty;
	
	CWinMIPS64Doc *pDoc = GetDocument();

	char txt[10];

	very_start=pDoc->history[0].start_cycle;

	for (i=0;i<pDoc->entries;i++)
	{
		last_stage=last_substage=0;
		my_start=pDoc->history[i].start_cycle;
		for (j=0;j<pDoc->cycles-my_start+1;j++)
		{
			stage=pDoc->history[i].status[j].stage;
			substage=pDoc->history[i].status[j].substage;
			cause=pDoc->history[i].status[j].cause;
			k=j+my_start-very_start;
			CRect box(32*k,14*i,32*(k+1),14*i+12);
			tx=32*k+5;
			ty=14*i+1;
			switch (stage)
			{
			case IFETCH:
				pDC->SelectObject(yellow);
				pDC->Rectangle(box);
				if (stage!=last_stage) pDC->TextOut(tx,ty,"IF");
				break;
			case IDECODE:
				pDC->SelectObject(cyan);
				pDC->Rectangle(box);
				if (stage!=last_stage) pDC->TextOut(tx,ty,"ID");
				break;
			case INTEX:
				pDC->SelectObject(red);
				pDC->Rectangle(box);
				if (stage!=last_stage) pDC->TextOut(tx,ty,"EX");
			break;
			case MULEX:
				pDC->SelectObject(dcyan);
				pDC->Rectangle(box);
				sprintf_s(txt,10,"M%d",substage);
				if (stage!=last_stage || substage!=last_substage) 
					pDC->TextOut(tx,ty,txt);
				break;
			case ADDEX:
				pDC->SelectObject(dgreen);
				pDC->Rectangle(box);
				sprintf_s(txt,10,"A%d",substage);
				if (stage!=last_stage || substage!=last_substage) 
					pDC->TextOut(tx,ty,txt);
				break;
			case DIVEX:
				pDC->SelectObject(dyellow);
				pDC->Rectangle(box);
				if (stage!=last_stage) pDC->TextOut(tx,ty,"DIV");
				break;
			case MEMORY:
				pDC->SelectObject(green);
				pDC->Rectangle(box);
				if (stage!=last_stage) pDC->TextOut(tx,ty,"Mem");
				break;
			case WRITEB:
				pDC->SelectObject(magenta);
				pDC->Rectangle(box);
				pDC->TextOut(tx,ty,"WB");
				break;
			}
			pDC->SelectStockObject(WHITE_BRUSH);

			last_stage=stage;
			last_substage=substage;
			if (cause==RAW) pDC->TextOut(tx,ty,"Raw");
			if (cause==WAW) pDC->TextOut(tx,ty,"Waw");
			if (cause==WAR) pDC->TextOut(tx,ty,"War");
			if (cause==STRUCTURAL) pDC->TextOut(tx,ty,"Str");
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCyclesView diagnostics

#ifdef _DEBUG
void CCyclesView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CCyclesView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinMIPS64Doc* CCyclesView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinMIPS64Doc)));
	return (CWinMIPS64Doc*)m_pDocument;
}

#endif //_DEBUG



/////////////////////////////////////////////////////////////////////////////
// CCyclesView message handlers

void CCyclesView::OnUpdate(CView* /* pSender */, LPARAM lHint, CObject* /* pHint */) 
{
	CPoint ps;
	CWinMIPS64Doc* pDoc=GetDocument();
	CSize sizeTotal;
	WORD32 entries=pDoc->entries;
	WORD32 start;
	WORD32 width;
	CRect sz;
	int len;

	if (lHint==2) return;	

	GetClientRect(&sz);
	start=pDoc->history[0].start_cycle;
	width=pDoc->cycles-start;

	len=32*width+32;
	if (len>sz.Width()) sizeTotal.cx=len;
	else sizeTotal.cx=sz.Width()+1;

	len=14*entries+14;
	if (len>sz.Height()) sizeTotal.cy=len;
	else sizeTotal.cy=sz.Height()+1;

	SetScrollSizes(MM_TEXT,sizeTotal,CSize(128,56),CSize(32,14));

	if (lHint==1L)
	{
		BOOL change=FALSE;
		ps=GetScrollPosition();

		srow=ps.y/14;
		scol=ps.x/32;

		CRect sz;
		GetClientRect(&sz);
		nrow=1+sz.Height()/14;
		ncol=1+sz.Width()/32;

		if (width < scol || width>=scol+ncol-1)
		{ 
			ps.x=(width-ncol+2)*32;
			change=TRUE;
		}
		if (entries < srow || entries >= srow+nrow-1)
		{ 
			change=TRUE;
			ps.y=(entries-nrow+2)*14;
		}
		if (change) ScrollToPosition(ps);
	}


	InvalidateRect(NULL);
}

void CCyclesView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&font);


	CScrollView::OnPrepareDC(pDC, pInfo);
}
