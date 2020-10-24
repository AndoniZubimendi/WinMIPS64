#if !defined(AFX_FREGDIALOG_H__912C7C21_4D62_11D7_B884_00C04F2AADC6__INCLUDED_)
#define AFX_FREGDIALOG_H__912C7C21_4D62_11D7_B884_00C04F2AADC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FRegDialog.h : header file
//

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

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FREGDIALOG_H__912C7C21_4D62_11D7_B884_00C04F2AADC6__INCLUDED_)
