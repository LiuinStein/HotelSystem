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

// ����ˢ���б�
void CHotelSystemMainDlg::RefreshList()
{
	// ����б�
	if(m_listSplash.DeleteAllItems() == FALSE)
	{
		g_log.insertNewError(aduit::e_error, "DeleteAllItems����б�ʧ��", GetLastError());
		return;
	}
	// �������
	m_vecSplashRoom.clear();
	// ��������Ҫ���ݷ�������������������Ϣ
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
		// ����guestid��ȡ��������
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
		MessageBox(_T("�����ݿ��м�����Ϣʧ��"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}
	// ���ӳ���ϵ
	if (g_mapRoomType.size() == 0)
		data::GetAllRoomType();
	// ����Ϣ������б�
	CString cstrtmp;
	for (int i = 0; i < m_vecSplashRoom.size(); i++)
	{
		// �����
		cstrtmp.Format(_T("%d"), m_vecSplashRoom[i].m_nRoomID);
		m_listSplash.InsertItem(i, cstrtmp);
		// ��������
		cstrtmp = g_vecRoomType[m_vecSplashRoom[i].m_nTypeID - 1].m_nName.c_str();
		m_listSplash.SetItemText(i, 1, cstrtmp);
		// ����
		cstrtmp.Format(_T("%.2lf"), m_vecSplashRoom[i].m_dPrice);
		m_listSplash.SetItemText(i, 2, cstrtmp);
		// ���״̬
		cstrtmp = m_vecSplashRoom[i].m_bIsDirty ? _T("��Ҫ���") : _T("�ɾ�");
		m_listSplash.SetItemText(i, 3, cstrtmp);
		// ��������
		cstrtmp = mapGuestIDToGuestName[m_vecSplashRoom[i].m_nGuestID].c_str();
		m_listSplash.SetItemText(i, 4, cstrtmp);
		// ���ʱ��
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
	// ����list��ʽ
	m_listSplash.SetExtendedStyle(m_listSplash.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// ������
	m_listSplash.InsertColumn(0, _T("�����"), LVCFMT_CENTER, 100, 50);
	m_listSplash.InsertColumn(1, _T("��������"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(2, _T("�۳�ʱ����"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(3, _T("���״̬"), LVCFMT_CENTER, 140, 50);
	m_listSplash.InsertColumn(4, _T("��������"), LVCFMT_CENTER, 160, 50);
	m_listSplash.InsertColumn(5, _T("���ʱ��"), LVCFMT_CENTER, 160, 50);
	// ˢ���б�
	RefreshList();
	// ���ü�ʱ��ÿ��5���Ӹ���һ��
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

// ˢ�°�ť����¼���Ӧ
void CHotelSystemMainDlg::OnBnClickedButtonRefresh()
{
	RefreshList();
}

// �Ǽ���ס��ť����¼���Ӧ
void CHotelSystemMainDlg::OnBnClickedButtonCheckin()
{
	CHotelSystemCheckinDlg dlg;
	dlg.DoModal();
}

// ������갴ť����¼���Ӧ
void CHotelSystemMainDlg::OnBnClickedButtonCheckout()
{
	CHotelSystemCheckoutDlg dlg;
	dlg.DoModal();
}

// ������Ϣ��ť����¼���Ӧ
void CHotelSystemMainDlg::OnBnClickedButtonSearchguest()
{
	CHotelSystemSearchGuestDlg dlg;
	dlg.DoModal();
}

// ���Ѽ��˿ť�¼���Ӧ
void CHotelSystemMainDlg::OnBnClickedButtonExpense()
{
	CHotelSystemExpense dlg;
	dlg.DoModal();
}

// ���ڰ�ť�¼���Ӧ
void CHotelSystemMainDlg::OnBnClickedButtonAbout()
{
	CHotelSystemAboutDlg dlg;
	dlg.DoModal();
}

// �Ҽ�list�򵯳����״̬�˵�
void CHotelSystemMainDlg::OnRclickListSplash(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW * pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);
	m_nCilckListLine = pNMListView->iItem;	// ��ȡѡ������Ϣ
	//�����ǰû��ѡ�е��У��Ҽ��˵�����ʾ
	POSITION pos = m_listSplash.GetFirstSelectedItemPosition();
	if (m_listSplash.GetNextSelectedItem(pos) == -1)
		return;		//���û��ѡ�е���Ŀ������

	//��ʾ�����˵�
	CPoint point;
	GetCursorPos(&point);
	CMenu menu;
	menu.LoadMenu(IDR_MENU1);//��ȡ�˵�����Դ
	CMenu* popup = menu.GetSubMenu(0);//ֻ��ȡ��һ���в˵���ָ��
	//�����˵���ʾ
	popup->TrackPopupMenu(TPM_RIGHTBUTTON | TPM_LEFTALIGN, point.x, point.y, this);
	*pResult = 0;
}

// ���÷�������
void CHotelSystemMainDlg::SetRoomClear()
{
	SetRoomDirtyCondition(false);
}

// ���÷�����Ҫ��ɨ
void CHotelSystemMainDlg::SetRoomDirty()
{
	SetRoomDirtyCondition(true);
}

// ���÷��ݵ����״̬
void CHotelSystemMainDlg::SetRoomDirtyCondition(bool __isdirty)
{
	// ���Ȼ�ȡ����ID
	if (m_nCilckListLine == -1)
		return;
	CString cstrSelectRoomID = m_listSplash.GetItemText(m_nCilckListLine, 0);
	if (cstrSelectRoomID == _T(""))
		return;
	int nRoomID{ _ttoi(cstrSelectRoomID) };
	// ��vector�в����������
	int nRankRoomID{};
	for (int i = 0; i < m_vecSplashRoom.size(); i++)
		if (m_vecSplashRoom[i].m_nRoomID == nRoomID)
		{
			nRankRoomID = i;
			break;
		}
	// ���״̬һ�µĻ��Ͳ����޸���
	if (m_vecSplashRoom[nRankRoomID].m_bIsDirty == __isdirty)
		return;
	// ��һ�µ����
	m_vecSplashRoom[nRankRoomID].m_bIsDirty = __isdirty;
	// ���������ݿ�
	int nDirty{ __isdirty ? 1 : 0 };
	CString sql;
	sql.Format(_T("UPDATE room SET dirty=%d WHERE id=%d"), nDirty, m_vecSplashRoom[nRankRoomID].m_nRoomID);
	try
	{
		if (g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("��Ϣδ�ܳɹ����������ݿ�"), 0, MB_ICONERROR | MB_OK);
			return;
		}
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("��Ϣδ�ܳɹ����������ݿ�"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}
	RefreshList();
	m_nCilckListLine = -1;
}

// ��list�л�ȡroomid
int CHotelSystemMainDlg::GetRoomIDFromList()
{
	// ���Ȼ�ȡ����ID
	if (m_nCilckListLine == -1)
		return -1;
	CString cstrSelectRoomID = m_listSplash.GetItemText(m_nCilckListLine, 0);
	if (cstrSelectRoomID == _T(""))
		return -1;
	int nRoomID{ _ttoi(cstrSelectRoomID) };
	// ��vector�в����������
	int nRankRoomID{};
	for (int i = 0; i < m_vecSplashRoom.size(); i++)
		if (m_vecSplashRoom[i].m_nRoomID == nRoomID)
		{
			nRankRoomID = i;
			break;
		}
	// �����ǰ�����п��˵Ļ��ǲ��ܱ����
	if (m_vecSplashRoom[nRankRoomID].m_nGuestID != 0)
	{
		MessageBox(_T("��ǰ�����б���,�����޸ķ�������"), 0, MB_ICONERROR | MB_OK);
		return -1;
	}
	return nRoomID;
}

// ��ʱ��ÿ��5���Ӹ���һ���б�
void CHotelSystemMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	RefreshList();
	CDialogEx::OnTimer(nIDEvent);
}

// ����·�
void CHotelSystemMainDlg::AddNewRoom()
{
	CHotelSystemRoomPropretiesDlg dlg(true);
	dlg.DoModal();
	RefreshList();
}

// �޸ķ�������
void CHotelSystemMainDlg::ChangeRoomType()
{
	int nRoomID{ GetRoomIDFromList() };
	if (-1 == nRoomID)
		return;
	// �����Ϣ
	CHotelSystemRoomPropretiesDlg dlg(false, nRoomID);
	dlg.DoModal();
	RefreshList();
}

// ɾ������
void CHotelSystemMainDlg::DeleteRoom()
{
	if (IDNO == MessageBox(_T("ȷ��ɾ������Ĳ�����"), 0, MB_ICONINFORMATION | MB_YESNO))
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
			MessageBox(_T("���ݿ�ɾ��ʧ��"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, sql, GetLastError());
		}
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("���ݿ�ɾ��ʧ��"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return;
	}
	MessageBox(_T("ɾ������ɹ�"), 0, MB_ICONINFORMATION | MB_OK);
	data::GetAllItem();
	RefreshList();
}
