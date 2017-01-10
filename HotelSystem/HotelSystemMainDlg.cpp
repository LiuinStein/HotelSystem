// HotelSystemMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemMainDlg.h"
#include "HotelSystemCheckinDlg.h"
#include "HotelSystemCheckoutDlg.h"
#include "HotelSystemSearchGuestDlg.h"
#include "HotelSystemAboutDlg.h"
#include "HotelSystemExpense.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"

// CHotelSystemMainDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemMainDlg, CDialogEx)

// 用于刷新列表
void CHotelSystemMainDlg::RefreshList()
{
	// 清空列表
	if(m_listSplash.DeleteAllItems() == FALSE)
	{
		g_log.insertNewError(aduit::e_error, "DeleteAllItems清空列表失败", GetLastError());
		return;
	}
	// 清空向量
	m_vecSplashRoom.clear();
	// 在这里我要依据房间号来进行排序检索信息
	std::map<int, std::string> mapGuestIDToGuestName;
	try
	{
		CString sql = _T("SELECT id,typeid,guestid,dirty,checkouttime,unitprice FROM room ORDER BY id ASC");
		g_mysql.excuteQuery(sql);
		while(g_mysql.resultNext())
		{
			SSplashShow tmp;
			tmp.m_nRoomID = g_mysql.getResultSet()->getInt("id");
			tmp.m_nTypeID = g_mysql.getResultSet()->getInt("typeid");
			tmp.m_nGuestID = g_mysql.getResultSet()->getInt("guestid");
			tmp.m_bIsDirty = g_mysql.getResultSet()->getBoolean("dirty");
			tmp.m_strCheckoutTime = g_mysql.getResultSet()->getString("checkouttime");
			tmp.m_dPrice = g_mysql.getResultSet()->getDouble("unitprice");
			m_vecSplashRoom.push_back(tmp);
		}
		// 依据guestid获取宾客姓名
		sql = _T("SELECT id,name FROM guest");
		g_mysql.excuteQuery(sql);
		while (g_mysql.resultNext())
			mapGuestIDToGuestName[g_mysql.getResultSet()->getInt("id")] =
				g_mysql.getResultSet()->getString("name");
		for (int i = 0; i < m_vecSplashRoom.size(); i++)
			m_vecSplashRoom[i].m_strGuestName = 
			mapGuestIDToGuestName[m_vecSplashRoom[i].m_nGuestID];
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("从数据库中检索信息失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}
	// 检测映射关系
	if (g_mapRoomType.size() == 0)
		data::GetAllRoomType();
	// 将信息填充至列表
	CString cstrtmp;
	for (int i = 0; i < m_vecSplashRoom.size(); i++)
	{
		// 房间号
		cstrtmp.Format(_T("%d"), m_vecSplashRoom[i].m_nRoomID);
		m_listSplash.InsertItem(i, cstrtmp);
		// 房间类型
		cstrtmp = g_vecRoomType[m_vecSplashRoom[i].m_nTypeID - 1].m_nName.c_str();
		m_listSplash.SetItemText(i, 1, cstrtmp);
		// 单价
		cstrtmp.Format(_T("%.2lf"), m_vecSplashRoom[i].m_dPrice);
		m_listSplash.SetItemText(i, 2, cstrtmp);
		// 清洁状态
		cstrtmp = m_vecSplashRoom[i].m_bIsDirty ? _T("需要清洁") : _T("干净");
		m_listSplash.SetItemText(i, 3, cstrtmp);
		// 宾客姓名
		cstrtmp = mapGuestIDToGuestName[m_vecSplashRoom[i].m_nGuestID].c_str();
		m_listSplash.SetItemText(i, 4, cstrtmp);
		// 离店时间
		cstrtmp = m_vecSplashRoom[i].m_strCheckoutTime.c_str();
		m_listSplash.SetItemText(i, 5, cstrtmp);
	}
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
	ON_BN_CLICKED(IDC_BUTTON_EXPENSE, &CHotelSystemMainDlg::OnBnClickedButtonExpense)
	ON_BN_CLICKED(IDC_BUTTON_ABOUT, &CHotelSystemMainDlg::OnBnClickedButtonAbout)
END_MESSAGE_MAP()


// CHotelSystemMainDlg message handlers


BOOL CHotelSystemMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置list样式
	m_listSplash.SetExtendedStyle(m_listSplash.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 插入列
	m_listSplash.InsertColumn(0, _T("房间号"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(1, _T("房间类型"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(2, _T("售出时单价"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(3, _T("清洁状态"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(4, _T("宾客姓名"), LVCFMT_CENTER, 160, 50);
	m_listSplash.InsertColumn(5, _T("离店时间"), LVCFMT_CENTER, 160, 50);
	// 刷新列表
	RefreshList();
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

// 刷新按钮点击事件响应
void CHotelSystemMainDlg::OnBnClickedButtonRefresh()
{
	RefreshList();
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

// 续费及退款按钮事件响应
void CHotelSystemMainDlg::OnBnClickedButtonExpense()
{
	CHotelSystemExpense dlg;
	dlg.DoModal();
}

// 关于按钮事件响应
void CHotelSystemMainDlg::OnBnClickedButtonAbout()
{
	CHotelSystemAboutDlg dlg;
	dlg.DoModal();
}
