// WinMIPS64.h : main header file for the WinMIPS64 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "../resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64App:
// See WinMIPS64.cpp for the implementation of this class
//

class CWinMIPS64App : public CWinApp
{
public:
	CWinMIPS64App();
	CMultiDocTemplate* regtemp;
	CMultiDocTemplate* datatemp;
	CMultiDocTemplate* stattemp;
	CMultiDocTemplate* pipetemp;
	CMultiDocTemplate* cyclestemp;
	CMultiDocTemplate* iotemp;
	int ExitInstance();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinMIPS64App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CWinMIPS64App)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSGs
	DECLARE_MESSAGE_MAP()
};
