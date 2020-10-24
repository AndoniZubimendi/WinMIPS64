#if !defined(AFX_PIPEVIEW_H__055555E2_52E0_11D7_B884_00C04F2AADC6__INCLUDED_)
#define AFX_PIPEVIEW_H__055555E2_52E0_11D7_B884_00C04F2AADC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PipeView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPipeView view

#include "WinEVEDoc.h"

class CPipeView : public CView
{
protected:
	CPipeView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPipeView)

// Attributes
public:
	CWinEVEDoc* GetDocument();
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

#ifndef _DEBUG  // debug version in WinEVEView.cpp
inline CWinEVEDoc* CPipeView::GetDocument()
   { return (CWinEVEDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPEVIEW_H__055555E2_52E0_11D7_B884_00C04F2AADC6__INCLUDED_)
