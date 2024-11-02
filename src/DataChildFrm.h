#pragma once

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
