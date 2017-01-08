// HotelSystemCheckoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemCheckoutDlg.h"
#include "afxdialogex.h"


// CHotelSystemCheckoutDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemCheckoutDlg, CDialogEx)

CHotelSystemCheckoutDlg::CHotelSystemCheckoutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHECKOUT, pParent), m_bUseRoomID(true)
{

}

CHotelSystemCheckoutDlg::~CHotelSystemCheckoutDlg()
{
}

void CHotelSystemCheckoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ROOMID, m_editRoomID);
	DDX_Control(pDX, IDC_EDIT1, m_editGuestName);
	DDX_Control(pDX, IDC_LIST_INFO, m_listInfo);
}


BEGIN_MESSAGE_MAP(CHotelSystemCheckoutDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_QUIT, &CHotelSystemCheckoutDlg::OnBnClickedButtonQuit)
	ON_BN_CLICKED(IDC_BUTTON_INDEX, &CHotelSystemCheckoutDlg::OnBnClickedButtonIndex)
	ON_BN_CLICKED(IDC_RADIO1, &CHotelSystemCheckoutDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CHotelSystemCheckoutDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CHotelSystemCheckoutDlg message handlers


BOOL CHotelSystemCheckoutDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

// �رմ��尴ť�¼���Ӧ
void CHotelSystemCheckoutDlg::OnBnClickedButtonQuit()
{
	this->DestroyWindow();
	this->EndDialog(0);
}

// ������Ϣ��ť�¼���Ӧ
void CHotelSystemCheckoutDlg::OnBnClickedButtonIndex()
{

}

// radioʹ�÷����
void CHotelSystemCheckoutDlg::OnBnClickedRadio1()
{
	m_bUseRoomID = true;
	m_editRoomID.EnableWindow(TRUE);
	m_editGuestName.EnableWindow(FALSE);
}

// radioʹ������
void CHotelSystemCheckoutDlg::OnBnClickedRadio2()
{
	m_bUseRoomID = false;
	m_editRoomID.EnableWindow(FALSE);
	m_editGuestName.EnableWindow(TRUE);
}


BOOL CHotelSystemCheckoutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ����ʹ�÷���Ű�ťĬ��ѡ��
	(static_cast<CButton*>(GetDlgItem(IDC_RADIO1)))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}
