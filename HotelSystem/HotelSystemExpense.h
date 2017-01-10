#pragma once
#include "afxwin.h"
#include "afxdtctl.h"
#include <string>


// CHotelSystemExpense dialog

class CHotelSystemExpense : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemExpense)

public:
	CHotelSystemExpense(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotelSystemExpense();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_EXPENSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedCheckIsdiscounted();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonSave();

	CTime StringToCTime(const std::string & __str);
	int GetMaxStayTimeRoomID();
	bool UpdateCheckoutTimeToGuest();
	bool UpdateCheckOutTimeToRoom();

	CEdit m_editRoomID;
	CDateTimeCtrl m_dtNewTime;
	CEdit m_editDiscounted;
	int m_nGuestID;
	int m_nRoomID;
	int m_nTypeID;
	double m_dRoomPrice;
	CTime m_ctRoomChkInDate;
	CTime m_ctRoomChkOutDate;
	CTime m_ctGuestChkOutDate;
	CTime m_ctNewDate;
	int m_nGuestMaxStayRoomID;
	double m_dUnitPrice;
	bool m_bIsDiscounted;
	CEdit m_editOldCheckoutTime;

};
