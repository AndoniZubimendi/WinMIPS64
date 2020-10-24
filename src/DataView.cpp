// DataView.cpp : implementation file
//

#include "stdafx.h"
#include "WinEVE.h"
#include "DataView.h"
#include "WinEVEDoc.h"
#include "utils.h"
#include "DataDialog.h"
#include "FPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataView

IMPLEMENT_DYNCREATE(CDataView, CScrollView)

CDataView::CDataView()
{

}

CDataView::~CDataView()
{
}


BEGIN_MESSAGE_MAP(CDataView, CScrollView)
	//{{AFX_MSG_MAP(CDataView)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataView drawing


void CDataView::OnDraw(CDC* pDC)
{
	unsigned int i,j,pos;
	WORD64 bytes;

	CWinEVEDoc* pDoc = GetDocument();
	char txt[400];
	const char *linetext;
	
	pDC->SelectObject(&font);

	for (i=0;i<pDoc->DATASIZE;i+=STEP)
	{
		bytes=pack(&pDoc->cpu.data[i]);
		linetext=pDoc->datalines[i/STEP];
		pos=2*sprintnbits(txt,i,bits(pDoc->DATASIZE));
	
		for (j=pos;j<pos+2;j++) txt[j]=' ';
		sprintword(&txt[pos+2],bytes);
		sprintf(&txt[pos+2+2*STEP]," %s",linetext);

		pos=(14*i)/STEP;

		pDC->TextOut(0,pos,txt);
	}
	
	CPoint pt;
	pt=GetScrollPosition();

//	if (pt.y>32000)
//	{
//		sprintf(txt,"pt.y= %d",pt.y);
//		AfxMessageBox(txt);
//	}

	first=pt.y/14;
	if (pt.y%14 != 0)
	{
		pt.y=14*first;
		ScrollToPosition(pt);
	}

}

/////////////////////////////////////////////////////////////////////////////
// CDataView diagnostics

#ifdef _DEBUG
void CDataView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDataView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinEVEDoc* CDataView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinEVEDoc)));
	return (CWinEVEDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDataView message handlers

void CDataView::OnInitialUpdate() 
{
	CScrollView::OnInitialUpdate();
	CWinEVEDoc* pDoc=GetDocument();

	font.CreateFont(15,0,0,0,400,FALSE,FALSE,0,
					ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,"Courier New");		
}

void CDataView::OnUpdate(CView* /* pSender */, LPARAM lHint, CObject* /* pHint */) 
{	
	CWinEVEDoc* pDoc=GetDocument();
	CSize sizeTotal;

	if (lHint==2) return;

	// TODO: calculate the total size of this view
	sizeTotal.cx = 400;
	sizeTotal.cy = 14*pDoc->DATASIZE/STEP+14;
	
	SetScrollSizes(MM_TEXT, sizeTotal,CSize(100,56),CSize(100,14));
	InvalidateRect(NULL);	
}

void CDataView::OnLButtonDblClk(UINT /* nFlags */, CPoint point) 
{
	CDataDialog dlg;
	unsigned int addr=first+point.y/14;
	CWinEVEDoc *pDoc=GetDocument();

	char txt[20];
	if (addr*STEP>=pDoc->DATASIZE) return;
	sprintword(txt,pack(&pDoc->cpu.data[addr*STEP]));

	dlg.m_contents=txt;
//	sprintword32(txt,STEP*addr);
	sprintnbits(txt,STEP*addr,bits(pDoc->DATASIZE));	
	dlg.m_addr=txt;
	if (dlg.DoModal()!=IDOK) return;
	strcpy(txt,LPCTSTR(dlg.m_contents));

	unpack(strtoint64(txt,NULL,16),&pDoc->cpu.data[addr*STEP]);

// mark memory as initialised

	for (int i=0;i<STEP;i++) pDoc->cpu.dstat[addr*STEP+i]=WRITTEN;

	OnUpdate(NULL,0,NULL);
}

void CDataView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

void CDataView::OnRButtonDblClk(UINT /* nFlags */, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CFPDialog dlg;
	DOUBLE64 db;
	double t;
	unsigned int addr=first+point.y/14;
	CWinEVEDoc *pDoc=GetDocument();

	char txt[1000];
	char txt1[1000];
	if (addr*STEP>=pDoc->DATASIZE) return;
	db.u=pack(&pDoc->cpu.data[addr*STEP]);
	sprintdouble(txt1,db.d);

//	AfxMessageBox(txt);

	t=atof(txt1);
	if (db.u!=0 && t==0.0) return;  // its not FP

	dlg.m_contents=txt1;
//	sprintword32(txt,STEP*addr);
	sprintnbits(txt,STEP*addr,bits(pDoc->DATASIZE));	
	dlg.m_addr=txt;
	if (dlg.DoModal()!=IDOK) return;
	if (strcmp(txt1,LPCTSTR(dlg.m_contents))!=0)
	{
		strcpy(txt,LPCTSTR(dlg.m_contents));
		db.d=atof(txt);
		unpack(db.u,&pDoc->cpu.data[addr*STEP]);
	}

// mark memory as initialised

	for (int i=0;i<STEP;i++) pDoc->cpu.dstat[addr*STEP+i]=WRITTEN;

	OnUpdate(NULL,0,NULL);
}
