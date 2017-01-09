// HotelSystemCheckoutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemCheckoutDlg.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"


// CHotelSystemCheckoutDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemCheckoutDlg, CDialogEx)

CHotelSystemCheckoutDlg::CHotelSystemCheckoutDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHECKOUT, pParent), m_bUseRoomID(true), m_nGuestID(-1)
	,  m_dDeposit(-1)
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
	ON_BN_CLICKED(IDC_BUTTON_CHECKOUT, &CHotelSystemCheckoutDlg::OnBnClickedButtonCheckout)
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
	if (!(m_bUseRoomID ? SearchByRoomID() : SearchByGuestName()))
	{
		MessageBox(_T("δ��������ر�����Ϣ,���������Ƿ���ȷ"), 0, MB_ICONERROR | MB_OK);
		return;
	}
	CString tmp;	// ������Format
	if (g_mapRoomType.size() == 0 && !data::GetRoomTypeMap())
	{
		g_log.insertNewError(aduit::e_error, "GetRoomTypeMap()��ȡʧ��,����λ��CHotelSystemCheckoutDlg::OnBnClickedButtonIndex()��", GetLastError());
		return;
	}
	m_listInfo.DeleteAllItems();
	for (int i = 0; i < m_vecRoom.size(); i++)
	{
		tmp.Format(_T("%d"), m_vecRoom[i].m_basic.m_nRoomID);
		m_listInfo.InsertItem(i, tmp);	// �����
		tmp = g_mapRoomType[m_vecRoom[i].m_basic.m_nTypeID].c_str();
		m_listInfo.SetItemText(i, 1, tmp);	// ��������
		tmp.Format(_T("%.2lf"), m_vecRoom[i].m_basic.m_dUnitPrice);
		m_listInfo.SetItemText(i, 2, tmp);	// ����ʱ����
		tmp.Format(_T("%d"), m_vecRoom[i].m_nRemaining);
		m_listInfo.SetItemText(i, 3, tmp);	// ʣ������
		tmp.Format(_T("%lf"), m_vecRoom[i].m_dRefund);
		m_listInfo.SetItemText(i, 4, tmp);	// �˻����
	}
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

