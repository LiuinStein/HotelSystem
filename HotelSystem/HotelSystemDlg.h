
// HotelSystemDlg.h : header file
//

#pragma once
#include "afxwin.h"


// CHotelSystemDlg dialog
class CHotelSystemDlg : public CDialogEx
{
// Construction
public:
	CHotelSystemDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOTELSYSTEM_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonLogin();

	CEdit m_editUsername;
	CEdit m_editPassword;
};
