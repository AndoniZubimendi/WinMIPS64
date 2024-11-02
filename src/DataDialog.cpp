// DataDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinMIPS64.h"
#include "DataDialog.h"
#include "FontCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataDialog dialog


CDataDialog::CDataDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CDataDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDataDialog)
	m_contents = _T("");
	m_addr = _T("");
	//}}AFX_DATA_INIT
}


void CDataDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDataDialog)
	DDX_Text(pDX, IDC_CONTENTS, m_contents);
	DDV_MaxChars(pDX, m_contents, 16);
	DDX_Text(pDX, IDC_LOCATION, m_addr);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDataDialog, CDialog)
	//{{AFX_MSG_MAP(CDataDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataDialog message handlers

void CDataDialog::OnOK()
{
	int i;
	char txt[20];

	UpdateData(TRUE);
	strcpy_s(txt, 20, m_contents);

	i = 0;
	while (txt[i] != 0 && txt[i] != ' ')
	{
		if (!isxdigit(txt[i])) return;
		i++;
	}
	CDialog::OnOK();

}

void CDataDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	CFont m_Font;

	m_Font.CreateFont(15, 0, 0, 0, 400, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_MODERN, "Courier New");

	//	m_Font.CreatePointFont(100,"Courier");
	CFontCtrl<CEdit>* pEdit;
	pEdit = (CFontCtrl<CEdit> *)GetDlgItem(IDC_CONTENTS);
	pEdit->SetFont(&m_Font, FALSE);
	m_Font.DeleteObject();

	//	GetDlgItem(IDC_CONTENTS)->SetFocus();	
		// Do not call CDialog::OnPaint() for painting messages
}


