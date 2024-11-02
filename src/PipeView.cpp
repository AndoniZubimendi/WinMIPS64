// PipeView.cpp : implementation file
//

#include "stdafx.h"
#include "PipeView.h"
#include "WinMIPS64Doc.h"
#include "utils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPipeView

IMPLEMENT_DYNCREATE(CPipeView, CView)

CPipeView::CPipeView()
{
}

CPipeView::~CPipeView()
{
}


BEGIN_MESSAGE_MAP(CPipeView, CView)
	//{{AFX_MSG_MAP(CPipeView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPipeView drawing

void get_mnemonic(CString s, char* txt)
{
	// TODO: 100 is fixed, should be passed as parameter
	strcpy_s(txt, 100, s);
}

void CPipeView::OnDraw(CDC* pDC)
{
	unsigned int i, unit, shift;
	CBrush brush;
	char txt[80];
	CWinMIPS64Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	brush.CreateSolidBrush(YELLOW);
	pDC->SelectObject(brush);
	pDC->Rectangle(CRect(-450, -150, -350, 150));
	pDC->TextOut(-415, 100, "IF");
	pDC->SetTextColor(BLUE);
	pDC->TextOut(-440, 0, pDoc->mnemonic[pDoc->cpu.PC / 4]);
	pDC->SetTextColor(BLACK);
	brush.DeleteObject();
	brush.Detach();
	pDC->MoveTo(-350, 0);
	pDC->LineTo(-300, 0);
	pDC->SelectStockObject(WHITE_BRUSH);
	if (pDoc->pipe.if_id.active)
	{
		brush.CreateSolidBrush(CYAN);
		pDC->SelectObject(brush);
	}

	pDC->Rectangle(CRect(-300, -150, -200, 150));
	brush.DeleteObject();
	brush.Detach();

	pDC->TextOut(-270, 100, "ID");
	if (pDoc->pipe.if_id.active)
	{
		pDC->SetTextColor(BLUE);
		pDC->TextOut(-290, 0, pDoc->mnemonic[pDoc->pipe.if_id.IR / 4]);
		pDC->SetTextColor(BLACK);
	}
	pDC->SelectStockObject(WHITE_BRUSH);
	pDC->MoveTo(-200, 100);
	pDC->LineTo(-50, 550);

	if (pDoc->pipe.integer.active)
	{
		brush.CreateSolidBrush(RED);
		pDC->SelectObject(brush);
	}
	pDC->Rectangle(CRect(-50, 400, 50, 700));
	pDC->TextOut(-20, 640, "EX");
	if (pDoc->pipe.integer.active)
	{
		pDC->SetTextColor(BLUE);
		pDC->TextOut(-40, 520, pDoc->mnemonic[pDoc->pipe.integer.IR / 4]);
		pDC->SetTextColor(BLACK);
	}
	brush.DeleteObject();
	brush.Detach();

	pDC->MoveTo(50, 550);
	pDC->LineTo(200, 100);

	unit = 100 / pDoc->MUL_LATENCY;
	shift = unit / 2;
	pDC->MoveTo(-200, 65);
	pDC->LineTo(-150 + shift, 100);
	for (i = 0; i < pDoc->MUL_LATENCY; i++)
	{
		pDC->SelectStockObject(WHITE_BRUSH);
		if (pDoc->pipe.m[i].active)
		{
			brush.CreateSolidBrush(DCYAN);
			pDC->SelectObject(brush);
		}

		pDC->Rectangle(-150 + (3 * i) * unit + shift, 0, -150 + (2 + 3 * i) * unit + shift, 200);
		pDC->MoveTo(-150 + (2 + (3 * i)) * unit + shift, 100);
		if (i < pDoc->MUL_LATENCY - 1) pDC->LineTo(-150 + (3 + 3 * i) * unit + shift, 100);
		brush.DeleteObject();
		brush.Detach();
	}
	pDC->LineTo(200, 65);
	pDC->TextOut(-50, 300, "Multiplier");

	unit = 100 / pDoc->ADD_LATENCY;
	shift = unit / 2;
	pDC->MoveTo(-200, -65);
	pDC->LineTo(-150 + shift, -150);
	for (i = 0; i < pDoc->ADD_LATENCY; i++)
	{
		pDC->SelectStockObject(WHITE_BRUSH);
		if (pDoc->pipe.a[i].active)
		{
			brush.CreateSolidBrush(DGREEN);
			pDC->SelectObject(brush);
		}

		pDC->Rectangle(-150 + 3 * i * unit + shift, -250, -150 + (2 + 3 * i) * unit + shift, -50);
		pDC->MoveTo(-150 + (2 + 3 * i) * unit + shift, -150);
		if (i < pDoc->ADD_LATENCY - 1) pDC->LineTo(-150 + (3 + 3 * i) * unit + shift, -150);
		brush.DeleteObject();
		brush.Detach();
	}
	pDC->LineTo(200, -65);
	pDC->TextOut(-75, -280, "FP Adder");

	pDC->MoveTo(-200, -100);
	pDC->LineTo(-100, -550);
	pDC->SelectStockObject(WHITE_BRUSH);
	if (pDoc->pipe.div.active)
	{
		brush.CreateSolidBrush(DYELLOW);
		pDC->SelectObject(brush);
	}
	pDC->Rectangle(CRect(-100, -400, 100, -700));
	sprintf_s(txt, 80, "  DIV  %d", pDoc->pipe.div.cycles);
	pDC->TextOut(-60, -500, txt);
	brush.DeleteObject();
	brush.Detach();
	pDC->MoveTo(100, -550);
	pDC->LineTo(200, -100);
	pDC->SelectStockObject(WHITE_BRUSH);
	if (pDoc->pipe.ex_mem.active)
	{
		brush.CreateSolidBrush(GREEN);
		pDC->SelectObject(brush);
	}
	pDC->Rectangle(CRect(200, -150, 300, 150));
	pDC->TextOut(220, 100, "MEM");
	if (pDoc->pipe.ex_mem.active)
	{
		pDC->SetTextColor(BLUE);
		pDC->TextOut(210, 0, pDoc->mnemonic[pDoc->pipe.ex_mem.IR / 4]);
		pDC->SetTextColor(BLACK);
	}

	brush.DeleteObject();
	brush.Detach();
	pDC->MoveTo(300, 0);
	pDC->LineTo(350, 0);
	pDC->SelectStockObject(WHITE_BRUSH);
	if (pDoc->pipe.mem_wb.active)
	{
		brush.CreateSolidBrush(MAGENTA);
		pDC->SelectObject(brush);
	}
	pDC->Rectangle(CRect(350, -150, 450, 150));
	pDC->TextOut(375, 100, "WB");
	if (pDoc->pipe.mem_wb.active)
	{
		pDC->SetTextColor(BLUE);
		pDC->TextOut(360, 0, pDoc->mnemonic[pDoc->pipe.mem_wb.IR / 4]);
		pDC->SetTextColor(BLACK);
	}

	pDC->SelectStockObject(WHITE_BRUSH);
	brush.DeleteObject();
	brush.Detach();
}

