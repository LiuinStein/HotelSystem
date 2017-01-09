#pragma once


// CHotelSystemChangeRoomDlg dialog

class CHotelSystemChangeRoomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemChangeRoomDlg)

public:
	CHotelSystemChangeRoomDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotelSystemChangeRoomDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHANGEROOM };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
