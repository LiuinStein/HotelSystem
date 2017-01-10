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

// 依据房间号进行信息检索
void CHotelSystemExpense::OnBnClickedButtonSearch()
{
	// 从页面获取房间号
	CString cstrRoomID;
	m_editRoomID.GetWindowTextW(cstrRoomID);
	if(cstrRoomID == _T(""))
	{
		m_editRoomID.ShowBalloonTip(_T("提示"), _T("此字段必填"), TTI_INFO);
		return;
	}
	m_nRoomID = _ttoi(cstrRoomID);
	try
	{
		// 从room表获取必要信息
		CString sql;
		sql.Format(_T("SELECT typeid,guestid,checkintime,checkouttime,unitprice FROM room WHERE id=%d"), m_nRoomID);
		g_mysql.excuteQuery(sql);
		if (!g_mysql.resultNext())
		{
			MessageBox(_T("请确认输入房间号正确"), 0, MB_ICONERROR | MB_OK);
			return;
		}
		m_nGuestID = g_mysql.getResultSet()->getInt("guestid");
		if(m_nGuestID == 0)
		{
			MessageBox(_T("此房间未登记入住"), 0, MB_ICONERROR | MB_OK);
			return;
		}
		m_nTypeID = g_mysql.getResultSet()->getInt("typeid");
		m_ctRoomChkInDate = StringToCTime(g_mysql.getResultSet()->getString("checkintime"));
		m_ctRoomChkOutDate = StringToCTime(g_mysql.getResultSet()->getString("checkouttime"));
		m_dUnitPrice = g_mysql.getResultSet()->getDouble("unitprice");
		// 从guest表中获取顾客最晚离店时间
		sql.Format(_T("SELECT checkouttime FROM guest WHERE id=%d"), m_nGuestID);
		g_mysql.excuteQuery(sql);
		if (!g_mysql.resultNext())
		{
			MessageBox(_T("用户离店时间检索失败"), 0, MB_ICONERROR | MB_OK);
			return;
		}
		m_ctGuestChkOutDate = StringToCTime(g_mysql.getResultSet()->getString("checkouttime"));
		m_editOldCheckoutTime.SetWindowTextW(m_ctRoomChkOutDate.Format("%Y-%m-%d"));
		m_nGuestMaxStayRoomID = GetMaxStayTimeRoomID();
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("数据库信息检索异常"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}

}

// std::string类型转CTime,字符串格式年-月-日 小时:分钟:秒
CTime CHotelSystemExpense::StringToCTime(const std::string& __str)
{
	int nYear{}, nMonth{}, nDay{}, nHour{}, nMinute{}, nSec{};
	sscanf_s(__str.c_str(), "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSec);
	return CTime(nYear, nMonth, nDay, nHour, nMinute, nSec);
}

// 从数据库中获取当前宾客预留时间最长的房间的ID
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

// 更新guest表中的客人离店时间,函数始终更新至最大时间
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

// 将数据更新至room表
bool CHotelSystemExpense::UpdateCheckOutTimeToRoom()
{
	// 将数据更新至数据库
	CString cstrNewTime;
	cstrNewTime.Format(_T("%d-%d-%d %d:%d:%d"), m_ctNewDate.GetYear(), m_ctNewDate.GetMonth(), m_ctNewDate.GetDay()
		, m_ctRoomChkOutDate.GetHour(), m_ctRoomChkOutDate.GetMinute(), m_ctRoomChkOutDate.GetSecond());
	try
	{
		// 先来更新一下room表中的信息
		CString sql;
		sql.Format(_T("UPDATE room SET checkouttime='%s' WHERE id=%d"), cstrNewTime.GetBuffer(), m_nRoomID);
		cstrNewTime.ReleaseBuffer();
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("数据未成功保存至数据库"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "CHotelSystemExpense::UpdateCheckOutTimeToRoom()数据未成功保存至数据库", GetLastError());
			return false;
		}
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("数据更新至数据库出现问题"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

// 复选框点击事件响应
void CHotelSystemExpense::OnBnClickedCheckIsdiscounted()
{
	m_bIsDiscounted = !m_bIsDiscounted;
	m_editDiscounted.EnableWindow(m_bIsDiscounted ? TRUE : FALSE);
}

// 确认保存按钮事件响应
void CHotelSystemExpense::OnBnClickedButtonSave()
{
	// 获取更新之后的时间
	m_dtNewTime.GetTime(m_ctNewDate);
	CTimeSpan tsTmp;
	tsTmp = m_ctNewDate - m_ctRoomChkOutDate;
	// 更新后的时间不能小于现在的时间
	if(CTimeSpan(m_ctNewDate - CTime::GetCurrentTime()).GetTotalSeconds() <= 0)
	{
		MessageBox(_T("更新后的时间不能小于现在的时间"), 0, MB_ICONERROR | MB_OK);
		return;
	}
	// 更新
	if(tsTmp.GetTotalSeconds() > 0)
	{
		// 续费
		// 折扣信息
		double dDiscounted{ 1 };
		if(m_bIsDiscounted)
		{
			// 如果打折的话从界面获取折扣信息
			CString cstrDiscounted;
			m_editDiscounted.GetWindowTextW(cstrDiscounted);
			if(cstrDiscounted == _T(""))
			{
				m_editDiscounted.ShowBalloonTip(_T("提示"), _T("此字段必填"), TTI_INFO);
				return;
			}
			int nTmp{ _ttoi(cstrDiscounted) };
			if (nTmp > 10 || nTmp <= 0)
			{
				m_editDiscounted.ShowBalloonTip(_T("提示"), _T("折扣信息必需小于10且大于0"), TTI_INFO);
				return;
			}
			dDiscounted = double(nTmp) / 10.0;
		}
		// 续费的话,查询room_type获取房间原价
		try
		{
			CString sql;
			sql.Format(_T("SELECT price FROM room_type WHERE id=%d"), m_nTypeID);
			g_mysql.excuteQuery(sql);
			if (!g_mysql.resultNext())
			{
				MessageBox(_T("获取房间价格信息失败-1"), 0, MB_ICONERROR | MB_OK);
				return;
			}
			m_dRoomPrice = g_mysql.getResultSet()->getDouble("price");
		}
		catch (const sql::SQLException &e)
		{
			MessageBox(_T("获取房间价格信息失败-2"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
			return;
		}
		double dTotal = m_dRoomPrice*dDiscounted*tsTmp.GetDays();
		double dDiscount = m_dRoomPrice*(1 - dDiscounted)*tsTmp.GetDays();
		CString msg;
		msg.Format(_T("您将要延长房间%d的退房时间, 需要补交%.2lf元的费用,通过折扣为您省下了%.2lf元,您确定吗"),
			m_nRoomID, dTotal, dDiscount);
		if (MessageBox(msg, 0, MB_YESNO | MB_ICONINFORMATION) == IDNO)
			return;
	}
	else if (tsTmp.GetTotalSeconds() < 0)
	{
		// 退款
		long long nDays{ -tsTmp.GetDays() };
		double nRefund{ nDays * m_dUnitPrice };
		CString msg;
		msg.Format(_T("您减少了%d房间的预留时间,将会获得%.2lf元的退款,您确定吗"), m_nRoomID, nRefund);
		if (MessageBox(msg, 0, MB_ICONINFORMATION | MB_YESNO) == IDNO)
			return;
	}
	else
	{
		MessageBox(_T("预留时间并没有改变"), 0, MB_ICONINFORMATION | MB_OK);
		return;
	}

	// 将数据更新至数据库
	UpdateCheckOutTimeToRoom();
	// 更新一下当前的最长时间房间id
	if (-1 == (m_nGuestMaxStayRoomID = GetMaxStayTimeRoomID()))
		return;
	// 如果当前房间正好是客人预留时间最长的房间,那么更新guest表中的信息
	if (m_nGuestMaxStayRoomID == m_nRoomID)
		UpdateCheckoutTimeToGuest();
	// 重新获取m_ctRoomChkOutDate
	m_ctRoomChkOutDate = m_ctNewDate;
	m_editOldCheckoutTime.SetWindowTextW(m_ctRoomChkOutDate.Format("%Y-%m-%d"));
	MessageBox(_T("信息更新成功"), 0, MB_OK | MB_ICONINFORMATION);
}
