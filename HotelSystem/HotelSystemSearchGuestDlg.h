#pragma once
#include "afxwin.h"
#include "DataStruct.h"


// CHotelSystemSearchGuestDlg dialog

class CHotelSystemSearchGuestDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemSearchGuestDlg)

public:
	CHotelSystemSearchGuestDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotelSystemSearchGuestDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SEARCHGUEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedRadioRoomid();
	afx_msg void OnBnClickedRadioName();
	afx_msg void OnBnClickedCheckOnchange();
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonReload();
	afx_msg void OnBnClickedButtonSave();

	void EnableChangeInfo(BOOL __b);
	bool GetInfoByRoomID();
	bool GetInfoByGuestName();
	bool GetInfoFromDBByID(int __id);
	void ShowInfoOnDlg();
	std::string GetInputFromCWnd(CWnd * __inp, std::string & __ref);

	CEdit m_editRoomID;
	CEdit m_editGuestName;
	CEdit m_editGName;
	CComboBox m_comboGSex;
	CEdit m_editGYear;
	CEdit m_editGCompName;
	CEdit m_editGCompAddr;
	CComboBox m_comboGIDCardType;
	CEdit m_editGIDCardID;
	CEdit m_editGPhone;
	CEdit m_editGAddr;
	data::SGuest m_guest;
	data::SGuest m_guestChanged;
	bool m_bOnChange;
	bool m_bUseRoomID;
	CButton m_buttonSave;

};
