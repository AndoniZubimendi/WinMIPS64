// MemDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WinEVE.h"
#include "MemDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemDialog dialog


CMemDialog::CMemDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMemDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMemDialog)
	m_code = 0;
	m_data = 0;
	m_fpal = 0;
	m_fpdl = 0;
	m_fpml = 0;
	//}}AFX_DATA_INIT
}


void CMemDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemDialog)
	DDX_Text(pDX, IDC_CODEMEM, m_code);
	DDV_MinMaxInt(pDX, m_code, 8, 13);
	DDX_Text(pDX, IDC_DATAMEM, m_data);
	DDV_MinMaxInt(pDX, m_data, 4, 12);
	DDX_Text(pDX, IDC_FPAL, m_fpal);
	DDV_MinMaxInt(pDX, m_fpal, 2, 8);
	DDX_Text(pDX, IDC_FPDL, m_fpdl);
	DDV_MinMaxInt(pDX, m_fpdl, 10, 30);
	DDX_Text(pDX, IDC_FPML, m_fpml);
	DDV_MinMaxInt(pDX, m_fpml, 2, 8);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMemDialog, CDialog)
	//{{AFX_MSG_MAP(CMemDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemDialog message handlers

