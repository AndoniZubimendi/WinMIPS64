#pragma once

/////////////////////////////////////////////////////////////////////////////
// CStatChildFrm frame

class CStatChildFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CStatChildFrm)
protected:
	CStatChildFrm();           // protected constructor used by dynamic creation

	// Attributes
public:

	// Operations
public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CStatChildFrm)
public:
	virtual void ActivateFrame(int nCmdShow = -1);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CStatChildFrm();

	// Generated message map functions
	//{{AFX_MSG(CStatChildFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
