// encrypt_win.h : main header file for the ENCRYPT_WIN application
//

#if !defined(AFX_ENCRYPT_WIN_H__8637CDAF_3A4E_4E68_9588_F413F042B4EB__INCLUDED_)
#define AFX_ENCRYPT_WIN_H__8637CDAF_3A4E_4E68_9588_F413F042B4EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CEncrypt_winApp:
// See encrypt_win.cpp for the implementation of this class
//

class CEncrypt_winApp : public CWinApp
{
public:
	CEncrypt_winApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEncrypt_winApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CEncrypt_winApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCRYPT_WIN_H__8637CDAF_3A4E_4E68_9588_F413F042B4EB__INCLUDED_)
