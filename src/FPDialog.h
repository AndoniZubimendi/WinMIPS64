#if !defined(AFX_FPDIALOG_H__991C3F86_63AD_419D_B02E_C08E0C73246E__INCLUDED_)
#define AFX_FPDIALOG_H__991C3F86_63AD_419D_B02E_C08E0C73246E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FPDialog.h : header file
//

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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FPDIALOG_H__991C3F86_63AD_419D_B02E_C08E0C73246E__INCLUDED_)
