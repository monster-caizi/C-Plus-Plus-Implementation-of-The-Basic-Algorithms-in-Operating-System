// 11Dlg.h : header file
//

#include<windows.h>
#include "Struct.h"

#if !defined(AFX_11DLG_H__FC455319_87A1_4FE5_BE83_AD2F0F6183C7__INCLUDED_)
#define AFX_11DLG_H__FC455319_87A1_4FE5_BE83_AD2F0F6183C7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMy11Dlg dialog

class CMy11Dlg : public CDialog
{
// Construction
public:
	CMy11Dlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMy11Dlg)
	enum { IDD = IDD_MY11_DIALOG };
	CListCtrl	mylist;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy11Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	node *root[50];
	void update(int);
	// Generated message map functions
	//{{AFX_MSG(CMy11Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButton1();
	afx_msg void OnButton2();
	afx_msg void OnButton3();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_11DLG_H__FC455319_87A1_4FE5_BE83_AD2F0F6183C7__INCLUDED_)
