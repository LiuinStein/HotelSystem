// HotelSystemMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemMainDlg.h"
#include "afxdialogex.h"
#include "Log.h"
#include "mysql_conn.h"

// CHotelSystemMainDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemMainDlg, CDialogEx)

// ����ˢ���б�
void CHotelSystemMainDlg::refreshList()
{
	if(!m_listSplash.DeleteAllItems())
	{
		aduit::log.insertNewError(aduit::e_error, "DeleteAllItems����б�ʧ��", GetLastError());
		return;
	}
	// TODO: �����ݽṹ��ϵĸ߳�����,��ʱ����
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
END_MESSAGE_MAP()


// CHotelSystemMainDlg message handlers


BOOL CHotelSystemMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ������
	m_listSplash.InsertColumn(0, _T("�����"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(1, _T("��������"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(2, _T("����"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(3, _T("���״̬"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(4, _T("��������"), LVCFMT_CENTER, 160, 50);
	m_listSplash.InsertColumn(5, _T("���ʱ��"), LVCFMT_CENTER, 160, 50);

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