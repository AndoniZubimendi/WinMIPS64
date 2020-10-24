// WinEVE.h : main header file for the WINEVE application
//

#if !defined(AFX_WINEVE_H__ADC8A8B4_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_)
#define AFX_WINEVE_H__ADC8A8B4_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "../resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinEVEApp:
// See WinEVE.cpp for the implementation of this class
//

class CWinEVEApp : public CWinApp
{
public:
	CWinEVEApp();
	CMultiDocTemplate* regtemp;
	CMultiDocTemplate* datatemp;
	CMultiDocTemplate* stattemp;
	CMultiDocTemplate* pipetemp;
	CMultiDocTemplate* cyclestemp;
	CMultiDocTemplate* iotemp;
	int ExitInstance();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinEVEApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinEVEApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINEVE_H__ADC8A8B4_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_)
