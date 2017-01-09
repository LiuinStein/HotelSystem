// HotelSystemMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemMainDlg.h"
#include "HotelSystemCheckinDlg.h"
#include "HotelSystemCheckoutDlg.h"
#include "HotelSystemSearchGuestDlg.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"

// CHotelSystemMainDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemMainDlg, CDialogEx)

// 用于刷新列表
void CHotelSystemMainDlg::refreshList()
{
	if(!m_listSplash.DeleteAllItems())
	{
		g_log.insertNewError(aduit::e_error, "DeleteAllItems清空列表失败", GetLastError());
		return;
	}
	// TODO: 与数据结构结合的高潮部分,暂时忽略
}

CHotelSystemMainDlg::CHotelSystemMainDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_MAIN, pParent)
{

}

CHotelSystemMainDlg::~CHotelSystemMainDlg()
{
}

void CHotelSystemMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SPLASH, m_listSplash);
}


BEGIN_MESSAGE_MAP(CHotelSystemMainDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CHotelSystemMainDlg::OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_CHECKIN, &CHotelSystemMainDlg::OnBnClickedButtonCheckin)
	ON_BN_CLICKED(IDC_BUTTON_CHECKOUT, &CHotelSystemMainDlg::OnBnClickedButtonCheckout)
	ON_BN_CLICKED(IDC_BUTTON_SEARCHGUEST, &CHotelSystemMainDlg::OnBnClickedButtonSearchguest)
END_MESSAGE_MAP()


// CHotelSystemMainDlg message handlers


BOOL CHotelSystemMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 插入列
	m_listSplash.InsertColumn(0, _T("房间号"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(1, _T("房间类型"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(2, _T("单价"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(3, _T("清洁状态"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(4, _T("宾客姓名"), LVCFMT_CENTER, 160, 50);
	m_listSplash.InsertColumn(5, _T("离店时间"), LVCFMT_CENTER, 160, 50);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CHotelSystemMainDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN &&  pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CHotelSystemMainDlg::OnClose()
{
	exit(0);
}


void CHotelSystemMainDlg::OnBnClickedButtonRefresh()
{
	refreshList();
}


void CHotelSystemMainDlg::OnBnClickedButtonCheckin()
{
	CHotelSystemCheckinDlg dlg;
	dlg.DoModal();
}


void CHotelSystemMainDlg::OnBnClickedButtonCheckout()
{
	CHotelSystemCheckoutDlg dlg;
	dlg.DoModal();
}

// 客人信息按钮点击事件响应
void CHotelSystemMainDlg::OnBnClickedButtonSearchguest()
{
	CHotelSystemSearchGuestDlg dlg;
	dlg.DoModal();
}
