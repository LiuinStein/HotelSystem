#pragma once
#include "afxwin.h"
#include <string>


// CHotelSystemRoomPropretiesDlg dialog

class CHotelSystemRoomPropretiesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemRoomPropretiesDlg)

public:
	CHotelSystemRoomPropretiesDlg(CWnd* pParent = NULL);   // standard constructor
	CHotelSystemRoomPropretiesDlg(bool __isAdd, int __roomid = NULL, CWnd* pParent = NULL);
	virtual ~CHotelSystemRoomPropretiesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ROOMPROPRETIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedButtonCancel();
	afx_msg void OnBnClickedButtonSave();
	
	bool GetInfoFromCombo(CComboBox& __from, std::string& __info, bool __isnessary = true);
	bool GetInfoFromEdit(CEdit& __from, int & __info, bool __isnessary = true);

	CEdit m_editRoomID;
	CComboBox m_comboRoomType;
	bool m_bAddRoom;
	int m_nRoomID;

};
