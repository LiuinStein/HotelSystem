// HotelSystemChangeRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemChangeRoomDlg.h"
#include "afxdialogex.h"


// CHotelSystemChangeRoomDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemChangeRoomDlg, CDialogEx)

CHotelSystemChangeRoomDlg::CHotelSystemChangeRoomDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHANGEROOM, pParent)
{

}

CHotelSystemChangeRoomDlg::~CHotelSystemChangeRoomDlg()
{
}

void CHotelSystemChangeRoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHotelSystemChangeRoomDlg, CDialogEx)
END_MESSAGE_MAP()


// CHotelSystemChangeRoomDlg message handlers


BOOL CHotelSystemChangeRoomDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CHotelSystemChangeRoomDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}
