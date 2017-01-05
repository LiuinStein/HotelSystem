// HotelSystemCheckinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemCheckinDlg.h"
#include "afxdialogex.h"


// CHotelSystemCheckinDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemCheckinDlg, CDialogEx)

CHotelSystemCheckinDlg::CHotelSystemCheckinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHECKIN, pParent)
{

}

CHotelSystemCheckinDlg::~CHotelSystemCheckinDlg()
{
}

void CHotelSystemCheckinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GUESTNAME, m_editGuestName);
	DDX_Control(pDX, IDC_COMBO_SEX, m_comboGuestSex);
	DDX_Control(pDX, IDC_EDIT_YEAROLD, m_editGuestYearOld);
	DDX_Control(pDX, IDC_COMBO_IDCARDTYPE, m_comboGuestIDCardType);
	DDX_Control(pDX, IDC_EDIT_IDCARDID, m_editGuestIDCardID);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editGuestPhone);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editGuestAddr);
	DDX_Control(pDX, IDC_EDIT_COMPANYNAME, m_editGuestCompName);
	DDX_Control(pDX, IDC_EDIT_COMPANYADD, m_editGuestCompAddr);
	DDX_Control(pDX, IDC_COMBO_ROOMTYPE, m_comboRoomType);
	DDX_Control(pDX, IDC_COMBO_FLOOR, m_comboRoomFloor);
	DDX_Control(pDX, IDC_COMBO_ROOMID, m_comboRoomNumber);
	DDX_Control(pDX, IDC_EDIT_STAYDAY, m_editStayTime);
	DDX_Control(pDX, IDC_EDIT_DISCOUNTED, m_editPayDiscounted);
	DDX_Control(pDX, IDC_EDIT_PREPAY, m_editPayPrePrice);
	DDX_Control(pDX, IDC_EDIT_SHOWDISCOUNTED, m_editPayLess);
	DDX_Control(pDX, IDC_EDIT13, m_editPayDeposite);
	DDX_Control(pDX, IDC_EDIT_TOTAL, m_editPayTotal);
	DDX_Control(pDX, IDC_COMBO6, m_comboPayMethod);
	DDX_Control(pDX, IDC_LIST_ACCOUNT, m_listAccount);
}


BEGIN_MESSAGE_MAP(CHotelSystemCheckinDlg, CDialogEx)
END_MESSAGE_MAP()


// CHotelSystemCheckinDlg message handlers


BOOL CHotelSystemCheckinDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
