// FPDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinMIPS64.h"
#include "FPDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFPDialog dialog


CFPDialog::CFPDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFPDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFPDialog)
	m_contents = _T("");
	m_addr = _T("");
	//}}AFX_DATA_INIT
}


void CFPDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFPDialog)
	DDX_Text(pDX, IDC_CONTENTS, m_contents);
	DDV_MaxChars(pDX, m_contents, 16);
	DDX_Text(pDX, IDC_LOCATION, m_addr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFPDialog, CDialog)
	//{{AFX_MSG_MAP(CFPDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFPDialog message handlers

void CFPDialog::OnOK()
{
	int i;
	char txt[2000];
	UpdateData(TRUE);
	strcpy_s(txt, 2000, m_contents);

	i = 0;
	while (txt[i] != 0 && txt[i] != ' ')
	{
		if (!isdigit(txt[i]) && txt[i] != '.')
		{
			if (i != 0 || txt[i] != '-') return;
		}
		i++;
	}
	CDialog::OnOK();
}

void CFPDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here

	// Do not call CDialog::OnPaint() for painting messages
}
