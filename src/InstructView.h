#pragma once

/////////////////////////////////////////////////////////////////////////////
// CInstructView view

#include "WinMIPS64Doc.h"

class CInstructView : public CScrollView
{
protected:
	CInstructView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CInstructView)

// Attributes
public:
	CWinMIPS64Doc* GetDocument();
	unsigned int first;
	unsigned int nlines;
	CFont font;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInstructView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CInstructView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CInstructView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in InstructView.cpp
inline CWinMIPS64Doc* CInstructView::GetDocument()
   { return (CWinMIPS64Doc*)m_pDocument; }
#endif
