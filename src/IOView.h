#pragma once

/////////////////////////////////////////////////////////////////////////////
// CIOView view

#include "WinMIPS64Doc.h"

#define GSXY 50

class CIOView : public CScrollView
{
protected:
	CIOView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CIOView)
	CSize 	m_ViewCharSize;
	CFont*	m_pFont;
	CPen* m_pPen;
	WORD32 nlines;
	CString line;
	WORD32 caretcount;

// Attributes
public:
	CWinMIPS64Doc* GetDocument();
// Operations
public:
	CFont*	  GetFont();
	CPen*	  GetPen();
	CSize GetCharSize() const { return m_ViewCharSize; }
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIOView)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void ComputeViewMetrics();
	virtual ~CIOView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CIOView)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in IOView.cpp
inline CWinMIPS64Doc* CIOView::GetDocument()
   { return (CWinMIPS64Doc*)m_pDocument; }
#endif
