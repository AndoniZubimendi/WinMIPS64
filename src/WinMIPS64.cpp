// WinMIPS64.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "WinMIPS64.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "WinMIPS64Doc.h"
#include "WinMIPS64View.h"
#include "DataView.h"
#include "RegView.h"
#include "StatView.h"
#include "IOView.h"
#include "PipeView.h"
#include "RegChildFrm.h"
#include "DataChildFrm.h"
#include "StatChildFrm.h"
#include "PipeChildFrm.h"
#include "CyclesChildFrm.h"
#include "IOChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64App

BEGIN_MESSAGE_MAP(CWinMIPS64App, CWinApp)
	//{{AFX_MSG_MAP(CWinMIPS64App)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG_MAP
// Standard file based document commands
ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64App construction

CWinMIPS64App::CWinMIPS64App()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWinMIPS64App object

CWinMIPS64App theApp;

/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64App initialization

BOOL CWinMIPS64App::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	// Change the registry key under which our settings are stored.
	// You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(
		IDR_WINMIPS64TYPE,
		RUNTIME_CLASS(CWinMIPS64Doc),
		RUNTIME_CLASS(CChildFrame), // custom MDI child frame
		RUNTIME_CLASS(CWinMIPS64View));
	AddDocTemplate(pDocTemplate);

	regtemp = new CMultiDocTemplate(
		IDR_WINMIPS64TYPE,
		RUNTIME_CLASS(CWinMIPS64Doc),
		RUNTIME_CLASS(CRegChildFrm), // custom MDI child frame
		RUNTIME_CLASS(CRegView));
	datatemp = new CMultiDocTemplate(
		IDR_WINMIPS64TYPE,
		RUNTIME_CLASS(CWinMIPS64Doc),
		RUNTIME_CLASS(CDataChildFrm), // custom MDI child frame
		RUNTIME_CLASS(CDataView));

	stattemp = new CMultiDocTemplate(
		IDR_WINMIPS64TYPE,
		RUNTIME_CLASS(CWinMIPS64Doc),
		RUNTIME_CLASS(CStatChildFrm), // custom MDI child frame
		RUNTIME_CLASS(CStatView));

	pipetemp = new CMultiDocTemplate(
		IDR_WINMIPS64TYPE,
		RUNTIME_CLASS(CWinMIPS64Doc),
		RUNTIME_CLASS(CPipeChildFrm), // custom MDI child frame
		RUNTIME_CLASS(CPipeView));

	cyclestemp = new CMultiDocTemplate(
		IDR_WINMIPS64TYPE,
		RUNTIME_CLASS(CWinMIPS64Doc),
		RUNTIME_CLASS(CCyclesChildFrm), // custom MDI child frame
		RUNTIME_CLASS(CView));

	iotemp = new CMultiDocTemplate(
		IDR_WINMIPS64TYPE,
		RUNTIME_CLASS(CWinMIPS64Doc),
		RUNTIME_CLASS(CIOChildFrm), // custom MDI child frame
		RUNTIME_CLASS(CIOView));


	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME, WS_OVERLAPPEDWINDOW | WS_MAXIMIZE))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it.

	pMainFrame->StatWindow();
	pMainFrame->DataWindow();

	pMainFrame->RegisterWindow();
	pMainFrame->PipeWindow();
	pMainFrame->CyclesWindow();

	pMainFrame->ShowWindow(/*m_nCmdShow|*/SW_SHOWMAXIMIZED);

	pMainFrame->MDITile();

	pMainFrame->IOWindow();

	pMainFrame->MDINext();
	pMainFrame->MDINext();
	pMainFrame->MDINext();
	pMainFrame->MDINext();
	pMainFrame->MDINext();
	pMainFrame->MDINext();

	pMainFrame->UpdateWindow();

	return TRUE;
}

int CWinMIPS64App::ExitInstance()
{
	delete regtemp;
	delete datatemp;
	delete stattemp;
	delete pipetemp;
	delete cyclestemp;
	delete iotemp;
	return CWinApp::ExitInstance();
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// Dialog Data
		//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CWinMIPS64App::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CWinMIPS64App commands
