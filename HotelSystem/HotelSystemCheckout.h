#pragma once


// CHotelSystemCheckout dialog

class CHotelSystemCheckout : public CDialogEx
{
	DECLARE_DYNAMIC(CHotelSystemCheckout)

public:
	CHotelSystemCheckout(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHotelSystemCheckout();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_CHECKOUT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
