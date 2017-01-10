// HotelSystemExpense.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemExpense.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"


// CHotelSystemExpense dialog

IMPLEMENT_DYNAMIC(CHotelSystemExpense, CDialogEx)

CHotelSystemExpense::CHotelSystemExpense(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_EXPENSE, pParent), m_bIsDiscounted(false)
{

}

CHotelSystemExpense::~CHotelSystemExpense()
{
}

void CHotelSystemExpense::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_editRoomID);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dtNewTime);
	DDX_Control(pDX, IDC_EDIT_DISCOUNTED, m_editDiscounted);
	DDX_Control(pDX, IDC_EDIT3, m_editOldCheckoutTime);
}


BEGIN_MESSAGE_MAP(CHotelSystemExpense, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CHotelSystemExpense::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_CHECK_ISDISCOUNTED, &CHotelSystemExpense::OnBnClickedCheckIsdiscounted)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CHotelSystemExpense::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CHotelSystemExpense message handlers


BOOL CHotelSystemExpense::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_dtNewTime.SetFormat(_T("yyyy-MM-dd"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


BOOL CHotelSystemExpense::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN &&  pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

// ���ݷ���Ž�����Ϣ����
void CHotelSystemExpense::OnBnClickedButtonSearch()
{
	// ��ҳ���ȡ�����
	CString cstrRoomID;
	m_editRoomID.GetWindowTextW(cstrRoomID);
	if(cstrRoomID == _T(""))
	{
		m_editRoomID.ShowBalloonTip(_T("��ʾ"), _T("���ֶα���"), TTI_INFO);
		return;
	}
	m_nRoomID = _ttoi(cstrRoomID);
	try
	{
		// ��room���ȡ��Ҫ��Ϣ
		CString sql;
		sql.Format(_T("SELECT typeid,guestid,checkintime,checkouttime,unitprice FROM room WHERE id=%d"), m_nRoomID);
		g_mysql.excuteQuery(sql);
		if (!g_mysql.resultNext())
		{
			MessageBox(_T("��ȷ�����뷿�����ȷ"), 0, MB_ICONERROR | MB_OK);
			return;
		}
		m_nGuestID = g_mysql.getResultSet()->getInt("guestid");
		if(m_nGuestID == 0)
		{
			MessageBox(_T("�˷���δ�Ǽ���ס"), 0, MB_ICONERROR | MB_OK);
			return;
		}
		m_nTypeID = g_mysql.getResultSet()->getInt("typeid");
		m_ctRoomChkInDate = StringToCTime(g_mysql.getResultSet()->getString("checkintime"));
		m_ctRoomChkOutDate = StringToCTime(g_mysql.getResultSet()->getString("checkouttime"));
		m_dUnitPrice = g_mysql.getResultSet()->getDouble("unitprice");
		// ��guest���л�ȡ�˿��������ʱ��
		sql.Format(_T("SELECT checkouttime FROM guest WHERE id=%d"), m_nGuestID);
		g_mysql.excuteQuery(sql);
		if (!g_mysql.resultNext())
		{
			MessageBox(_T("�û����ʱ�����ʧ��"), 0, MB_ICONERROR | MB_OK);
			return;
		}
		m_ctGuestChkOutDate = StringToCTime(g_mysql.getResultSet()->getString("checkouttime"));
		m_editOldCheckoutTime.SetWindowTextW(m_ctRoomChkOutDate.Format("%Y-%m-%d"));
		m_nGuestMaxStayRoomID = GetMaxStayTimeRoomID();
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("���ݿ���Ϣ�����쳣"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}

}

// std::string����תCTime,�ַ�����ʽ��-��-�� Сʱ:����:��
CTime CHotelSystemExpense::StringToCTime(const std::string& __str)
{
	int nYear{}, nMonth{}, nDay{}, nHour{}, nMinute{}, nSec{};
	sscanf_s(__str.c_str(), "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSec);
	return CTime(nYear, nMonth, nDay, nHour, nMinute, nSec);
}

// �����ݿ��л�ȡ��ǰ����Ԥ��ʱ����ķ����ID
int CHotelSystemExpense::GetMaxStayTimeRoomID()
{
	if (m_nGuestID <= 0)
		return -1;
	CString sql;
	sql.Format(_T("SELECT id FROM room WHERE guestid=%d AND checkouttime LIKE '%d%%%d%%%d%%'")
		, m_nGuestID, m_ctGuestChkOutDate.GetYear(), m_ctGuestChkOutDate.GetMonth()
		, m_ctGuestChkOutDate.GetDay());
	try
	{
		g_mysql.excuteQuery(sql);
		if (!g_mysql.resultNext())
			return -1;
		return g_mysql.getResultSet()->getInt("id");
	}
	catch (const sql::SQLException &e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return -1;
	}
}

// ����guest���еĿ������ʱ��,����ʼ�ո��������ʱ��
bool CHotelSystemExpense::UpdateCheckoutTimeToGuest()
{
	CString sql;
	sql.Format(_T("UPDATE guest SET checkouttime=(SELECT MAX(checkouttime) FROM room WHERE guestid=%d) WHERE id=%d")
		, m_nGuestID, m_nGuestID);
	try
	{
		if (g_mysql.excuteUpdate(sql) <= 0)
			return false;
	}
	catch (const sql::SQLException &e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

// �����ݸ�����room��
bool CHotelSystemExpense::UpdateCheckOutTimeToRoom()
{
	// �����ݸ��������ݿ�
	CString cstrNewTime;
	cstrNewTime.Format(_T("%d-%d-%d %d:%d:%d"), m_ctNewDate.GetYear(), m_ctNewDate.GetMonth(), m_ctNewDate.GetDay()
		, m_ctRoomChkOutDate.GetHour(), m_ctRoomChkOutDate.GetMinute(), m_ctRoomChkOutDate.GetSecond());
	try
	{
		// ��������һ��room���е���Ϣ
		CString sql;
		sql.Format(_T("UPDATE room SET checkouttime='%s' WHERE id=%d"), cstrNewTime.GetBuffer(), m_nRoomID);
		cstrNewTime.ReleaseBuffer();
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("����δ�ɹ����������ݿ�"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "CHotelSystemExpense::UpdateCheckOutTimeToRoom()����δ�ɹ����������ݿ�", GetLastError());
			return false;
		}
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("���ݸ��������ݿ��������"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

// ��ѡ�����¼���Ӧ
void CHotelSystemExpense::OnBnClickedCheckIsdiscounted()
{
	m_bIsDiscounted = !m_bIsDiscounted;
	m_editDiscounted.EnableWindow(m_bIsDiscounted ? TRUE : FALSE);
}

// ȷ�ϱ��水ť�¼���Ӧ
void CHotelSystemExpense::OnBnClickedButtonSave()
{
	// ��ȡ����֮���ʱ��
	m_dtNewTime.GetTime(m_ctNewDate);
	CTimeSpan tsTmp;
	tsTmp = m_ctNewDate - m_ctRoomChkOutDate;
	// ���º��ʱ�䲻��С�����ڵ�ʱ��
	if(CTimeSpan(m_ctNewDate - CTime::GetCurrentTime()).GetTotalSeconds() <= 0)
	{
		MessageBox(_T("���º��ʱ�䲻��С�����ڵ�ʱ��"), 0, MB_ICONERROR | MB_OK);
		return;
	}
	// ����
	if(tsTmp.GetTotalSeconds() > 0)
	{
		// ����
		// �ۿ���Ϣ
		double dDiscounted{ 1 };
		if(m_bIsDiscounted)
		{
			// ������۵Ļ��ӽ����ȡ�ۿ���Ϣ
			CString cstrDiscounted;
			m_editDiscounted.GetWindowTextW(cstrDiscounted);
			if(cstrDiscounted == _T(""))
			{
				m_editDiscounted.ShowBalloonTip(_T("��ʾ"), _T("���ֶα���"), TTI_INFO);
				return;
			}
			int nTmp{ _ttoi(cstrDiscounted) };
			if (nTmp > 10 || nTmp <= 0)
			{
				m_editDiscounted.ShowBalloonTip(_T("��ʾ"), _T("�ۿ���Ϣ����С��10�Ҵ���0"), TTI_INFO);
				return;
			}
			dDiscounted = double(nTmp) / 10.0;
		}
		// ���ѵĻ�,��ѯroom_type��ȡ����ԭ��
		try
		{
			CString sql;
			sql.Format(_T("SELECT price FROM room_type WHERE id=%d"), m_nTypeID);
			g_mysql.excuteQuery(sql);
			if (!g_mysql.resultNext())
			{
				MessageBox(_T("��ȡ����۸���Ϣʧ��-1"), 0, MB_ICONERROR | MB_OK);
				return;
			}
			m_dRoomPrice = g_mysql.getResultSet()->getDouble("price");
		}
		catch (const sql::SQLException &e)
		{
			MessageBox(_T("��ȡ����۸���Ϣʧ��-2"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
			return;
		}
		double dTotal = m_dRoomPrice*dDiscounted*tsTmp.GetDays();
		double dDiscount = m_dRoomPrice*(1 - dDiscounted)*tsTmp.GetDays();
		CString msg;
		msg.Format(_T("����Ҫ�ӳ�����%d���˷�ʱ��, ��Ҫ����%.2lfԪ�ķ���,ͨ���ۿ�Ϊ��ʡ����%.2lfԪ,��ȷ����"),
			m_nRoomID, dTotal, dDiscount);
		if (MessageBox(msg, 0, MB_YESNO | MB_ICONINFORMATION) == IDNO)
			return;
	}
	else if (tsTmp.GetTotalSeconds() < 0)
	{
		// �˿�
		long long nDays{ -tsTmp.GetDays() };
		double nRefund{ nDays * m_dUnitPrice };
		CString msg;
		msg.Format(_T("��������%d�����Ԥ��ʱ��,������%.2lfԪ���˿�,��ȷ����"), m_nRoomID, nRefund);
		if (MessageBox(msg, 0, MB_ICONINFORMATION | MB_YESNO) == IDNO)
			return;
	}
	else
	{
		MessageBox(_T("Ԥ��ʱ�䲢û�иı�"), 0, MB_ICONINFORMATION | MB_OK);
		return;
	}

	// �����ݸ��������ݿ�
	UpdateCheckOutTimeToRoom();
	// ����һ�µ�ǰ���ʱ�䷿��id
	if (-1 == (m_nGuestMaxStayRoomID = GetMaxStayTimeRoomID()))
		return;
	// �����ǰ���������ǿ���Ԥ��ʱ����ķ���,��ô����guest���е���Ϣ
	if (m_nGuestMaxStayRoomID == m_nRoomID)
		UpdateCheckoutTimeToGuest();
	// ���»�ȡm_ctRoomChkOutDate
	m_ctRoomChkOutDate = m_ctNewDate;
	m_editOldCheckoutTime.SetWindowTextW(m_ctRoomChkOutDate.Format("%Y-%m-%d"));
	MessageBox(_T("��Ϣ���³ɹ�"), 0, MB_OK | MB_ICONINFORMATION);
}