// ͨ������Ż�ȡ��Ϣ
bool CHotelSystemCheckoutDlg::SearchByRoomID()
{
	// ���ȴӴ����ȡroomid
	CString inpRoomID;
	m_editRoomID.GetWindowTextW(inpRoomID);
	if (inpRoomID == _T(""))
	{
		m_editRoomID.ShowBalloonTip(_T("��ʾ"), _T("���ֶα���"), TTI_INFO);
		return false;
	}
	m_vecRoom.clear();
	try
	{
		CString sql;
		sql.Format(_T("SELECT guestid FROM room WHERE id=%s"), inpRoomID.GetBuffer());
		inpRoomID.ReleaseBuffer();
		// ���Ȼ�ȡguestid
		g_mysql.excuteQuery(sql);
		if(!g_mysql.resultNext() || 0>= (m_nGuestID = g_mysql.getResultSet()->getInt("guestid")))
			return false;
		// ����guestid��ȡ������Ϣ
		return GetRoomInfoByGuestID() && GetDepositByGuestID();
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
}

// ͨ����������ȡ��Ϣ
bool CHotelSystemCheckoutDlg::SearchByGuestName()
{
	// �Ӵ����ȡ������
	CString inpGuestName;
	m_editGuestName.GetWindowTextW(inpGuestName);
	if (inpGuestName == _T(""))
	{
		m_editGuestName.ShowBalloonTip(_T("��ʾ"), _T("���ֶα���"), TTI_INFO);
		return false;
	}
	if(db::SQLIsBad(inpGuestName))
	{
		CString logtmp;
		logtmp.Format(_T("SQLע�빥��,ʹ�ò��� %s"), inpGuestName.GetBuffer());
		inpGuestName.ReleaseBuffer();
		g_log.insertNewError(aduit::e_error, logtmp, GetLastError());
		return false;
	}
	m_vecRoom.clear();
	try
	{
		CString sql;
		// �ȴ�guest���л�ȡguestid
		sql.Format(_T("SELECT id FROM guest WHERE name='%s'"), inpGuestName.GetBuffer());
		g_mysql.excuteQuery(sql);
		if (!g_mysql.resultNext() || 0 >= (m_nGuestID = g_mysql.getResultSet()->getInt("id")))
			return false;
		// �ͷŻ�����
		inpGuestName.ReleaseBuffer();
		// ����guestid��room���л�ȡ������Ϣ
		return GetRoomInfoByGuestID() && GetDepositByGuestID();
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
}

// ����guestid��room���л�ȡ������Ϣ
bool CHotelSystemCheckoutDlg::GetRoomInfoByGuestID()
{
	CString sql;
	sql.Format(_T("SELECT id,typeid,checkintime,checkouttime,unitprice FROM room WHERE guestid=%d"), m_nGuestID);
	try
	{
		g_mysql.excuteQuery(sql);
		CString cstrtmp;
		CTime tNow = CTime::GetCurrentTime();
		while (g_mysql.resultNext())
		{
			SRoomEx tmp;
			tmp.m_basic.m_nRoomID = g_mysql.getResultSet()->getInt("id");
			tmp.m_basic.m_nTypeID = g_mysql.getResultSet()->getInt("typeid");
			tmp.m_basic.m_tCheckinTime = g_mysql.getResultSet()->getString("checkintime");
			tmp.m_basic.m_tCheckoutTime = g_mysql.getResultSet()->getString("checkouttime");
			tmp.m_basic.m_dUnitPrice = g_mysql.getResultSet()->getDouble("unitprice");
			// ����ʣ������
			int nYear{}, nMonth{}, nDay{};
			sscanf_s(tmp.m_basic.m_tCheckoutTime.c_str(), "%d-%d-%d", &nYear, &nMonth, &nDay);
			CTime tCheckout(nYear, nMonth, nDay, 0, 0, 0);
			CTimeSpan tSpan = tCheckout - tNow;
			tmp.m_nRemaining = tSpan.GetDays();
			// ������������˷��Ļ�Ӧ���˻��Ľ��
			tmp.m_dRefund = tmp.m_nRemaining > 0 ?
				 tmp.m_nRemaining * tmp.m_basic.m_dUnitPrice : 0;
			m_vecRoom.push_back(tmp);
		}
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

// ����guestid��guest���л�ȡѺ����Ϣ
bool CHotelSystemCheckoutDlg::GetDepositByGuestID()
{
	CString sql;
	sql.Format(_T("SELECT deposit FROM guest WHERE id=%d"), m_nGuestID);
	try
	{
		g_mysql.excuteQuery(sql);
		m_dDeposit = g_mysql.resultNext() ?
			g_mysql.getResultSet()->getDouble("deposit") : -1;
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}


BOOL CHotelSystemCheckoutDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// ��ʼ���˵������б��
	// ���ȫ��ѡ�к�դ�����
	m_listInfo.SetExtendedStyle(m_listInfo.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// ������
	m_listInfo.InsertColumn(0, _T("�����"), LVCFMT_CENTER, 80, 50);
	m_listInfo.InsertColumn(1, _T("��������"), LVCFMT_CENTER, 90, 50);
	m_listInfo.InsertColumn(2, _T("����ʱ����"), LVCFMT_CENTER, 90, 50);
	m_listInfo.InsertColumn(3, _T("ʣ������"), LVCFMT_CENTER, 100, 50);
	m_listInfo.InsertColumn(4, _T("�˻����"), LVCFMT_CENTER, 100, 50);
	// ����ʹ�÷���Ű�ťĬ��ѡ��
	(static_cast<CButton*>(GetDlgItem(IDC_RADIO1)))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ȷ����갴ť�¼���Ӧ
void CHotelSystemCheckoutDlg::OnBnClickedButtonCheckout()
{
	// ȷ�����֮��Ӧ�����˻�Ѻ����������
	double dRefund{};
	for (int i = 0; i < m_vecRoom.size(); i++)
		dRefund += m_vecRoom[i].m_dRefund;
	CString cstrShow;
	cstrShow.Format(_T("�����˷����˻�Ѻ��%.2lfԪ,�������%.2lfԪ,�ϼ�%.2lfԪ,��ȷ����?"), m_dDeposit, dRefund, m_dDeposit + dRefund);
	if (IDNO == MessageBox(cstrShow, 0, MB_ICONINFORMATION | MB_YESNO))
		return;
	try
	{
		// ����room���еķ�����Ϣ
		CString sql;
		sql.Format(_T("UPDATE room SET guestid=0,dirty=0,checkintime=NULL,checkouttime=NULL,unitprice=0 WHERE guestid=%d"), m_nGuestID);
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("���ݿ����ʧ��"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "CHotelSystemCheckoutDlg::OnBnClickedButtonCheckout()���ݿ����ʧ��", GetLastError());
			return;
		}
		// ɾ��guest���еı��ͼ�¼
		sql.Format(_T("DELETE FROM guest WHERE id=%d"), m_nGuestID);
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("���ݿ����ʧ��"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "CHotelSystemCheckoutDlg::OnBnClickedButtonCheckout()���ݿ����ʧ��", GetLastError());
		}
		// ��ʾ��Ϣ
		MessageBox(_T("ȷ�����ɹ�"), 0, MB_ICONINFORMATION | MB_OK);
		if (MessageBox(_T("�Ƿ�رմ˴���"), 0, MB_ICONINFORMATION | MB_YESNO) == IDYES)
		{
			// �رմ���
			this->DestroyWindow();
			this->EndDialog(0);
		}
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}
}
