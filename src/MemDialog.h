#pragma once

/////////////////////////////////////////////////////////////////////////////
// CMemDialog dialog

class CMemDialog : public CDialog
{
	// Construction
public:
	CMemDialog(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
		//{{AFX_DATA(CMemDialog)
	enum { IDD = IDD_ARCHITECTURE };
	int		m_code;
	int		m_data;
	int		m_fpal;
	int		m_fpdl;
	int		m_fpml;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMemDialog)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
