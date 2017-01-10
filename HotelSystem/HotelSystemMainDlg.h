#pragma once
#include "afxcmn.h"
#include <string>
#include "Vector.h"


// CHotelSystemMainDlg dialog

class CHotelSystemMainDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemMainDlg)

	// Ë¢ÐÂÁÐ±í
	void RefreshList();

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

	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonCheckin();
	afx_msg void OnBnClickedButtonCheckout();
	afx_msg void OnBnClickedButtonSearchguest();
	afx_msg void OnBnClickedButtonExpense();

	struct SSplashShow
	{
		int m_nRoomID;
		int m_nTypeID;
		int m_nGuestID;
		double m_dPrice;
		bool m_bIsDirty;
		std::string m_strGuestName;
		std::string m_strCheckoutTime;
	};

	CListCtrl m_listSplash;
	stl::CVector<SSplashShow> m_vecSplashRoom;

	afx_msg void OnBnClickedButtonAbout();
};
