#pragma once


// CHotelSystemAboutDlg dialog

class CHotelSystemAboutDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemAboutDlg)

public:
	CHotelSystemAboutDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotelSystemAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
