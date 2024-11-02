#pragma once

#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CRegDialog dialog

class CRegDialog : public CDialog
{
	// Construction
public:
	CRegDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CRegDialog)
	enum { IDD = IDD_REGISTER };
	CString	m_reg;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRegDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRegDialog)
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
