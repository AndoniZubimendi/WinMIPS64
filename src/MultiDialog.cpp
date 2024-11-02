// MultiDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinMIPS64.h"
#include "MultiDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiDialog dialog


CMultiDialog::CMultiDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiDialog)
	m_cycles = 0;
	//}}AFX_DATA_INIT
}


void CMultiDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiDialog)
	DDX_Text(pDX, IDC_CYCLES, m_cycles);
	DDV_MinMaxUInt(pDX, m_cycles, 2, 100000000);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiDialog, CDialog)
	//{{AFX_MSG_MAP(CMultiDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiDialog message handlers

void CMultiDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	GetDlgItem(IDC_CYCLES)->SetFocus();

	// Do not call CDialog::OnPaint() for painting messages
}
