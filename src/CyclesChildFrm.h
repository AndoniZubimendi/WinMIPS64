#pragma once

// CyclesChildFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCyclesChildFrm frame

class CCyclesChildFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CCyclesChildFrm)
protected:
	CCyclesChildFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCyclesChildFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	CSplitterWnd split;
	virtual ~CCyclesChildFrm();

	// Generated message map functions
	//{{AFX_MSG(CCyclesChildFrm)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
