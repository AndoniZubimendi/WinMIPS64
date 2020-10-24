#if !defined(AFX_CYCLESCHILDFRM_H__FD4DCC81_5931_11D7_B885_00C04F2AADC6__INCLUDED_)
#define AFX_CYCLESCHILDFRM_H__FD4DCC81_5931_11D7_B885_00C04F2AADC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
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

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CYCLESCHILDFRM_H__FD4DCC81_5931_11D7_B885_00C04F2AADC6__INCLUDED_)
