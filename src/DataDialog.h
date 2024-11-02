#if !defined(AFX_DATADIALOG_H__674501FC_D345_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_DATADIALOG_H__674501FC_D345_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DataDialog.h : header file
//

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

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATADIALOG_H__674501FC_D345_11D3_9351_00A024DFE8FB__INCLUDED_)
