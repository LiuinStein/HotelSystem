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

// 关闭窗体按钮事件响应
void CHotelSystemCheckoutDlg::OnBnClickedButtonQuit()
{
	this->DestroyWindow();
	this->EndDialog(0);
}

// 检索信息按钮事件响应
void CHotelSystemCheckoutDlg::OnBnClickedButtonIndex()
{
	if (!(m_bUseRoomID ? SearchByRoomID() : SearchByGuestName()))
	{
		MessageBox(_T("未检索到相关宾客信息,请检查输入是否正确"), 0, MB_ICONERROR | MB_OK);
		return;
	}
	CString tmp;	// 用来做Format
	if (g_mapRoomType.size() == 0 && !data::GetRoomTypeMap())
	{
		g_log.insertNewError(aduit::e_error, "GetRoomTypeMap()获取失败,错误位于CHotelSystemCheckoutDlg::OnBnClickedButtonIndex()中", GetLastError());
		return;
	}
	m_listInfo.DeleteAllItems();
	for (int i = 0; i < m_vecRoom.size(); i++)
	{
		tmp.Format(_T("%d"), m_vecRoom[i].m_basic.m_nRoomID);
		m_listInfo.InsertItem(i, tmp);	// 房间号
		tmp = g_mapRoomType[m_vecRoom[i].m_basic.m_nTypeID].c_str();
		m_listInfo.SetItemText(i, 1, tmp);	// 房间类型
		tmp.Format(_T("%.2lf"), m_vecRoom[i].m_basic.m_dUnitPrice);
		m_listInfo.SetItemText(i, 2, tmp);	// 购买时单价
		tmp.Format(_T("%d"), m_vecRoom[i].m_nRemaining);
		m_listInfo.SetItemText(i, 3, tmp);	// 剩余天数
		tmp.Format(_T("%lf"), m_vecRoom[i].m_dRefund);
		m_listInfo.SetItemText(i, 4, tmp);	// 退还金额
	}
}

// radio使用房间号
void CHotelSystemCheckoutDlg::OnBnClickedRadio1()
{
	m_bUseRoomID = true;
	m_editRoomID.EnableWindow(TRUE);
	m_editGuestName.EnableWindow(FALSE);
}

// radio使用姓名
void CHotelSystemCheckoutDlg::OnBnClickedRadio2()
{
	m_bUseRoomID = false;
	m_editRoomID.EnableWindow(FALSE);
	m_editGuestName.EnableWindow(TRUE);
}

