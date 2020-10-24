#if !defined(AFX_CYCLESVIEW_H__FD4DCC82_5931_11D7_B885_00C04F2AADC6__INCLUDED_)
#define AFX_CYCLESVIEW_H__FD4DCC82_5931_11D7_B885_00C04F2AADC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CyclesView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCyclesView view

#include "WinEVEDoc.h"

class CCyclesView : public CScrollView
{
protected:
	CCyclesView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCyclesView)

// Attributes
public:
	CWinEVEDoc* GetDocument();
	WORD32 srow;
	WORD32 nrow;
	WORD32 scol;
	WORD32 ncol;
	CFont font;
	CBrush yellow,cyan,red,green,magenta,dcyan,dgreen,dyellow;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCyclesView)
	public:
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCyclesView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CCyclesView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WinEVEView.cpp
inline CWinEVEDoc* CCyclesView::GetDocument()
   { return (CWinEVEDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYCLESVIEW_H__FD4DCC82_5931_11D7_B885_00C04F2AADC6__INCLUDED_)
