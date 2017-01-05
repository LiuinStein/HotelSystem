#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CHotelSystemCheckinDlg dialog

class CHotelSystemCheckinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemCheckinDlg)

public:
	CHotelSystemCheckinDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotelSystemCheckinDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHECKIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CEdit m_editGuestName;
	CComboBox m_comboGuestSex;
	CEdit m_editGuestYearOld;
	CComboBox m_comboGuestIDCardType;
	CEdit m_editGuestIDCardID;
	CEdit m_editGuestPhone;
	CEdit m_editGuestAddr;
	CEdit m_editGuestCompName;
	CEdit m_editGuestCompAddr;
	CComboBox m_comboRoomType;
	CComboBox m_comboRoomFloor;
	CComboBox m_comboRoomNumber;
	CEdit m_editStayTime;
	CEdit m_editPayDiscounted;
	CEdit m_editPayPrePrice;
	CEdit m_editPayLess;
	CEdit m_editPayDeposite;
	CEdit m_editPayTotal;
	CComboBox m_comboPayMethod;
	CListCtrl m_listAccount;
};
