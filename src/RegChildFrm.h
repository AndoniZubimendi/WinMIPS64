#if !defined(AFX_REGCHILDFRM_H__12D1DD12_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_REGCHILDFRM_H__12D1DD12_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RegChildFrm.h : header file
//

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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGCHILDFRM_H__12D1DD12_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_)
