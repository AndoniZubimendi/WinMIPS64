// WinMIPS64Doc.h : interface of the CWinMIPS64Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINMIPS64DOC_H__ADC8A8BC_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_)
#define AFX_WINMIPS64DOC_H__ADC8A8BC_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "utils.h"


#define MIN_CODEBITS 8
#define MAX_CODEBITS 13
#define MIN_DATABITS 4
#define MAX_DATABITS 11
#define MIN_ADD_LATENCY 2
#define MAX_ADD_LATENCY 8
#define MIN_MUL_LATENCY 2
#define MAX_MUL_LATENCY 8
#define MIN_DIV_LATENCY 10
#define MAX_DIV_LATENCY 30

#define SYMTABSIZE 1000

class CWinMIPS64Doc : public CDocument
{
protected: // create from serialization only
	CWinMIPS64Doc();
	DECLARE_DYNCREATE(CWinMIPS64Doc)

// Attributes
public:
	char AppDir[MAX_PATH+1];
	char LasDir[MAX_PATH+1];
	CString *codelines;
	CString *datalines;
	CString *assembly;
	CString *mnemonic;
	CString lastfile;

	unsigned int CODESIZE;
	unsigned int DATASIZE;

	processor cpu;
    pipeline pipe;

	BOOL forwarding;
	BOOL delay_slot;
	BOOL branch_target_buffer;
	BOOL registers_as_numbers;

	symbol_table code_table[SYMTABSIZE];
	symbol_table data_table[SYMTABSIZE];
	unsigned int codeptr;
	unsigned int dataptr;

	unsigned int code_symptr;
	unsigned int data_symptr;
	int CODEORDATA;
	unsigned int cycles;
	unsigned int instructions;
	unsigned int loads;
	unsigned int stores;
	unsigned int branch_taken_stalls;
	unsigned int branch_misprediction_stalls;
	unsigned int raw_stalls;
	unsigned int waw_stalls;
	unsigned int war_stalls;
	unsigned int structural_stalls;

	int multi;
	unsigned int ADD_LATENCY;
	unsigned int MUL_LATENCY;
	unsigned int DIV_LATENCY;
	
	BOOL simulation_running;
	BOOL restart;
	int stall_type;
	int stalls;
	int amount;

	record history[50];
	WORD32 entries;
	WORD32 offset;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWinMIPS64Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWinMIPS64Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	int mygets(char *,int,CFile *);
	BOOL openit(CString);
	int openfile(CString);

protected:
	BOOL getcodesym(char *&,WORD32 *);
	BOOL getdatasym(char *&,WORD32 *);
	int instruction(char *);
	BOOL directive(int, char *, char *);
	int first_pass(char *,int);
	int second_pass(char *,int);
	void process_result(RESULT *,BOOL);
	void clear();
	int one_cycle(pipeline *,processor *,BOOL);
	void check_stalls(int,char *,int,char *);
	void update_history(pipeline *,processor *,RESULT *);
	int update_io(processor *);

// Generated message map functions
protected:
	//{{AFX_MSG(CWinMIPS64Doc)
	afx_msg void OnFileReset();
	afx_msg void OnFileOpen();
	afx_msg void OnExecuteSingle();
	afx_msg void OnFileMemory();
	afx_msg void OnExecuteMulticycle();
	afx_msg void OnFileMulti();
	afx_msg void OnExecuteRunto();
	afx_msg void OnExecuteStop();
	afx_msg void OnUpdateExecuteStop(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExecuteRunto(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExecuteSingle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateExecuteMulticycle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMulti(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMemory(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileReset(CCmdUI* pCmdUI);
//	afx_msg void OnUpdateExecuteInterrupt(CCmdUI* pCmdUI);
	afx_msg void OnFullReset();
	afx_msg void OnUpdateFullReset(CCmdUI* pCmdUI);
	afx_msg void OnReload();
	afx_msg void OnUpdateReload(CCmdUI* pCmdUI);
//	afx_msg void OnConfigureWordlength();
	afx_msg void OnFileDelaySlot();
	afx_msg void OnUpdateFileDelaySlot(CCmdUI* pCmdUI);
	afx_msg void OnFileForwarding();
	afx_msg void OnUpdateFileForwarding(CCmdUI* pCmdUI);
	afx_msg void OnBtb();
	afx_msg void OnUpdateBtb(CCmdUI* pCmdUI);
	afx_msg void OnFileRegistersAsNumbers();
	afx_msg void OnUpdateRegistersAsNumbers(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINMIPS64DOC_H__ADC8A8BC_CA8B_11D3_934C_00A024DFE8FB__INCLUDED_)
