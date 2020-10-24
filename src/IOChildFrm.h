#if !defined(AFX_IOCHILDFRM_H__1C3A18AE_C09D_4E44_A9B4_672002929CD5__INCLUDED_)
#define AFX_IOCHILDFRM_H__1C3A18AE_C09D_4E44_A9B4_672002929CD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IOChildFrm.h : header file
//

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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IOCHILDFRM_H__1C3A18AE_C09D_4E44_A9B4_672002929CD5__INCLUDED_)
