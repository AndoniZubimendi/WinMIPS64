// WinEVEView.h : interface of the CWinEVEView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINEVEVIEW_H__ADC8A8BE_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_)
#define AFX_WINEVEVIEW_H__ADC8A8BE_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWinEVEView : public CScrollView
{
protected: // create from serialization only
	CWinEVEView();
	DECLARE_DYNCREATE(CWinEVEView)

// Attributes
public:
	CWinEVEDoc* GetDocument();
	unsigned int first;
	unsigned int nlines;
	CFont font;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinEVEView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWinEVEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinEVEView)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in WinEVEView.cpp
inline CWinEVEDoc* CWinEVEView::GetDocument()
   { return (CWinEVEDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINEVEVIEW_H__ADC8A8BE_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_)
