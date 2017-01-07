#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "DataStruct.h"


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
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedCheckDiscounted();
	afx_msg void OnSelchangeComboRoomtype();
	afx_msg void OnSelchangeComboFloor();
	afx_msg void OnBnClickedButtonAppendroom();
	afx_msg void OnBnClickedButtonCalc();
	afx_msg void OnBnClickedButtonRemoveroom();

	bool GetInfoFromDlg();
	bool GetInfoFromEdit(CEdit & __from, std::string & __info, bool __isnessary = true);
	bool GetInfoFromCombo(CComboBox & __from, std::string & __info, bool __isnessary = true);
	void emptyRoomInfo();

	struct SRoomInfo
	{
		data::SRoom m_basicInfo;
		UINT m_stayDay;
		double m_prePay;	// 从数据库中获取的原价
		double m_despoit;	// 折扣信息
		CString m_cstrRoomid;		// 房间号
		CString m_cstrRoomType;	// 房间类型
		CString m_cstrPrePrice;	// 原价
		CString m_cstrDiscounted;	// 折扣
		CString m_cstrPrice;	// 现价
		CString m_cstrStayDay;	// 预留时间
		CString m_cstrTotal;
	};

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
	bool m_bIsDiscounted;
	data::SGuest m_sGuest;
	stl::CVector<SRoomInfo> m_vecRoom;
	int m_nCilckListLine;

	afx_msg void OnClickListAccount(NMHDR *pNMHDR, LRESULT *pResult);
};
