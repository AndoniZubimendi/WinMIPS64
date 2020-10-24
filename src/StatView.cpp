// StatView.cpp : implementation file
//

#include "stdafx.h"
#include "WinEVE.h"
#include "StatView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatView

IMPLEMENT_DYNCREATE(CStatView, CScrollView)

CStatView::CStatView()
{
}

CStatView::~CStatView()
{
}


BEGIN_MESSAGE_MAP(CStatView, CScrollView)
	//{{AFX_MSG_MAP(CStatView)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatView drawing

void CStatView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 250;
	sizeTotal.cy = 300;
	SetScrollSizes(MM_TEXT, sizeTotal);

	font.CreateFont(15,0,0,0,400,FALSE,FALSE,0,
					ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
					DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,"Courier New");
}

void CStatView::OnDraw(CDC* pDC)
{
	CWinEVEDoc* pDoc = GetDocument();
	
	char txt[100];

	pDC->SelectObject(&font);

	pDC->SetTextColor(RGB(255,0,0));
	pDC->TextOut(0,0," Ejecución");
	pDC->SetTextColor(RGB(0,0,0));

	if (pDoc->cycles==1)
		sprintf(txt," 1 Ciclo");
	else
		sprintf(txt," %d Ciclos",pDoc->cycles);
	pDC->TextOut(0,20,txt);

	if (pDoc->instructions==1)
		sprintf(txt," 1 Instrucción");
	else
		sprintf(txt," %d Instrucciones",pDoc->instructions);
	pDC->TextOut(0,34,txt);

	if (pDoc->instructions>0) 
	{
		sprintf(txt," %3.3f Ciclos Por Instrucción (CPI)",(double)pDoc->cycles/pDoc->instructions);
		pDC->TextOut(0,48,txt);
	}

	pDC->SetTextColor(RGB(255,0,0));
/*
	pDC->TextOut(0,70," Instruction type");
	pDC->SetTextColor(RGB(0,0,0));

	sprintf(txt," %d Loads",pDoc->loads);
	pDC->TextOut(0,90,txt);

	sprintf(txt," %d Stores",pDoc->stores);
	pDC->TextOut(0,104,txt);

	sprintf(txt," %d Branches taken",pDoc->branches_taken);
	pDC->TextOut(0,120,txt);

	sprintf(txt," %d Branches not taken",pDoc->branches_not_taken);
	pDC->TextOut(0,134,txt);

	sprintf(txt," %d Branches Total",pDoc->branches_not_taken+pDoc->branches_taken);
	pDC->TextOut(0,148,txt);
*/
	pDC->SetTextColor(RGB(255,0,0));
	pDC->TextOut(0,90," Atascos");
	pDC->SetTextColor(RGB(0,0,0));

	if (pDoc->raw_stalls==1)
		sprintf(txt," 1 Atasco RAW");
	else
		sprintf(txt," %d Atascos RAW ",pDoc->raw_stalls);
	pDC->TextOut(0,110,txt);

	if(pDoc->waw_stalls==1)
		sprintf(txt," 1 Atasco WAW");
	else
		sprintf(txt," %d Atascos WAW",pDoc->waw_stalls);
	pDC->TextOut(0,124,txt);

	if (pDoc->war_stalls==1)
		sprintf(txt," 1 Atasco WAR");
	else
		sprintf(txt," %d Atascos WAR",pDoc->war_stalls);
	pDC->TextOut(0,138,txt);

	if (pDoc->structural_stalls==1)
		sprintf(txt," 1 Atasco Estructural");
	else
		sprintf(txt," %d Atascos Estructurales",pDoc->structural_stalls);
	pDC->TextOut(0,152,txt);

	if (pDoc->branch_taken_stalls==1)
		sprintf(txt," 1 Atasco Branch Taken");
	else
		sprintf(txt," %d Atascos Branch Taken",pDoc->branch_taken_stalls);
	pDC->TextOut(0,166,txt);

	if (pDoc->branch_misprediction_stalls==1)
		sprintf(txt," 1 Atasco Branch Misprediction ");
	else 
		sprintf(txt," %d Atascos Branch Misprediction",pDoc->branch_misprediction_stalls);
	pDC->TextOut(0,180,txt);


	pDC->SetTextColor(RGB(255,0,0));
	pDC->TextOut(0,222," Tamaño del Código");
	pDC->SetTextColor(RGB(0,0,0));

	sprintf(txt," %d Bytes",pDoc->codeptr);
	pDC->TextOut(0,242,txt);

}

/////////////////////////////////////////////////////////////////////////////
// CStatView diagnostics

#ifdef _DEBUG
void CStatView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CStatView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinEVEDoc* CStatView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinEVEDoc)));
	return (CWinEVEDoc*)m_pDocument;
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CStatView message handlers

void CStatView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
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

void CStatView::OnUpdate(CView* /* pSender */, LPARAM lHint, CObject* /* pHint */) 
{
	if (lHint==2) return;
	InvalidateRect(NULL);
	
	
}
