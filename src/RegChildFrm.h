#pragma once

/////////////////////////////////////////////////////////////////////////////
// CRegChildFrm frame

class CRegChildFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CRegChildFrm)
protected:
	CRegChildFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegChildFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CRegChildFrm();

	// Generated message map functions
	//{{AFX_MSG(CRegChildFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
