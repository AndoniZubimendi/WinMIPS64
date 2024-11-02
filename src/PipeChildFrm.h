#pragma once

/////////////////////////////////////////////////////////////////////////////
// CPipeChildFrm frame

class CPipeChildFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CPipeChildFrm)
protected:
	CPipeChildFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPipeChildFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPipeChildFrm();

	// Generated message map functions
	//{{AFX_MSG(CPipeChildFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
