#if !defined(AFX_MULTIDIALOG_H__0A44B16A_D260_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_MULTIDIALOG_H__0A44B16A_D260_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MultiDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMultiDialog dialog

class CMultiDialog : public CDialog
{
// Construction
public:
	CMultiDialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMultiDialog)
	enum { IDD = IDD_MULTI_CYCLE };
	UINT	m_cycles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMultiDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMultiDialog)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIDIALOG_H__0A44B16A_D260_11D3_9351_00A024DFE8FB__INCLUDED_)
