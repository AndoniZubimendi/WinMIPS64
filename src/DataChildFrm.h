#if !defined(AFX_DATACHILDFRM_H__12D1DD14_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_DATACHILDFRM_H__12D1DD14_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DataChildFrm.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDataChildFrm frame

class CDataChildFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CDataChildFrm)
protected:
	CDataChildFrm();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataChildFrm)
	public:
	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CDataChildFrm();

	// Generated message map functions
	//{{AFX_MSG(CDataChildFrm)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATACHILDFRM_H__12D1DD14_CCD6_11D3_9351_00A024DFE8FB__INCLUDED_)
