#pragma once

/////////////////////////////////////////////////////////////////////////////
// CIOChildFrm frame

class CIOChildFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CIOChildFrm)
protected:
	CIOChildFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIOChildFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CIOChildFrm();

	// Generated message map functions
	//{{AFX_MSG(CIOChildFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
