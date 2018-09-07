// 11.h : main header file for the 11 application
//

#if !defined(AFX_11_H__4B206BD1_1843_49BE_8C48_91C92E570FF3__INCLUDED_)
#define AFX_11_H__4B206BD1_1843_49BE_8C48_91C92E570FF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMy11App:
// See 11.cpp for the implementation of this class
//

class CMy11App : public CWinApp
{
public:
	CMy11App();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMy11App)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMy11App)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_11_H__4B206BD1_1843_49BE_8C48_91C92E570FF3__INCLUDED_)
