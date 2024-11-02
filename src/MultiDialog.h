#pragma once

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
