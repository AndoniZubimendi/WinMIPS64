#pragma once

/////////////////////////////////////////////////////////////////////////////
// CFRegDialog dialog

class CFRegDialog : public CDialog
{
// Construction
public:
	CFRegDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFRegDialog)
	enum { IDD = IDD_DOUBLE };
	CString	m_freg;
	CString	m_fname;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFRegDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFRegDialog)
	afx_msg void OnPaint();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};