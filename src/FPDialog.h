#pragma once

/////////////////////////////////////////////////////////////////////////////
// CFPDialog dialog

class CFPDialog : public CDialog
{
	// Construction
public:
	CFPDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CFPDialog)
	enum { IDD = IDD_FP };
	CString	m_contents;
	CString	m_addr;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFPDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CFPDialog)
	virtual void OnOK();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};