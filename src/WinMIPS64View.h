// WinMIPS64View.h : interface of the CWinMIPS64View class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

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
	void OnDraw(CDC* pDC) override;  // overridden to draw this view
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
	void OnInitialUpdate() override;
protected:
	void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) override;
	//}}AFX_VIRTUAL

// Implementation
public:
	~CWinMIPS64View() override;
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
{
	return (CWinMIPS64Doc*)m_pDocument;
}
#endif
