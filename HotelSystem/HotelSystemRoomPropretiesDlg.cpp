// HotelSystemRoomPropretiesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemRoomPropretiesDlg.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"


// CHotelSystemRoomPropretiesDlg dialog

IMPLEMENT_DYNAMIC(CHotelSystemRoomPropretiesDlg, CDialogEx)

CHotelSystemRoomPropretiesDlg::CHotelSystemRoomPropretiesDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_ROOMPROPRETIES, pParent), m_bAddRoom(true)
{

}

CHotelSystemRoomPropretiesDlg::CHotelSystemRoomPropretiesDlg(bool __isAdd, int __roomid, CWnd* pParent)
	: CDialogEx(IDD_DIALOG_ROOMPROPRETIES, pParent), m_bAddRoom(__isAdd), m_nRoomID(__roomid)
{
}

CHotelSystemRoomPropretiesDlg::~CHotelSystemRoomPropretiesDlg()
{
}

void CHotelSystemRoomPropretiesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ROOMID, m_editRoomID);
	DDX_Control(pDX, IDC_COMBO1, m_comboRoomType);
}


BEGIN_MESSAGE_MAP(CHotelSystemRoomPropretiesDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, &CHotelSystemRoomPropretiesDlg::OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CHotelSystemRoomPropretiesDlg::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CHotelSystemRoomPropretiesDlg message handlers


BOOL CHotelSystemRoomPropretiesDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN &&  pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

// 关闭窗体按钮响应
void CHotelSystemRoomPropretiesDlg::OnBnClickedButtonCancel()
{
	if (MessageBox(_T("是否关闭此窗口"), 0, MB_ICONINFORMATION | MB_YESNO) == IDYES)
	{
		// 关闭窗口
		this->DestroyWindow();
		this->EndDialog(0);
	}
}


BOOL CHotelSystemRoomPropretiesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 获取房间类型
	if (g_vecRoomType.size() == 0 && !data::GetAllRoomType())
	{
		MessageBox(_T("CHotelSystemRoomPropretiesDlg::OnInitDialog()获取房间类型信息失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemRoomPropretiesDlg::OnInitDialog()获取房间类型信息失败");
		EndDialog(IDCANCEL);
		return FALSE;
	}
	for (int i = 0; i < g_vecRoomType.size(); i++)
		m_comboRoomType.InsertString(i, CString(g_vecRoomType[i].m_nName.c_str()));
	// 不是添加房间的话就把edit默认禁用掉
	if(!m_bAddRoom)
	{
		CString tmp;
		tmp.Format(_T("%d"), m_nRoomID);
		m_editRoomID.SetWindowTextW(tmp);
		m_editRoomID.EnableWindow(FALSE);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// 确认修改按钮事件响应
void CHotelSystemRoomPropretiesDlg::OnBnClickedButtonSave()
{
	// 如果是添加房间的话那就获取房间号
	if (m_bAddRoom && !GetInfoFromEdit(m_editRoomID, m_nRoomID))
	{
		MessageBox(_T("读取窗体信息失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemRoomPropretiesDlg::OnBnClickedButtonSave()读取edit失败", GetLastError());
		return;
	}
	// 获取房间类型名
	std::string strRoomType;
	if(!GetInfoFromCombo(m_comboRoomType,strRoomType))
	{
		MessageBox(_T("读取窗体信息失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemRoomPropretiesDlg::OnBnClickedButtonSave()读取combo失败", GetLastError());
		return;
	}
	// 映射成typeid
	if (g_mapRoomID.size() == 0)
		data::GetRoomTypeMap();
	int nTypeID = g_mapRoomID[strRoomType];
	// 这样只更新room表就可以了
	CString sql;
	try
	{
		if (m_bAddRoom)
		{
			// 添加房间的话
			sql.Format(_T("INSERT INTO `room` VALUES ('%d', '%d', '0', '0', null, null, '0');"), m_nRoomID, nTypeID);
			if (g_mysql.excuteUpdate(sql) <= 0)
			{
				MessageBox(_T("数据库添加失败"), 0, MB_ICONERROR | MB_OK);
				g_log.insertNewError(aduit::e_error, sql, GetLastError());
			}
		}
		else
		{
			// 更改房间类型的话
			sql.Format(_T("UPDATE room SET typeid=%d WHERE id=%d"), nTypeID, m_nRoomID);
			if (g_mysql.excuteUpdate(sql) <= 0)
			{
				MessageBox(_T("数据库添加失败"), 0, MB_ICONERROR | MB_OK);
				g_log.insertNewError(aduit::e_error, sql, GetLastError());
			}
		}
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("数据库添加失败"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return;
	}
	MessageBox(_T("添加或修改成功"), 0, MB_ICONINFORMATION | MB_OK);
	if (MessageBox(_T("是否关闭此窗口"), 0, MB_ICONINFORMATION | MB_YESNO) == IDYES)
	{
		// 关闭窗口
		this->DestroyWindow();
		this->EndDialog(0);
	}
}

// 获取选中的编辑框的信息
bool CHotelSystemRoomPropretiesDlg::GetInfoFromCombo(CComboBox& __from, std::string& __info, bool __isnessary)
{
	CString tmp;
	__from.GetLBText(__from.GetCurSel(), tmp);
	if (__isnessary && tmp == _T(""))
		return false;
	USES_CONVERSION;
	__info = W2A(tmp.GetBuffer());
	tmp.ReleaseBuffer();
	return true;
}

// 从编辑框从获取信息
bool CHotelSystemRoomPropretiesDlg::GetInfoFromEdit(CEdit& __from, int & __info, bool __isnessary)
{
	CString tmp;
	__from.GetWindowTextW(tmp);
	if (__isnessary && tmp == _T(""))
	{
		__from.ShowBalloonTip(_T("提示"), _T("请填写此字段"), TTI_INFO);
		return false;
	}
	__info = _ttoi(tmp);
	return true;
}
