#if !defined(AFX_STATCHILDFRM_H__597594E6_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_STATCHILDFRM_H__597594E6_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StatChildFrm.h : header file
//

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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATCHILDFRM_H__597594E6_CD9E_11D3_9351_00A024DFE8FB__INCLUDED_)
