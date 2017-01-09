// HotelSystemSearchGuestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemSearchGuestDlg.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"


// CHotelSystemSearchGuestDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemSearchGuestDlg, CDialogEx)

CHotelSystemSearchGuestDlg::CHotelSystemSearchGuestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_SEARCHGUEST, pParent), m_bOnChange(false), m_bUseRoomID(true)
{
	
}

CHotelSystemSearchGuestDlg::~CHotelSystemSearchGuestDlg()
{
}

void CHotelSystemSearchGuestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ROOMID, m_editRoomID);
	DDX_Control(pDX, IDC_EDIT_GUESTNAME, m_editGuestName);
	DDX_Control(pDX, IDC_EDIT_GNAME, m_editGName);
	DDX_Control(pDX, IDC_COMBO_GSEX, m_comboGSex);
	DDX_Control(pDX, IDC_EDIT_YEAR, m_editGYear);
	DDX_Control(pDX, IDC_EDIT_COMPNAME, m_editGCompName);
	DDX_Control(pDX, IDC_EDIT_COMPADDR, m_editGCompAddr);
	DDX_Control(pDX, IDC_COMBO_IDCARDTYPE, m_comboGIDCardType);
	DDX_Control(pDX, IDC_EDIT_IDCARDID, m_editGIDCardID);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_editGPhone);
	DDX_Control(pDX, IDC_EDIT7_ADDR, m_editGAddr);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_buttonSave);
}


BEGIN_MESSAGE_MAP(CHotelSystemSearchGuestDlg, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_ROOMID, &CHotelSystemSearchGuestDlg::OnBnClickedRadioRoomid)
	ON_BN_CLICKED(IDC_RADIO_NAME, &CHotelSystemSearchGuestDlg::OnBnClickedRadioName)
	ON_BN_CLICKED(IDC_CHECK_ONCHANGE, &CHotelSystemSearchGuestDlg::OnBnClickedCheckOnchange)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CHotelSystemSearchGuestDlg::OnBnClickedButtonSearch)
	ON_BN_CLICKED(IDC_BUTTON_RELOAD, &CHotelSystemSearchGuestDlg::OnBnClickedButtonReload)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CHotelSystemSearchGuestDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CHotelSystemSearchGuestDlg message handlers


BOOL CHotelSystemSearchGuestDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CHotelSystemSearchGuestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// 初始化单选框
	(static_cast<CButton*>(GetDlgItem(IDC_RADIO_ROOMID)))->SetCheck(TRUE);
	// 初始化一个非法值
	m_guest.m_nID = -1;
	// 初始化下拉框
	m_comboGSex.InsertString(0, _T("男"));
	m_comboGSex.InsertString(1, _T("女"));
	m_comboGIDCardType.InsertString(0, _T("身份证"));
	m_comboGIDCardType.InsertString(1, _T("军人证"));
	m_comboGIDCardType.InsertString(2, _T("护照"));
	m_comboGIDCardType.InsertString(3, _T("港澳同胞证"));

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// radio_通过房间号检索
void CHotelSystemSearchGuestDlg::OnBnClickedRadioRoomid()
{
	m_bUseRoomID = true;
	m_editRoomID.EnableWindow(TRUE);
	m_editGuestName.EnableWindow(FALSE);
}

// radio_通过姓名检索
void CHotelSystemSearchGuestDlg::OnBnClickedRadioName()
{
	m_bUseRoomID = false;
	m_editRoomID.EnableWindow(FALSE);
	m_editGuestName.EnableWindow(TRUE);
}

// check_启用修改
void CHotelSystemSearchGuestDlg::OnBnClickedCheckOnchange()
{
	if(m_guest.m_nID == -1)
	{
		MessageBox(_T("请先获取宾客信息"), 0, MB_ICONERROR | MB_OK);
		static_cast<CButton*>(GetDlgItem(IDC_CHECK_ONCHANGE))->SetCheck(FALSE);
		return;
	}
	m_bOnChange = !m_bOnChange;
	EnableChangeInfo(m_bOnChange ? TRUE : FALSE);
}

// 当__b为TRUE时启用控件,为FALSE时禁用控件
void CHotelSystemSearchGuestDlg::EnableChangeInfo(BOOL __b)
{
	m_editGName.EnableWindow(__b);
	m_comboGSex.EnableWindow(__b);
	m_editGYear.EnableWindow(__b);
	m_editGCompName.EnableWindow(__b);
	m_editGCompAddr.EnableWindow(__b);
	m_comboGIDCardType.EnableWindow(__b);
	m_editGIDCardID.EnableWindow(__b);
	m_editGPhone.EnableWindow(__b);
	m_editGAddr.EnableWindow(__b);
	m_buttonSave.EnableWindow(__b);
}

