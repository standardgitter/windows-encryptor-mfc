// encrypt_winDlg.h : header file
//

#if !defined(AFX_ENCRYPT_WINDLG_H__671C68E8_BD08_4B34_9FA8_677A3B144661__INCLUDED_)
#define AFX_ENCRYPT_WINDLG_H__671C68E8_BD08_4B34_9FA8_677A3B144661__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CEncrypt_winDlg dialog

class CEncrypt_winDlg : public CDialog
{
// Construction
public:
	CEncrypt_winDlg(CWnd* pParent = NULL);	// standard constructor
	unsigned char *hex_dump(unsigned char *src, int src_len ,unsigned char *result);
	CStringArray  m_AryFilename,m_AryFiletitle;
	
	CWinThread* pThread;
	static UINT encryptAdp(LPVOID pParam);
	void encrypt();
	static UINT decryptAdp(LPVOID pParam);
	void decrypt();
// Dialog Data
	//{{AFX_DATA(CEncrypt_winDlg)
	enum { IDD = IDD_ENCRYPT_WIN_DIALOG };
	CListCtrl	m_file_list;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEncrypt_winDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CEncrypt_winDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnEncBtn();
	afx_msg void OnFileBtn();
	afx_msg void OnDecBtn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENCRYPT_WINDLG_H__671C68E8_BD08_4B34_9FA8_677A3B144661__INCLUDED_)
