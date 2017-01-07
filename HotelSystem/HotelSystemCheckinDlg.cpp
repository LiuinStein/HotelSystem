// HotelSystemCheckinDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemCheckinDlg.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"
#include "Vector.h"

// CHotelSystemCheckinDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemCheckinDlg, CDialogEx)

CHotelSystemCheckinDlg::CHotelSystemCheckinDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHECKIN, pParent), m_bIsDiscounted(false)
{

}

CHotelSystemCheckinDlg::~CHotelSystemCheckinDlg()
{
}

void CHotelSystemCheckinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_GUESTNAME, m_editGuestName);
	DDX_Control(pDX, IDC_COMBO_SEX, m_comboGuestSex);
	DDX_Control(pDX, IDC_EDIT_YEAROLD, m_editGuestYearOld);
	DDX_Control(pDX, IDC_COMBO_IDCARDTYPE, m_comboGuestIDCardType);
	DDX_Control(pDX, IDC_EDIT_IDCARDID, m_editGuestIDCardID);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editGuestPhone);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_editGuestAddr);
	DDX_Control(pDX, IDC_EDIT_COMPANYNAME, m_editGuestCompName);
	DDX_Control(pDX, IDC_EDIT_COMPANYADD, m_editGuestCompAddr);
	DDX_Control(pDX, IDC_COMBO_ROOMTYPE, m_comboRoomType);
	DDX_Control(pDX, IDC_COMBO_FLOOR, m_comboRoomFloor);
	DDX_Control(pDX, IDC_COMBO_ROOMID, m_comboRoomNumber);
	DDX_Control(pDX, IDC_EDIT_STAYDAY, m_editStayTime);
	DDX_Control(pDX, IDC_EDIT_DISCOUNTED, m_editPayDiscounted);
	DDX_Control(pDX, IDC_EDIT_PREPAY, m_editPayPrePrice);
	DDX_Control(pDX, IDC_EDIT_SHOWDISCOUNTED, m_editPayLess);
	DDX_Control(pDX, IDC_EDIT13, m_editPayDeposite);
	DDX_Control(pDX, IDC_EDIT_TOTAL, m_editPayTotal);
	DDX_Control(pDX, IDC_COMBO6, m_comboPayMethod);
	DDX_Control(pDX, IDC_LIST_ACCOUNT, m_listAccount);
}


BEGIN_MESSAGE_MAP(CHotelSystemCheckinDlg, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_DISCOUNTED, &CHotelSystemCheckinDlg::OnBnClickedCheckDiscounted)
	ON_CBN_SELCHANGE(IDC_COMBO_ROOMTYPE, &CHotelSystemCheckinDlg::OnSelchangeComboRoomtype)
	ON_CBN_SELCHANGE(IDC_COMBO_FLOOR, &CHotelSystemCheckinDlg::OnSelchangeComboFloor)
END_MESSAGE_MAP()


// CHotelSystemCheckinDlg message handlers


BOOL CHotelSystemCheckinDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CHotelSystemCheckinDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_editPayDiscounted.EnableWindow(FALSE);
	// 性别下拉框
	m_comboGuestSex.InsertString(0, _T("男"));
	m_comboGuestSex.InsertString(1, _T("女"));
	// 证件类型
	m_comboGuestIDCardType.InsertString(0, _T("身份证"));
	m_comboGuestIDCardType.InsertString(1, _T("军人证"));
	m_comboGuestIDCardType.InsertString(2, _T("护照"));
	m_comboGuestIDCardType.InsertString(3, _T("港澳同胞证"));
	// 获取房间类型
	if(g_vecRoomType.size() == 0 && !data::GetAllRoomType())
	{
		MessageBox(_T("CHotelSystemCheckinDlg::OnInitDialog获取房间类型信息失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemCheckinDlg::OnInitDialog获取房间类型信息失败");
		EndDialog(IDCANCEL);
		return FALSE;
	}
	for (int i = 0; i < g_vecRoomType.size(); i++)
		m_comboRoomType.InsertString(i, CString(g_vecRoomType[i].m_nName.c_str()));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}


void CHotelSystemCheckinDlg::OnBnClickedCheckDiscounted()
{
	m_bIsDiscounted = !m_bIsDiscounted;
	m_editPayDiscounted.EnableWindow(m_bIsDiscounted ? TRUE : FALSE);
}

// 用户切换了房间类型选择框的选择
void CHotelSystemCheckinDlg::OnSelchangeComboRoomtype()
{
	// 当用户切换完成的时候,在这里初始化楼层信息组合框
	int nRoomTypeID{ m_comboRoomType.GetCurSel() + 1 };
	std::string sql("SELECT id FROM room WHERE typeid=");
	sql += char(nRoomTypeID + '0');
	sql += " ORDER BY id ASC";
	stl::CVector<int> vecRoomFloor;
	try
	{
		g_mysql.excuteQuery(sql);
		while (g_mysql.resultNext())
			vecRoomFloor.push_back(g_mysql.getResultSet()->getInt("id"));
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return;
	}
	m_comboRoomFloor.ResetContent();	// 清空之前的内容
	char tmp{};
	for (int i = 0, j = 0; i< vecRoomFloor.size(); i++)
		if (vecRoomFloor[i] / 100 != tmp)
		{
			tmp = vecRoomFloor[i] / 100;
			m_comboRoomFloor.InsertString(j++, CString(char(tmp + '0')));
		}
}

// 用户切换了楼层编号
void CHotelSystemCheckinDlg::OnSelchangeComboFloor()
{
	// 初始化可用房间号信息
	CString cstrFloor;
	m_comboRoomFloor.GetLBText(m_comboRoomFloor.GetCurSel(), cstrFloor);
	int nRoomTypeID{ m_comboRoomType.GetCurSel() + 1 };
	CString sql;
	sql.Format(_T("SELECT id FROM room WHERE id LIKE '%s__' AND guestid=0 AND typeid=%d ORDER BY id ASC"), cstrFloor.GetBuffer(), nRoomTypeID);
	cstrFloor.ReleaseBuffer();
	stl::CVector<int> vecRoomID;
	try
	{
		g_mysql.excuteQuery(sql);
		while (g_mysql.resultNext())
			vecRoomID.push_back(g_mysql.getResultSet()->getInt("id"));
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return;
	}
	m_comboRoomNumber.ResetContent();
	CString tmp;
	for (int i = 0; i < vecRoomID.size(); i++)
	{
		tmp.Format(_T("%d"), vecRoomID[i]);
		m_comboRoomNumber.InsertString(i, tmp);
	}
}
