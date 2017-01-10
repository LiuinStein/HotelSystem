// HotelSystemMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemMainDlg.h"
#include "HotelSystemCheckinDlg.h"
#include "HotelSystemCheckoutDlg.h"
#include "HotelSystemSearchGuestDlg.h"
#include "HotelSystemAboutDlg.h"
#include "HotelSystemRoomPropretiesDlg.h"
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
	: CDialogEx(IDD_DIALOG_MAIN, pParent), m_nCilckListLine(-1)
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
	ON_NOTIFY(NM_RCLICK, IDC_LIST_SPLASH, &CHotelSystemMainDlg::OnRclickListSplash)
	ON_COMMAND(ID__32773, &CHotelSystemMainDlg::SetRoomClear)
	ON_COMMAND(ID__32772, &CHotelSystemMainDlg::SetRoomDirty)
	ON_WM_TIMER()
	ON_COMMAND(ID_32775, &CHotelSystemMainDlg::AddNewRoom)
	ON_COMMAND(ID_32776, &CHotelSystemMainDlg::ChangeRoomType)
	ON_COMMAND(ID_32777, &CHotelSystemMainDlg::DeleteRoom)
END_MESSAGE_MAP()


// CHotelSystemMainDlg message handlers


BOOL CHotelSystemMainDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 设置list样式
	m_listSplash.SetExtendedStyle(m_listSplash.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 插入列
	m_listSplash.InsertColumn(0, _T("房间号"), LVCFMT_CENTER, 100, 50);
	m_listSplash.InsertColumn(1, _T("房间类型"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(2, _T("售出时单价"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(3, _T("清洁状态"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(4, _T("宾客姓名"), LVCFMT_CENTER, 160, 50);
	m_listSplash.InsertColumn(5, _T("离店时间"), LVCFMT_CENTER, 160, 50);
	// 刷新列表
	RefreshList();
	// 设置计时器每隔5分钟更新一次
	SetTimer(1, 5 * 60 * 1000, NULL);
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

// 登记入住按钮点击事件响应
void CHotelSystemMainDlg::OnBnClickedButtonCheckin()
{
	CHotelSystemCheckinDlg dlg;
	dlg.DoModal();
}

// 结账离店按钮点击事件响应
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

// 右键list框弹出清洁状态菜单
void CHotelSystemMainDlg::OnRclickListSplash(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW * pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);
	m_nCilckListLine = pNMListView->iItem;	// 获取选中行信息
	//如果当前没有选中的行，右键菜单不显示
	POSITION pos = m_listSplash.GetFirstSelectedItemPosition();
	if (m_listSplash.GetNextSelectedItem(pos) == -1)
		return;		//如果没有选中的项目，返回

	//显示弹出菜单
	CPoint point;
	GetCursorPos(&point);
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);//获取菜单的资源
	CMenu* popup = menu.GetSubMenu(0);//只获取第一个列菜单的指针
	//弹出菜单显示
	popup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}

// 设置房屋整洁
void CHotelSystemMainDlg::SetRoomClear()
{
	SetRoomDirtyCondition(false);
}

// 设置房屋需要打扫
void CHotelSystemMainDlg::SetRoomDirty()
{
	SetRoomDirtyCondition(true);
}

// 设置房屋的清洁状态
void CHotelSystemMainDlg::SetRoomDirtyCondition(bool __isdirty)
{
	// 首先获取房间ID
	if (m_nCilckListLine == -1)
		return;
	CString cstrSelectRoomID = m_listSplash.GetItemText(m_nCilckListLine, 0);
	if (cstrSelectRoomID == _T(""))
		return;
	int nRoomID{ _ttoi(cstrSelectRoomID) };
	// 在vector中查找这个房间
	int nRankRoomID{};
	for (int i = 0; i < m_vecSplashRoom.size(); i++)
		if (m_vecSplashRoom[i].m_nRoomID == nRoomID)
		{
			nRankRoomID = i;
			break;
		}
	// 清洁状态一致的话就不用修改了
	if (m_vecSplashRoom[nRankRoomID].m_bIsDirty == __isdirty)
		return;
	// 不一致的情况
	m_vecSplashRoom[nRankRoomID].m_bIsDirty = __isdirty;
	// 更新至数据库
	int nDirty{ __isdirty ? 1 : 0 };
	CString sql;
	sql.Format(_T("UPDATE room SET dirty=%d WHERE id=%d"), nDirty, m_vecSplashRoom[nRankRoomID].m_nRoomID);
	try
	{
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("信息未能成功更新至数据库"), 0, MB_ICONERROR | MB_OK);
			return;
		}
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("信息未能成功更新至数据库"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}
	RefreshList();
	m_nCilckListLine = -1;
}

// 从list中获取roomid
int CHotelSystemMainDlg::GetRoomIDFromList()
{
	// 首先获取房间ID
	if (m_nCilckListLine == -1)
		return -1;
	CString cstrSelectRoomID = m_listSplash.GetItemText(m_nCilckListLine, 0);
	if (cstrSelectRoomID == _T(""))
		return -1;
	int nRoomID{ _ttoi(cstrSelectRoomID) };
	// 在vector中查找这个房间
	int nRankRoomID{};
	for (int i = 0; i < m_vecSplashRoom.size(); i++)
		if (m_vecSplashRoom[i].m_nRoomID == nRoomID)
		{
			nRankRoomID = i;
			break;
		}
	// 如果当前房间有客人的话是不能变更的
	if (m_vecSplashRoom[nRankRoomID].m_nGuestID != 0)
	{
		MessageBox(_T("当前房间有宾客,不能修改房间类型"), 0, MB_ICONERROR | MB_OK);
		return -1;
	}
	return nRoomID;
}

// 计时器每隔5分钟更新一次列表
void CHotelSystemMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	RefreshList();
	CDialogEx::OnTimer(nIDEvent);
}

// 添加新房
void CHotelSystemMainDlg::AddNewRoom()
{
	CHotelSystemRoomPropretiesDlg dlg(true);
	dlg.DoModal();
	RefreshList();
}

// 修改房间类型
void CHotelSystemMainDlg::ChangeRoomType()
{
	int nRoomID{ GetRoomIDFromList() };
	if (-1 == nRoomID)
		return;
	// 变更信息
	CHotelSystemRoomPropretiesDlg dlg(false, nRoomID);
	dlg.DoModal();
	RefreshList();
}

// 删除房间
void CHotelSystemMainDlg::DeleteRoom()
{
	if (IDNO == MessageBox(_T("确认删除房间的操作吗"), 0, MB_ICONINFORMATION | MB_YESNO))
		return;
	int nRoomID{ GetRoomIDFromList() };
	if (-1 == nRoomID)
		return;
	CString sql;
	sql.Format(_T("DELETE FROM room WHERE id=%d"), nRoomID);
	try
	{
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("数据库删除失败"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, sql, GetLastError());
		}
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("数据库删除失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return;
	}
	MessageBox(_T("删除房间成功"), 0, MB_ICONINFORMATION | MB_OK);
	data::GetAllItem();
	RefreshList();
}
