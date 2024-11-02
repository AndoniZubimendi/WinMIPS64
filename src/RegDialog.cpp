// RegDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinMIPS64.h"
#include "RegDialog.h"
#include "FontCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegDialog dialog


CRegDialog::CRegDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CRegDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegDialog)
	m_reg = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT

}


void CRegDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegDialog)
	DDX_Text(pDX, IDC_REG, m_reg);
	DDV_MaxChars(pDX, m_reg, 16);
	DDX_Text(pDX, IDC_REGNAME, m_name);
	//}}AFX_DATA_MAP

}


BEGIN_MESSAGE_MAP(CRegDialog, CDialog)
	//{{AFX_MSG_MAP(CRegDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegDialog message handlers

void CRegDialog::OnOK()
{
	int i;
	char txt[20];
	UpdateData(TRUE);
	strcpy_s(txt, 20, m_reg);

	i = 0;
	while (txt[i] != 0 && txt[i] != ' ')
	{
		if (!isxdigit(txt[i])) return;
		i++;
	}

	CDialog::OnOK();
}

void CRegDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CFont m_Font;

	m_Font.CreateFont(15, 0, 0, 0, 400, FALSE, FALSE,
		0, ANSI_CHARSET, OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_MODERN, "Courier New");

	//	m_Font.CreatePointFont(100,"Courier");
	CFontCtrl<CEdit>* pEdit;
	pEdit = (CFontCtrl<CEdit> *)GetDlgItem(IDC_REG);
	pEdit->SetFont(&m_Font, FALSE);
	m_Font.DeleteObject();
	//	GetDlgItem(IDC_REG)->SetFocus();	
		// Do not call CDialog::OnPaint() for painting messages
}
