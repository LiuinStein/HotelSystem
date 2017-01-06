#pragma once
#include "afxcmn.h"


// CHotelSystemMainDlg dialog

class CHotelSystemMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemMainDlg)

	// ˢ���б�
	void refreshList();

public:
	CHotelSystemMainDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotelSystemMainDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_listSplash;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonRefresh();
};