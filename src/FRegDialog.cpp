// FRegDialog.cpp : implementation file
//

#include "stdafx.h"
#include "wineve.h"
#include "FRegDialog.h"
#include "FontCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFRegDialog dialog


CFRegDialog::CFRegDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CFRegDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFRegDialog)
	m_freg = _T("");
	m_fname = _T("");
	//}}AFX_DATA_INIT
}


void CFRegDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFRegDialog)
	DDX_Text(pDX, IDC_FREG, m_freg);
	DDV_MaxChars(pDX, m_freg, 16);
	DDX_Text(pDX, IDC_FREGNAME, m_fname);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFRegDialog, CDialog)
	//{{AFX_MSG_MAP(CFRegDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFRegDialog message handlers

void CFRegDialog::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	CFont m_Font;

	m_Font.CreateFont(15,0,0,0,400,FALSE,FALSE,
		0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_MODERN,"Courier New");

//	m_Font.CreatePointFont(100,"Courier");
	CFontCtrl<CEdit> * pEdit;
	pEdit = (CFontCtrl<CEdit> *)GetDlgItem(IDC_FREG);
	pEdit->SetFont(&m_Font,FALSE);
	m_Font.DeleteObject();
//	GetDlgItem(IDC_REG)->SetFocus();	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

void CFRegDialog::OnOK() 
{
	// TODO: Add extra validation here
	int i;
	char txt[20];
	UpdateData(TRUE);
	strcpy_s(txt,20,m_freg);

	i=0;
	while (txt[i]!=0 && txt[i]!=' ')
	{
		if (!isdigit(txt[i]) && txt[i]!='.') 
		{
			if (i!=0 || txt[i]!='-') return;
		}
		i++;
	}
	CDialog::OnOK();
}

