#if !defined(AFX_DATAVIEW_H__12D1DD10_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_DATAVIEW_H__12D1DD10_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DataView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataView view

#include "WinEVEDoc.h"

class CDataView : public CScrollView
{
protected:
	CDataView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CDataView)

// Attributes
public:
	CWinMIPS64Doc* GetDocument();
	int first;
	CFont font;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataView)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDataView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CDataView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


#ifndef _DEBUG  // debug version in WinEVEView.cpp
inline CWinMIPS64Doc* CDataView::GetDocument()
   { return (CWinMIPS64Doc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAVIEW_H__12D1DD10_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_)
