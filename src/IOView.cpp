// IOView.cpp : implementation file
//

#include "stdafx.h"
#include "wineve.h"
#include "IOView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIOView

IMPLEMENT_DYNCREATE(CIOView, CScrollView)

CIOView::CIOView() :  m_ViewCharSize(0,0)
{
   m_pFont = NULL;
   m_pPen = NULL;
   nlines=0;
   line="";
   caretcount=0;
}

CIOView::~CIOView()
{
}


BEGIN_MESSAGE_MAP(CIOView, CScrollView)
	//{{AFX_MSG_MAP(CIOView)
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIOView drawing


void CIOView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

//	CSize sizeTotal;
	// TODO: calculate the total size of this view
//	sizeTotal.cx = sizeTotal.cy = 32;
//	SetScrollSizes(MM_TEXT, sizeTotal);
	ComputeViewMetrics();
	
//	font.CreateFont(15,0,0,0,400,FALSE,FALSE,0,
//					ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,
//					DEFAULT_QUALITY,DEFAULT_PITCH|FF_MODERN,"Courier New");
}

CFont*	CIOView::GetFont()
{
	if (m_pFont == NULL)
	{  
		m_pFont = new CFont;
		if(m_pFont)
		{
			m_pFont->CreatePointFont(90, "Courier New");
		}
	}
	return m_pFont;
}

CPen*	CIOView::GetPen()
{
	if (m_pPen == NULL)
	{  
		m_pPen = new CPen;
		if(m_pPen)
		{
			m_pPen->CreatePen(PS_SOLID, 1, LGREY);
		}
	}
	return m_pPen;
}


void CIOView::ComputeViewMetrics()
{	 
	CDC* pDC = CDC::FromHandle(::GetDC(NULL));  
	int nSaveDC = pDC->SaveDC();      
	pDC->SetMapMode(MM_TEXT);     
	CFont* pPreviousFont = pDC->SelectObject(GetFont());  
	TEXTMETRIC tm;  
	pDC->GetTextMetrics(&tm);   
	m_ViewCharSize.cy = tm.tmHeight + tm.tmExternalLeading;  
	m_ViewCharSize.cx = tm.tmAveCharWidth;   
	pDC->LPtoDP(&m_ViewCharSize);  
/*
	CTextDoc* pDoc = GetDocument();  
	m_DocSize.cy = m_ViewCharSize.cy * pDoc->GetLineList()->GetCount();
    
	CString	Line;  
	CSize size;  
	POSITION pos = pDoc->GetLineList()->GetHeadPosition();  
	while( pos != NULL )  
	{	
		Line = pDoc->GetLineList()->GetNext( pos );	
		size = pDC->GetTextExtent(Line, Line.GetLength());	
		m_DocSize.cx = max(size.cx, m_DocSize.cx);  
	}  
	//	Account for our simple margin  
	m_DocSize.cx += 4 * m_ViewCharSize.cx;    
	// clean up  
*/
	if(pPreviousFont)  
	{	
		pDC->SelectObject(pPreviousFont);
	}  
	pDC->RestoreDC(nSaveDC);
	::ReleaseDC(NULL,pDC->GetSafeHdc());
}



void CIOView::OnDraw(CDC* pDC)
{
	CWinEVEDoc* pDoc = GetDocument();
	int cursor_y;
	CSize CharSize = GetCharSize();
	CFont* pPreviousFont = pDC->SelectObject(GetFont());
	CPen* pPreviousPen= pDC->SelectObject(GetPen());
	CString line;
	CString string=pDoc->cpu.Terminal;
	CPoint ps;
	int i,beg,cr,x,y;
    int size=GSXY*8;
	int x_off=200;
	int y_off=10;
	
//	BOOL drawit=FALSE;

//pDoc->cpu.screen[871]=YELLOW;

//	for (i=0;i<1024;i++) if (pDoc->cpu.screen[i]!=WHITE) {drawit=TRUE;break;} 

	if (pDoc->cpu.drawit)
	{
		pDC->Rectangle(x_off, y_off, x_off+size+1, y_off+size+1);

		for (i=0;i<GSXY*8;i+=8)
		{
			pDC->MoveTo(x_off,y_off+i);
			pDC->LineTo(x_off+size,y_off+i);
		}
		for (i=0;i<GSXY*8;i+=8)
		{
			pDC->MoveTo(x_off+i,y_off);
			pDC->LineTo(x_off+i,y_off+size);
		}

		for (i=0;i<GSXY*GSXY;i++)
			if (pDoc->cpu.screen[i]!=WHITE)
			{
				CBrush brush(pDoc->cpu.screen[i]);
				x=i%GSXY; y=i/GSXY; y=GSXY-1-y;
				pDC->SelectObject(brush);
				pDC->Rectangle(x_off+8*x,y_off+8*y,x_off+8*x+9,y_off+8*y+9);
				pDC->SelectStockObject(WHITE_BRUSH);
			}
	}


	beg=0; cursor_y=0;
	for(;;)
	{
		cr=string.Find('\n',beg);
		if (cr<0)
		{
			line=string.Mid(beg,string.GetLength());
			pDC->TextOut(0,cursor_y,line);
			break;
		}

		line=string.Mid(beg,cr-beg);
		pDC->TextOut(0,cursor_y,line);

		beg=cr+1;
		cursor_y+=CharSize.cy;
	}
 
	if(pPreviousFont)  
	{	
		pDC->SelectObject(pPreviousFont);  
	}

	if(pPreviousPen)  
	{	
		pDC->SelectObject(pPreviousPen);  
	}

	// Scroll if no graphics displayed
	
	if (!pDoc->cpu.drawit && pDoc->cpu.nlines!=nlines)
	{
		BOOL change;
		int rows,start_row,nl=pDoc->cpu.nlines;
		ps=GetScrollPosition();

		start_row=ps.y/CharSize.cy;
		CRect sz;
		GetClientRect(&sz);
		rows=sz.Height()/CharSize.cy;
		change=FALSE;
		if (nl>start_row+rows)
		{
			change=TRUE;
			ps.y=(nl-rows)*CharSize.cy;
						
		}
		if (change) ScrollToPosition(ps);
	}

	nlines=pDoc->cpu.nlines;

	if (pDoc->cpu.keyboard==1)
	{
		ps.x=(pDoc->cpu.ncols)*CharSize.cx;
		ps.y=(pDoc->cpu.nlines+1)*CharSize.cy; 
		SetCaretPos (ps);
		
		if (caretcount==0) 
		{
			ShowCaret();
			caretcount=1;
		}

//		char txt[80];
//		sprintf(txt,"%d",caretcount);
//		AfxMessageBox(txt);	

	}
//	else HideCaret();

}

