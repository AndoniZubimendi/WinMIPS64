// WinMIPS64View.h : interface of the CWinMIPS64View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINMIPS64VIEW_H__ADC8A8BE_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_)
#define AFX_WINMIPS64VIEW_H__ADC8A8BE_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWinMIPS64View : public CScrollView
{
protected: // create from serialization only
	CWinMIPS64View();
	DECLARE_DYNCREATE(CWinMIPS64View)

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
	//{{AFX_VIRTUAL(CWinMIPS64View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWinMIPS64View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWinMIPS64View)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in CWinMIPS64View.cpp
inline CWinMIPS64Doc* CWinMIPS64View::GetDocument()
   { return (CWinMIPS64Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMIPS64VIEW_H__ADC8A8BE_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_)