/////////////////////////////////////////////////////////////////////////////
// CPipeView diagnostics

#ifdef _DEBUG
void CPipeView::AssertValid() const
{
	CView::AssertValid();
}

void CPipeView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWinMIPS64Doc* CPipeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinMIPS64Doc)));
	return (CWinMIPS64Doc*)m_pDocument;
}
#endif //_DEBUG


/////////////////////////////////////////////////////////////////////////////
// CPipeView message handlers

void CPipeView::OnPrepareDC(CDC* pDC, CPrintInfo* /* pInfo */)
{
	CRect rectClient;
	// TODO: Add your specialized code here and/or call the base class

//	CView::OnPrepareDC(pDC, pInfo);

	GetClientRect(rectClient);
	pDC->SetMapMode(MM_ANISOTROPIC);
	pDC->SetWindowExt(1000, 1000);
	pDC->SetViewportExt(rectClient.right, -rectClient.bottom / 2);
	pDC->SetViewportOrg(rectClient.right / 2, rectClient.bottom / 2);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&font);
}

void CPipeView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	font.CreateFont(80, 12, 0, 0, 500, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, "Arial");

	// TODO: Add your specialized code here and/or call the base class

}

void CPipeView::OnUpdate(CView* /* pSender */, LPARAM lHint, CObject* /* pHint */)
{
	if (lHint == 2) return;
	InvalidateRect(NULL);


}
