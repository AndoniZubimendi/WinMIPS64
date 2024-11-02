#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPipeView view

#include "WinMIPS64Doc.h"

class CPipeView : public CView
{
protected:
	CPipeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPipeView)

	// Attributes
public:
	CWinMIPS64Doc* GetDocument();
	CFont font;
	// Operations
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CPipeView)
public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	virtual void OnInitialUpdate();
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPipeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPipeView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in PipeView.cpp
inline CWinMIPS64Doc* CPipeView::GetDocument()
{
	return (CWinMIPS64Doc*)m_pDocument;
}
#endif
