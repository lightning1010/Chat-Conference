
// ClientDlg.h : header file


#pragma once
#include "ClientCon.h"
#include <Windows.h>
#include "resource.h"
#include "afxwin.h"

// CClientDlg dialog
class CClientDlg : public CDialogEx
{
// Construction
public:
	CClientDlg(CWnd* pParent = NULL);	// standard constructor
	void ShowServerInfo(string sValue);
	void ShowPrivateInfo(string sValue);
	string global_password, global_username;

// Dialog Data
enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual void DoDataExchangePrivate(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();

	ClientCon *m_pClient;
	static UINT __cdecl StaticThreadFunc(LPVOID pParam);
    UINT ThreadFunc();
	void AppendTextToEditCtrl(CEdit& edit, LPCTSTR pszText);
	CEdit m_Portbox;

private:
	HANDLE m_Thread_handle;
	CWinThread *cTh;

public:
	CEdit m_Textbox;
	CEdit private_Textbox;
	afx_msg void OnBnClickedButton3();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit4();
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnEnChangeEdit9();
	afx_msg void OnEnChangeEdit8();
	CEdit xprivate_Textbox;
};
