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
	, m_nCilckListLine(-1)
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
	ON_BN_CLICKED(IDC_BUTTON_APPENDROOM, &CHotelSystemCheckinDlg::OnBnClickedButtonAppendroom)
	ON_BN_CLICKED(IDC_BUTTON_CALC, &CHotelSystemCheckinDlg::OnBnClickedButtonCalc)
	ON_BN_CLICKED(IDC_BUTTON_REMOVEROOM, &CHotelSystemCheckinDlg::OnBnClickedButtonRemoveroom)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ACCOUNT, &CHotelSystemCheckinDlg::OnClickListAccount)
	ON_BN_CLICKED(IDC_BUTTON_PAYANDSAVE, &CHotelSystemCheckinDlg::OnBnClickedButtonPayandsave)
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
	// 初始化气泡
	EnableToolTips(TRUE);
	// 性别下拉框
	m_comboGuestSex.InsertString(0, _T("男"));
	m_comboGuestSex.InsertString(1, _T("女"));
	// 证件类型
	m_comboGuestIDCardType.InsertString(0, _T("身份证"));
	m_comboGuestIDCardType.InsertString(1, _T("军人证"));
	m_comboGuestIDCardType.InsertString(2, _T("护照"));
	m_comboGuestIDCardType.InsertString(3, _T("港澳同胞证"));
	// 支付方式
	m_comboPayMethod.InsertString(0, _T("现金"));
	m_comboPayMethod.InsertString(1, _T("银行卡"));
	m_comboPayMethod.InsertString(2, _T("支付宝"));
	m_comboPayMethod.InsertString(3, _T("微信"));
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
	// 初始化账单详情列表框
	// 标记全行选中和栅栏风格
	m_listAccount.SetExtendedStyle(m_listAccount.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// 插入列
	m_listAccount.InsertColumn(0, _T("房间号"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(1, _T("房间类型"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(2, _T("原价每天"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(3, _T("折扣每天"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(4, _T("现价每天"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(5, _T("预留时间"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(6, _T("此项共计"), LVCFMT_CENTER, 100, 50);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// 点击是否折扣单选框
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

// 追加房间按钮
void CHotelSystemCheckinDlg::OnBnClickedButtonAppendroom()
{
	if(!GetInfoFromDlg())
		return;
	if(m_vecRoom.size() == 0)
	{
		g_log.insertNewError(aduit::e_error, "m_vecRoom.size()异常,异常位于CHotelSystemCheckinDlg::OnBnClickedButtonAppendroom()", GetLastError());
		return;
	}
	// 插入列表
	for (int i = 0; i < m_vecRoom.size() - 1; i++)
		if(m_vecRoom[i].m_basicInfo.m_nRoomID == m_vecRoom[m_vecRoom.size() - 1].m_basicInfo.m_nRoomID)
		{
			m_vecRoom.remove(m_vecRoom.size() - 1);
			MessageBox(_T("此房间您已预约过"), 0, MB_ICONINFORMATION | MB_OK);
			return;
		}
	m_listAccount.InsertItem(m_vecRoom.size() - 1, m_vecRoom[m_vecRoom.size() - 1].m_cstrRoomid);
	m_listAccount.SetItemText(m_vecRoom.size() - 1, 1, m_vecRoom[m_vecRoom.size() - 1].m_cstrRoomType);
	m_listAccount.SetItemText(m_vecRoom.size() - 1, 2, m_vecRoom[m_vecRoom.size() - 1].m_cstrPrePrice);
	m_listAccount.SetItemText(m_vecRoom.size() - 1, 3, m_vecRoom[m_vecRoom.size() - 1].m_cstrDiscounted);
	m_listAccount.SetItemText(m_vecRoom.size() - 1, 4, m_vecRoom[m_vecRoom.size() - 1].m_cstrPrice);
	m_listAccount.SetItemText(m_vecRoom.size() - 1, 5, m_vecRoom[m_vecRoom.size() - 1].m_cstrStayDay);
	m_listAccount.SetItemText(m_vecRoom.size() - 1, 6, m_vecRoom[m_vecRoom.size() - 1].m_cstrTotal);
	emptyRoomInfo();
}

// 从界面获取信息
bool CHotelSystemCheckinDlg::GetInfoFromDlg()
{
	// 先获取必需的宾客信息
	if (!(
		GetInfoFromEdit(m_editGuestName, m_sGuest.m_strName) &&	// 姓名
		GetInfoFromCombo(m_comboGuestSex, m_sGuest.m_strSex) &&	// 性别
		GetInfoFromCombo(m_comboGuestIDCardType, m_sGuest.m_strIDCardType) &&	// 证件类型
		GetInfoFromEdit(m_editGuestIDCardID, m_sGuest.m_strIDCardID) &&	// 证件编号
		GetInfoFromEdit(m_editGuestPhone, m_sGuest.m_strPhone)	// 联系电话
		))
		return false;
	// 获取非必需的宾客信息
	GetInfoFromEdit(m_editGuestYearOld, m_sGuest.m_strYearOld, false);	// 年龄
	GetInfoFromEdit(m_editGuestAddr, m_sGuest.m_strAddress, false);	// 联系地址
	GetInfoFromEdit(m_editGuestCompName, m_sGuest.m_strCompName, false);	// 公司名
	GetInfoFromEdit(m_editGuestCompAddr, m_sGuest.m_strCompAddr, false);	// 公司地址

	// 获取房间信息(全部必需)
	SRoomInfo tmp;
	tmp.m_basicInfo.m_nIsDirty = false;	// 新房默认干净
	// 获取房间类型
	tmp.m_basicInfo.m_nTypeID = m_comboRoomType.GetCurSel() + 1;
	m_comboRoomType.GetWindowTextW(tmp.m_cstrRoomType);
	std::string tmpstr;
	// 获取房间号
	if (!GetInfoFromCombo(m_comboRoomNumber, tmpstr))
		return false;
	tmp.m_basicInfo.m_nRoomID = atoi(tmpstr.c_str());
	tmp.m_cstrRoomid = tmpstr.c_str();
	// 获取预留时间
	if (!GetInfoFromEdit(m_editStayTime, tmpstr))
		return false;
	tmp.m_stayDay = atoi(tmpstr.c_str());
	tmp.m_cstrStayDay = tmpstr.c_str();
	// 获取折扣
	if (m_bIsDiscounted)
	{
		if (!GetInfoFromEdit(m_editPayDiscounted, tmpstr))
			return false;
		tmp.m_despoit = double(atoi(tmpstr.c_str())) / 10.0;
		if(tmp.m_despoit > 1.0 || tmp.m_despoit <= 0)
		{
			m_editPayDiscounted.ShowBalloonTip(_T("提示"), _T("折扣范围必需小于10且大于等于1"), TTI_INFO);
			return false;
		}
	}
	else
		tmp.m_despoit = 1;
	// 从数据库获取房间原价
	std::string sql("SELECT price FROM room_type WHERE id=");
	sql += char(tmp.m_basicInfo.m_nTypeID + '0');
	try
	{
		g_mysql.excuteQuery(sql);
		if (g_mysql.resultNext())
			tmp.m_prePay = g_mysql.getResultSet()->getDouble("price");
	}
	catch (const sql::SQLException & e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	tmp.m_cstrPrePrice.Format(_T("%.2lf"), tmp.m_prePay);
	// 获取折扣金额和现价
	tmp.m_cstrDiscounted.Format(_T("-%.2lf"), (1.0 - tmp.m_despoit) * tmp.m_prePay);
	tmp.m_cstrPrice.Format(_T("%.2lf"), tmp.m_despoit * tmp.m_prePay);
	// 计算此项共计费用
	tmp.m_cstrTotal.Format(_T("%.2lf"), tmp.m_despoit * tmp.m_prePay * tmp.m_stayDay);

	tmp.m_basicInfo.m_nGuestID = 0;
	m_vecRoom.push_back(tmp);
	return true;
}

// 从编辑框从获取信息
bool CHotelSystemCheckinDlg::GetInfoFromEdit(CEdit& __from, std::string & __info, bool __isnessary)
{
	CString tmp;
	__from.GetWindowTextW(tmp);
	if (__isnessary && tmp == _T(""))
	{
		__from.ShowBalloonTip(_T("提示"), _T("请填写此字段"), TTI_INFO);
		return false;
	}
	USES_CONVERSION;
	__info = W2A(tmp.GetBuffer());
	tmp.ReleaseBuffer();
	return true;
}

// 获取选中的编辑框的信息
bool CHotelSystemCheckinDlg::GetInfoFromCombo(CComboBox& __from, std::string& __info, bool __isnessary)
{
	CString tmp;
	__from.GetLBText(__from.GetCurSel(), tmp);
	if(__isnessary && tmp == _T(""))
		return false;
	USES_CONVERSION;
	__info = W2A(tmp.GetBuffer());
	tmp.ReleaseBuffer();
	return true;
}

// 用来置空房间信息栏
void CHotelSystemCheckinDlg::emptyRoomInfo()
{
	m_comboRoomNumber.ResetContent();
	m_comboRoomFloor.ResetContent();
	m_comboRoomType.SetWindowTextW(_T(""));
	m_editStayTime.SetWindowTextW(_T(""));
	m_editPayDiscounted.SetWindowTextW(_T(""));
}

// 计算总价按钮事件响应
void CHotelSystemCheckinDlg::OnBnClickedButtonCalc()
{
	std::string strDeposit;
	if (!GetInfoFromEdit(m_editPayDeposite, strDeposit))
		return;
	int nDeposit{ atoi(strDeposit.c_str()) };
	m_sGuest.m_dDeposit = nDeposit;
	double dPrePrice{};
	double dDiscounted{};
	double dTotal{};
	for (int i = 0; i < m_vecRoom.size(); i++)
	{
		dPrePrice += m_vecRoom[i].m_prePay * m_vecRoom[i].m_stayDay;
		dDiscounted += (1.0 - m_vecRoom[i].m_despoit) * m_vecRoom[i].m_prePay;
		dTotal += m_vecRoom[i].m_despoit * m_vecRoom[i].m_prePay * m_vecRoom[i].m_stayDay;
	}
	dTotal += nDeposit;
	CString tmp;
	tmp.Format(_T("%.2lf"), dPrePrice);
	m_editPayPrePrice.SetWindowTextW(tmp);
	tmp.Format(_T("%.2lf"), dDiscounted);
	m_editPayLess.SetWindowTextW(tmp);
	tmp.Format(_T("%.2lf"), dTotal);
	m_editPayTotal.SetWindowTextW(tmp);
}

// 移除房间按钮事件响应
void CHotelSystemCheckinDlg::OnBnClickedButtonRemoveroom()
{
	if (m_nCilckListLine == -1)
		return;
	CString cstrDelRoomID = m_listAccount.GetItemText(m_nCilckListLine, 0);
	if (cstrDelRoomID == _T(""))
		return;
	// 或许,比较整数的效率要比比较字符串的效率高吧
	int nDelRoomID{ _ttoi(cstrDelRoomID) };
	for (int i = 0; i < m_vecRoom.size(); i++)
		if(m_vecRoom[i].m_basicInfo.m_nRoomID == nDelRoomID)
		{
			m_vecRoom.remove(i);
			break;
		}
	// 删除信息
	m_listAccount.DeleteItem(m_nCilckListLine);
	m_nCilckListLine = -1;	// 干完活之后一定马上复原
}

// 点击控件获取行号
void CHotelSystemCheckinDlg::OnClickListAccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW * pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);
	m_nCilckListLine = pNMListView->iItem;	// 获取选中行信息
	*pResult = 0;
}

// 支付并保存按钮事件响应
void CHotelSystemCheckinDlg::OnBnClickedButtonPayandsave()
{
	std::string tmp;
	GetInfoFromCombo(m_comboPayMethod, tmp);
	try
	{
		USES_CONVERSION;
		// guest表没有外键约束,先加guest表
		CString sql{ "INSERT INTO `guest` VALUES (" };
		CString cstrtmp;
		// 获取id
		std::string sqltmp{ "SELECT MAX(id)+1 AS id FROM guest" };
		g_mysql.excuteQuery(sqltmp);
		int nID{};
		if (g_mysql.resultNext())
			nID = g_mysql.getResultSet()->getInt("id");
		cstrtmp.Format(_T("'%d', "), nID);
		sql += cstrtmp;
		// 获取name,必填
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strName.c_str()));
		sql += cstrtmp;
		// 获取性别,必填
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strSex.c_str()));
		sql += cstrtmp;
		// 获取年龄,选填
		m_sGuest.m_strYearOld == "" ? cstrtmp = _T("NULL, ") : cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strYearOld.c_str()));
		sql += cstrtmp;
		// 获取证件类型,必填
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strIDCardType.c_str()));
		sql += cstrtmp;
		// 获取证件编号,必填
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strIDCardID.c_str()));
		sql += cstrtmp;
		// 获取联系电话,必填
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strPhone.c_str()));
		sql += cstrtmp;
		// 获取联系地址,选填
		m_sGuest.m_strAddress == "" ? cstrtmp = _T("NULL, ") : cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strAddress.c_str()));
		sql += cstrtmp;
		// 获取公司名称,选填
		m_sGuest.m_strCompName == "" ? cstrtmp = _T("NULL, ") : cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strCompName.c_str()));
		sql += cstrtmp;
		// 获取公司地址,选填
		m_sGuest.m_strCompAddr == "" ? cstrtmp = _T("NULL, ") : cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strCompAddr.c_str()));
		sql += cstrtmp;
		// 获取入住时间,必填
		CTime tNow = CTime::GetCurrentTime();
		CString cstrTime;
		cstrTime.Format(_T("%d-%d-%d %d:%d:%d"), tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
		CString cstrNowTime(cstrTime);
		cstrtmp.Format(_T("'%s', "), cstrTime.GetBuffer());
		sql += cstrtmp;
		// 获取预计离店时间,必填
		// 在这里考虑到一个客人可能预定多个房间,所以客人的预计离店时间为最长的那一个房间
		UINT nStayDay{};
		for (int i = 0; i < m_vecRoom.size(); i++)
			if (m_vecRoom[i].m_stayDay > nStayDay)
				nStayDay = m_vecRoom[i].m_stayDay;
		// 计算并累加时间差
		tNow += CTimeSpan(nStayDay, 0, 0, 0);
		cstrTime.Format(_T("%d-%d-%d %d:%d:%d"), tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
		cstrtmp.Format(_T("'%s', "), cstrTime.GetBuffer());
		sql += cstrtmp;
		// 获取押金数目,必填
		cstrtmp.Format(_T("'%.2lf')"), m_sGuest.m_dDeposit);
		sql += cstrtmp;
		if(g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("数据库更新失败"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "CHotelSystemCheckinDlg::OnBnClickedButtonPayandsave()数据库更新失败-1", GetLastError());
			return;
		}

		// 更新room表的房间信息
		// 可能一个客户会订购多个房间
		for (int i = 0; i < m_vecRoom.size(); i++)
		{
			sql = _T("");
			sql.Format(_T("UPDATE room SET guestid=%d, dirty=0, checkintime='%s', checkouttime='%s', unitprice='%s' WHERE id=%d"), 
				nID, cstrNowTime.GetBuffer(), cstrTime.GetBuffer(), m_vecRoom[i].m_cstrPrice.GetBuffer(), m_vecRoom[i].m_basicInfo.m_nRoomID);
			m_vecRoom[i].m_cstrPrice.ReleaseBuffer();
			if (g_mysql.excuteUpdate(sql) <= 0)
			{
				MessageBox(_T("数据库更新失败"), 0, MB_ICONERROR | MB_OK);
				g_log.insertNewError(aduit::e_error, "CHotelSystemCheckinDlg::OnBnClickedButtonPayandsave()数据库更新失败-2", GetLastError());
				return;
			}
		}
		// 释放CString缓冲区
		cstrTime.ReleaseBuffer();
		cstrNowTime.ReleaseBuffer();
		// 提示并进行下一步操作
		MessageBox(_T("信息保存成功"), 0, MB_ICONINFORMATION | MB_OK);
		if(MessageBox(_T("是否关闭此窗口"),0,MB_ICONINFORMATION | MB_YESNO) == IDYES)
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