// 通过房间id获取宾客信息
bool CHotelSystemSearchGuestDlg::GetInfoByRoomID()
{
	CString tmp;
	m_editRoomID.GetWindowTextW(tmp);
	if (tmp == _T(""))
	{
		m_editRoomID.ShowBalloonTip(_T("提示"), _T("请填写此字段"), TTI_INFO);
		return false;
	}
	// 先查room表获取guestid
	CString sqlWhere;
	sqlWhere.Format(_T("SELECT guestid FROM room WHERE id=%s"), tmp.GetBuffer());
	tmp.ReleaseBuffer();
	try
	{
		g_mysql.excuteQuery(sqlWhere);
		if (!g_mysql.resultNext())
		{
			MessageBox(_T("从数据库获取信息失败"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "结果集为空", GetLastError());
			return false;
		}
		m_guest.m_nID = g_mysql.getResultSet()->getInt("guestid");
	}
	catch (const sql::SQLException & e)
	{
		MessageBox(_T("从数据库获取信息失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	if (!GetInfoFromDBByID(m_guest.m_nID))
		return false;
	ShowInfoOnDlg();
	return true;
}

// 通过姓名获取宾客信息
bool CHotelSystemSearchGuestDlg::GetInfoByGuestName()
{
	CString tmp;
	m_editGuestName.GetWindowTextW(tmp);
	if (tmp == _T(""))
	{
		m_editGuestName.ShowBalloonTip(_T("提示"), _T("请填写此字段"), TTI_INFO);
		return false;
	}
	// 无论如何我要获取一遍guestid
	CString sqlWhere;
	sqlWhere.Format(_T("SELECT id FROM guest WHERE name='%s'"), tmp.GetBuffer());
	tmp.ReleaseBuffer();
	try
	{
		g_mysql.excuteQuery(sqlWhere);
		if (!g_mysql.resultNext())
		{
			MessageBox(_T("从数据库获取信息失败"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "结果集为空", GetLastError());
			return false;
		}
		m_guest.m_nID = g_mysql.getResultSet()->getInt("id");
	}
	catch (const sql::SQLException & e)
	{
		MessageBox(_T("从数据库获取信息失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	if (!GetInfoFromDBByID(m_guest.m_nID))
		return false;
	ShowInfoOnDlg();
	return true;
}

// 通过guestid来查询信息
bool CHotelSystemSearchGuestDlg::GetInfoFromDBByID(int __id)
{
	if (__id <= 0)
		return false;
	try
	{
		CString sql;
		// 依据guestid查询宾客信息
		sql.Format(_T("SELECT name,sex,years,idcard_type,idcard_id,phone,address,companyname,companyaddr FROM guest WHERE id=%d"), __id);
		g_mysql.excuteQuery(sql);
		if (!g_mysql.resultNext())
			return false;
		m_guest.m_strName = g_mysql.getResultSet()->getString("name");
		m_guest.m_strSex = g_mysql.getResultSet()->getString("sex");
		m_guest.m_strYearOld = g_mysql.getResultSet()->getString("years");
		m_guest.m_strCompName = g_mysql.getResultSet()->getString("companyname");
		m_guest.m_strCompAddr = g_mysql.getResultSet()->getString("companyaddr");
		m_guest.m_strIDCardType = g_mysql.getResultSet()->getString("idcard_type");
		m_guest.m_strIDCardID = g_mysql.getResultSet()->getString("idcard_id");
		m_guest.m_strPhone = g_mysql.getResultSet()->getString("phone");
		m_guest.m_strAddress = g_mysql.getResultSet()->getString("address");
	}
	catch (const sql::SQLException & e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

// 在页面显示信息
void CHotelSystemSearchGuestDlg::ShowInfoOnDlg()
{
	if (m_guest.m_nID == -1)
		return;
	USES_CONVERSION;
	m_editGName.SetWindowTextW(A2W(m_guest.m_strName.c_str()));
	m_comboGSex.SetWindowTextW(A2W(m_guest.m_strSex.c_str()));
	m_editGYear.SetWindowTextW(A2W(m_guest.m_strYearOld.c_str()));
	m_editGCompName.SetWindowTextW(A2W(m_guest.m_strCompName.c_str()));
	m_editGCompAddr.SetWindowTextW(A2W(m_guest.m_strCompAddr.c_str()));
	m_comboGIDCardType.SetWindowTextW(A2W(m_guest.m_strIDCardType.c_str()));
	m_editGIDCardID.SetWindowTextW(A2W(m_guest.m_strIDCardID.c_str()));
	m_editGPhone.SetWindowTextW(A2W(m_guest.m_strPhone.c_str()));
	m_editGAddr.SetWindowTextW(A2W(m_guest.m_strAddress.c_str()));
}

// 基类指针指向派生类对象用来获取对象信息
std::string CHotelSystemSearchGuestDlg::GetInputFromCWnd(CWnd* __inp, std::string& __ref)
{
		std::string ret;
		CString tmp;
		__inp->GetWindowTextW(tmp);
		if (tmp == _T(""))
			return __ref;	// 框为空的话直接返回参考字段
		USES_CONVERSION;
		ret = W2A(tmp.GetBuffer());
		tmp.ReleaseBuffer();
		return ret;
}

// 检索按钮点击事件响应
void CHotelSystemSearchGuestDlg::OnBnClickedButtonSearch()
{
	if(!(m_bUseRoomID ? GetInfoByRoomID() : GetInfoByGuestName()))
	{
		MessageBox(_T("检索失败,请检查输入信息是否正确"), 0, MB_ICONERROR | MB_OK);
		return;
	}
	m_guestChanged = m_guest;	// 拷贝一份做日后修改使用
}

// 重新载入按钮事件响应
void CHotelSystemSearchGuestDlg::OnBnClickedButtonReload()
{
	if (m_guest.m_nID == -1)
	{
		MessageBox(_T("请先检索一个宾客"), 0, MB_ICONERROR | MB_OK);
		return;
	}
	ShowInfoOnDlg();
}

// 保存修改按钮事件响应
void CHotelSystemSearchGuestDlg::OnBnClickedButtonSave()
{
	// 让我们先来从诸多的输入框中获取信息
	m_guestChanged.m_strName = GetInputFromCWnd(&m_editGName, m_guest.m_strName);
	m_guestChanged.m_strSex = GetInputFromCWnd(&m_comboGSex, m_guest.m_strSex);
	m_guestChanged.m_strYearOld = GetInputFromCWnd(&m_editGYear, m_guest.m_strYearOld);
	m_guestChanged.m_strCompName = GetInputFromCWnd(&m_editGCompName, m_guest.m_strCompName);
	m_guestChanged.m_strCompAddr = GetInputFromCWnd(&m_editGCompAddr, m_guest.m_strCompAddr);
	m_guestChanged.m_strIDCardType = GetInputFromCWnd(&m_comboGIDCardType, m_guest.m_strIDCardType);
	m_guestChanged.m_strIDCardID = GetInputFromCWnd(&m_editGIDCardID, m_guest.m_strIDCardID);
	m_guestChanged.m_strPhone = GetInputFromCWnd(&m_editGPhone, m_guest.m_strPhone);
	m_guestChanged.m_strAddress = GetInputFromCWnd(&m_editGAddr, m_guest.m_strAddress);
	// 将信息更新至数据库
	CString sql;
	USES_CONVERSION;
	sql.Format(_T("UPDATE guest SET name='%s',sex='%s',years='%s',idcard_type='%s',idcard_id='%s',phone='%s',address='%s',companyname='%s',companyaddr='%s' WHERE id=%d")
		, A2W(m_guestChanged.m_strName.c_str()), A2W(m_guestChanged.m_strSex.c_str()), A2W(m_guestChanged.m_strYearOld.c_str())
		, A2W(m_guestChanged.m_strIDCardType.c_str()), A2W(m_guestChanged.m_strIDCardID.c_str()), A2W(m_guestChanged.m_strPhone.c_str())
		, A2W(m_guestChanged.m_strAddress.c_str()), A2W(m_guestChanged.m_strCompName.c_str()), A2W(m_guestChanged.m_strCompAddr.c_str())
		, m_guest.m_nID
	);
	try
	{
		if (0 >= g_mysql.excuteUpdate(sql))
		{
			MessageBox(_T("更新到数据库时发生错误, 语句未能产生作用"), 0, MB_ICONERROR | MB_OK);
			return;		
		}
	}
	catch (const sql::SQLException & e)
	{
		MessageBox(_T("更新到数据库时发生错误, 语句抛出异常"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return;
	}
	// 重新赋值,然后显示
	m_guest = m_guestChanged;
	ShowInfoOnDlg();
	MessageBox(_T("宾客信息更新成功"), 0, MB_ICONINFORMATION | MB_OK);
}
