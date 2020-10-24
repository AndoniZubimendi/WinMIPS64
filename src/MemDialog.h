#if !defined(AFX_MEMDIALOG_H__0A44B169_D260_11D3_9351_00A024DFE8FB__INCLUDED_)
#define AFX_MEMDIALOG_H__0A44B169_D260_11D3_9351_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MemDialog.h : header file
//

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

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMDIALOG_H__0A44B169_D260_11D3_9351_00A024DFE8FB__INCLUDED_)
