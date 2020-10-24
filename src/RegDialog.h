#if !defined(AFX_REGDIALOG_H__0A44B168_D260_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_REGDIALOG_H__0A44B168_D260_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RegDialog.h : header file
//

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

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_REGDIALOG_H__0A44B168_D260_11D3_9351_00A024DFE8FB__INCLUDED_)
