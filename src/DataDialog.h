#pragma once

#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDataDialog dialog

class CDataDialog : public CDialog
{
	// Construction
public:
	CDataDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CDataDialog)
	enum { IDD = IDD_DATA };
	CString	m_contents;
	CString	m_addr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CDataDialog)
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};