/////////////////////////////////////////////////////////////////////////////
// CIOView diagnostics

#ifdef _DEBUG
void CIOView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CIOView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CWinEVEDoc* CIOView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWinEVEDoc)));
	return (CWinEVEDoc*)m_pDocument;
}


#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CIOView message handlers

void CIOView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
//	CRect rectClient;
	// TODO: Add your specialized code here and/or call the base class
	
//	CView::OnPrepareDC(pDC, pInfo);

//	GetClientRect(rectClient);
//	pDC->SetMapMode(MM_ANISOTROPIC);
//	pDC->SetWindowExt(1000,1000);
//	pDC->SetViewportExt(rectClient.right,-rectClient.bottom/2);
//	pDC->SetViewportOrg(rectClient.right/2,rectClient.bottom/2);
//	pDC->SetBkMode(TRANSPARENT);
//	pDC->SelectObject(&font);	
	CScrollView::OnPrepareDC(pDC, pInfo);
}

void CIOView::OnUpdate(CView* /* pSender */, LPARAM /* lHint */, CObject* /* pHint */) 
{
	// TODO: Add your specialized code here and/or call the base class
	CWinEVEDoc* pDoc = GetDocument();
	CSize sizeTotal;
	CSize CharSize = GetCharSize();

	sizeTotal.cx = 80*CharSize.cx;
	sizeTotal.cy = (pDoc->cpu.nlines+1)*CharSize.cy;
	SetScrollSizes(MM_TEXT, sizeTotal,CSize(100,56),CSize(100,14));

	if (pDoc->cpu.keyboard==0 && caretcount==1) 
	{
		HideCaret();
		caretcount=0;
	}

	InvalidateRect(NULL);
	
}

void CIOView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

//	char txt[20];
	CWinEVEDoc* pDoc = GetDocument();
	CSize CharSize=GetCharSize();
	CPoint point;
	DOUBLE64 number;
	CMainFrame* pFrame=(CMainFrame*) AfxGetApp()->m_pMainWnd;
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;
	
	if (pDoc->cpu.keyboard==2)
	{ // just take one character, do not echo it.

		pDoc->cpu.mm[8] = (BYTE) nChar;
		pDoc->cpu.keyboard=0;
		pStatus->SetPaneText(0,"Ready");
		if (pDoc->restart) OnCommand(ID_EXECUTE_RUNTO,0);
	}

	if (pDoc->cpu.keyboard==1)
	{
//		char txt[80];
//		sprintf(txt,"%d",nChar);
//		AfxMessageBox(txt);
	
		point.x=(pDoc->cpu.ncols+1)*CharSize.cx;
		point.y=(pDoc->cpu.nlines+1)*CharSize.cy;
   
		SetCaretPos (point);

		if (nChar==13)
		{
			if (caretcount==1)
			{
				HideCaret();
				caretcount=0;
			}
			if (line.Find('.')>=0)
				number.d=atof(line);
			else
				number.s=_atoi64(line);
			*(WORD64 *)&(pDoc->cpu.mm[8]) = number.u; 
			line="";
			pDoc->cpu.Terminal+='\n';
			pDoc->cpu.keyboard=0;
			pStatus->SetPaneText(0,"Ready");
			if (pDoc->restart) OnCommand(ID_EXECUTE_RUNTO,0);
		}
		else 
		{

			if (nChar==8) 
			{
				if (line.GetLength()>0)
				{
					line.Delete(line.GetLength()-1);
					pDoc->cpu.Terminal.Delete(pDoc->cpu.Terminal.GetLength()-1);
					pDoc->cpu.ncols--;
				}

			}
			else
			{
				line+= (BYTE) nChar;
				pDoc->cpu.Terminal+= (BYTE) nChar;
				pDoc->cpu.ncols++;
			}
			if (caretcount==1)
			{
				HideCaret();
				caretcount=0;
			}

			InvalidateRect(NULL);
		}
	}

	CScrollView::OnChar(nChar, nRepCnt, nFlags);
}

void CIOView::OnSetFocus(CWnd* pOldWnd) 
{
	CWinEVEDoc* pDoc = GetDocument();
	CScrollView::OnSetFocus(pOldWnd);
	CreateSolidCaret (10, 2);
	caretcount=0;
	if (pDoc->cpu.keyboard==1)
	{
		ShowCaret();
		caretcount=1;
	}
}
