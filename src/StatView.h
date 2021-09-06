#if !defined(AFX_STATVIEW_H__597594E5_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_STATVIEW_H__597594E5_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StatView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStatView view

#include "WinMIPS64Doc.h"

class CStatView : public CScrollView
{
protected:
	CStatView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CStatView)

// Attributes
public:
	CWinMIPS64Doc* GetDocument();
	CFont font;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStatView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CStatView)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in StatView.cpp
inline CWinMIPS64Doc* CStatView::GetDocument()
   { return (CWinMIPS64Doc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATVIEW_H__597594E5_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_)