// 通过房间号获取信息
bool CHotelSystemCheckoutDlg::SearchByRoomID()
{
	// 首先从窗体获取roomid
	CString inpRoomID;
	m_editRoomID.GetWindowTextW(inpRoomID);
	if (inpRoomID == _T(""))
	{
		m_editRoomID.ShowBalloonTip(_T("提示"), _T("此字段必填"), TTI_INFO);
		return false;
	}
	m_vecRoom.clear();
	try
	{
		CString sql;
		sql.Format(_T("SELECT guestid FROM room WHERE id=%s"), inpRoomID.GetBuffer());
		inpRoomID.ReleaseBuffer();
		// 首先获取guestid
		g_mysql.excuteQuery(sql);
		if(!g_mysql.resultNext() || 0>= (m_nGuestID = g_mysql.getResultSet()->getInt("guestid")))
			return false;
		// 依据guestid获取房间信息
		return GetRoomInfoByGuestID() && GetDepositByGuestID();
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
}

// 通过宾客名获取信息
bool CHotelSystemCheckoutDlg::SearchByGuestName()
{
	// 从窗体获取宾客名
	CString inpGuestName;
	m_editGuestName.GetWindowTextW(inpGuestName);
	if (inpGuestName == _T(""))
	{
		m_editGuestName.ShowBalloonTip(_T("提示"), _T("此字段必填"), TTI_INFO);
		return false;
	}
	if(db::SQLIsBad(inpGuestName))
	{
		CString logtmp;
		logtmp.Format(_T("SQL注入攻击,使用参数 %s"), inpGuestName.GetBuffer());
		inpGuestName.ReleaseBuffer();
		g_log.insertNewError(aduit::e_error, logtmp, GetLastError());
		return false;
	}
	m_vecRoom.clear();
	try
	{
		CString sql;
		// 先从guest表中获取guestid
		sql.Format(_T("SELECT id FROM guest WHERE name='%s'"), inpGuestName.GetBuffer());
		g_mysql.excuteQuery(sql);
		if (!g_mysql.resultNext() || 0 >= (m_nGuestID = g_mysql.getResultSet()->getInt("id")))
			return false;
		// 释放缓冲区
		inpGuestName.ReleaseBuffer();
		// 依据guestid从room表中获取房间信息
		return GetRoomInfoByGuestID() && GetDepositByGuestID();
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
}

// 依据guestid从room表中获取房间信息
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
			// 计算剩余天数
			int nYear{}, nMonth{}, nDay{};
			sscanf_s(tmp.m_basic.m_tCheckoutTime.c_str(), "%d-%d-%d", &nYear, &nMonth, &nDay);
			CTime tCheckout(nYear, nMonth, nDay, 0, 0, 0);
			CTimeSpan tSpan = tCheckout - tNow;
			tmp.m_nRemaining = tSpan.GetDays();
			// 计算如果现在退房的话应当退还的金额
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

// 依据guestid从guest表中获取押金信息
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
	// 初始化账单详情列表框
	// 标记全行选中和栅栏风格
	m_listInfo.SetExtendedStyle(m_listInfo.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 插入列
	m_listInfo.InsertColumn(0, _T("房间号"), LVCFMT_CENTER, 80, 50);
	m_listInfo.InsertColumn(1, _T("房间类型"), LVCFMT_CENTER, 90, 50);
	m_listInfo.InsertColumn(2, _T("购买时单价"), LVCFMT_CENTER, 90, 50);
	m_listInfo.InsertColumn(3, _T("剩余天数"), LVCFMT_CENTER, 100, 50);
	m_listInfo.InsertColumn(4, _T("退还金额"), LVCFMT_CENTER, 100, 50);
	// 设置使用房间号按钮默认选中
	(static_cast<CButton*>(GetDlgItem(IDC_RADIO1)))->SetCheck(TRUE);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// 确认离店按钮事件响应
void CHotelSystemCheckoutDlg::OnBnClickedButtonCheckout()
{
	// 确认离店之后应当先退还押金及其他费用
	double dRefund{};
	for (int i = 0; i < m_vecRoom.size(); i++)
		dRefund += m_vecRoom[i].m_dRefund;
	CString cstrShow;
	cstrShow.Format(_T("现在退房将退还押金%.2lf元,房间费用%.2lf元,合计%.2lf元,你确定吗?"), m_dDeposit, dRefund, m_dDeposit + dRefund);
	if (IDNO == MessageBox(cstrShow, 0, MB_ICONINFORMATION | MB_YESNO))
		return;
	try
	{
		// 更新room表中的房间信息
		CString sql;
		sql.Format(_T("UPDATE room SET guestid=0,dirty=0,checkintime=NULL,checkouttime=NULL,unitprice=0 WHERE guestid=%d"), m_nGuestID);
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("数据库更新失败"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "CHotelSystemCheckoutDlg::OnBnClickedButtonCheckout()数据库更新失败", GetLastError());
			return;
		}
		// 删除guest表中的宾客记录
		sql.Format(_T("DELETE FROM guest WHERE id=%d"), m_nGuestID);
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("数据库更新失败"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "CHotelSystemCheckoutDlg::OnBnClickedButtonCheckout()数据库更新失败", GetLastError());
		}
		// 提示信息
		MessageBox(_T("确认离店成功"), 0, MB_ICONINFORMATION | MB_OK);
		if (MessageBox(_T("是否关闭此窗口"), 0, MB_ICONINFORMATION | MB_YESNO) == IDYES)
		{
			// 关闭窗口
			this->DestroyWindow();
			this->EndDialog(0);
		}
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}
}
