#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CHotelSystemCheckoutDlg dialog

class CHotelSystemCheckoutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemCheckoutDlg)

public:
	CHotelSystemCheckoutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotelSystemCheckoutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHECKOUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonQuit();
	afx_msg void OnBnClickedButtonIndex();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();

	CEdit m_editRoomID;
	CEdit m_editGuestName;
	CListCtrl m_listInfo;
	bool m_bUseRoomID;
};
