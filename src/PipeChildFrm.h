#if !defined(AFX_PIPECHILDFRM_H__055555E1_52E0_11D7_B884_00C04F2AADC6__INCLUDED_)
#define AFX_PIPECHILDFRM_H__055555E1_52E0_11D7_B884_00C04F2AADC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PipeChildFrm.h : header file
//

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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIPECHILDFRM_H__055555E1_52E0_11D7_B884_00C04F2AADC6__INCLUDED_)
