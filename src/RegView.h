#if !defined(AFX_REGVIEW_H__597594E4_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_REGVIEW_H__597594E4_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RegView.h : header file
//
#include "WinEVEDoc.h"
/////////////////////////////////////////////////////////////////////////////
// CRegView view

class CRegView : public CScrollView
{
protected:
	CRegView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CRegView)

// Attributes
public:
	CWinMIPS64Doc* GetDocument();
	int first;
	CFont font;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRegView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CRegView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in WinEVEView.cpp
inline CWinMIPS64Doc* CRegView::GetDocument()
   { return (CWinMIPS64Doc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGVIEW_H__597594E4_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_)
