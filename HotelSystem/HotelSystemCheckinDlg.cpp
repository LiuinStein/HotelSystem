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
	// ��ʼ������
	EnableToolTips(TRUE);
	// �Ա�������
	m_comboGuestSex.InsertString(0, _T("��"));
	m_comboGuestSex.InsertString(1, _T("Ů"));
	// ֤������
	m_comboGuestIDCardType.InsertString(0, _T("���֤"));
	m_comboGuestIDCardType.InsertString(1, _T("����֤"));
	m_comboGuestIDCardType.InsertString(2, _T("����"));
	m_comboGuestIDCardType.InsertString(3, _T("�۰�ͬ��֤"));
	// ֧����ʽ
	m_comboPayMethod.InsertString(0, _T("�ֽ�"));
	m_comboPayMethod.InsertString(1, _T("���п�"));
	m_comboPayMethod.InsertString(2, _T("֧����"));
	m_comboPayMethod.InsertString(3, _T("΢��"));
	// ��ȡ��������
	if(g_vecRoomType.size() == 0 && !data::GetAllRoomType())
	{
		MessageBox(_T("CHotelSystemCheckinDlg::OnInitDialog��ȡ����������Ϣʧ��"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemCheckinDlg::OnInitDialog��ȡ����������Ϣʧ��");
		EndDialog(IDCANCEL);
		return FALSE;
	}
	for (int i = 0; i < g_vecRoomType.size(); i++)
		m_comboRoomType.InsertString(i, CString(g_vecRoomType[i].m_nName.c_str()));
	// ��ʼ���˵������б��
	// ���ȫ��ѡ�к�դ�����
	m_listAccount.SetExtendedStyle(m_listAccount.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	// ������
	m_listAccount.InsertColumn(0, _T("�����"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(1, _T("��������"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(2, _T("ԭ��ÿ��"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(3, _T("�ۿ�ÿ��"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(4, _T("�ּ�ÿ��"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(5, _T("Ԥ��ʱ��"), LVCFMT_CENTER, 100, 50);
	m_listAccount.InsertColumn(6, _T("�����"), LVCFMT_CENTER, 100, 50);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

// ����Ƿ��ۿ۵�ѡ��
void CHotelSystemCheckinDlg::OnBnClickedCheckDiscounted()
{
	m_bIsDiscounted = !m_bIsDiscounted;
	m_editPayDiscounted.EnableWindow(m_bIsDiscounted ? TRUE : FALSE);
}

// �û��л��˷�������ѡ����ѡ��
void CHotelSystemCheckinDlg::OnSelchangeComboRoomtype()
{
	// ���û��л���ɵ�ʱ��,�������ʼ��¥����Ϣ��Ͽ�
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
	m_comboRoomFloor.ResetContent();	// ���֮ǰ������
	char tmp{};
	for (int i = 0, j = 0; i< vecRoomFloor.size(); i++)
		if (vecRoomFloor[i] / 100 != tmp)
		{
			tmp = vecRoomFloor[i] / 100;
			m_comboRoomFloor.InsertString(j++, CString(char(tmp + '0')));
		}
}

// �û��л���¥����
void CHotelSystemCheckinDlg::OnSelchangeComboFloor()
{
	// ��ʼ�����÷������Ϣ
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

// ׷�ӷ��䰴ť
void CHotelSystemCheckinDlg::OnBnClickedButtonAppendroom()
{
	if(!GetInfoFromDlg())
		return;
	if(m_vecRoom.size() == 0)
	{
		g_log.insertNewError(aduit::e_error, "m_vecRoom.size()�쳣,�쳣λ��CHotelSystemCheckinDlg::OnBnClickedButtonAppendroom()", GetLastError());
		return;
	}
	// �����б�
	for (int i = 0; i < m_vecRoom.size() - 1; i++)
		if(m_vecRoom[i].m_basicInfo.m_nRoomID == m_vecRoom[m_vecRoom.size() - 1].m_basicInfo.m_nRoomID)
		{
			m_vecRoom.remove(m_vecRoom.size() - 1);
			MessageBox(_T("�˷�������ԤԼ��"), 0, MB_ICONINFORMATION | MB_OK);
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

// �ӽ����ȡ��Ϣ
bool CHotelSystemCheckinDlg::GetInfoFromDlg()
{
	// �Ȼ�ȡ����ı�����Ϣ
	if (!(
		GetInfoFromEdit(m_editGuestName, m_sGuest.m_strName) &&	// ����
		GetInfoFromCombo(m_comboGuestSex, m_sGuest.m_strSex) &&	// �Ա�
		GetInfoFromCombo(m_comboGuestIDCardType, m_sGuest.m_strIDCardType) &&	// ֤������
		GetInfoFromEdit(m_editGuestIDCardID, m_sGuest.m_strIDCardID) &&	// ֤�����
		GetInfoFromEdit(m_editGuestPhone, m_sGuest.m_strPhone)	// ��ϵ�绰
		))
		return false;
	// ��ȡ�Ǳ���ı�����Ϣ
	GetInfoFromEdit(m_editGuestYearOld, m_sGuest.m_strYearOld, false);	// ����
	GetInfoFromEdit(m_editGuestAddr, m_sGuest.m_strAddress, false);	// ��ϵ��ַ
	GetInfoFromEdit(m_editGuestCompName, m_sGuest.m_strCompName, false);	// ��˾��
	GetInfoFromEdit(m_editGuestCompAddr, m_sGuest.m_strCompAddr, false);	// ��˾��ַ

	// ��ȡ������Ϣ(ȫ������)
	SRoomInfo tmp;
	tmp.m_basicInfo.m_nIsDirty = false;	// �·�Ĭ�ϸɾ�
	// ��ȡ��������
	tmp.m_basicInfo.m_nTypeID = m_comboRoomType.GetCurSel() + 1;
	m_comboRoomType.GetWindowTextW(tmp.m_cstrRoomType);
	std::string tmpstr;
	// ��ȡ�����
	if (!GetInfoFromCombo(m_comboRoomNumber, tmpstr))
		return false;
	tmp.m_basicInfo.m_nRoomID = atoi(tmpstr.c_str());
	tmp.m_cstrRoomid = tmpstr.c_str();
	// ��ȡԤ��ʱ��
	if (!GetInfoFromEdit(m_editStayTime, tmpstr))
		return false;
	tmp.m_stayDay = atoi(tmpstr.c_str());
	tmp.m_cstrStayDay = tmpstr.c_str();
	// ��ȡ�ۿ�
	if (m_bIsDiscounted)
	{
		if (!GetInfoFromEdit(m_editPayDiscounted, tmpstr))
			return false;
		tmp.m_despoit = double(atoi(tmpstr.c_str())) / 10.0;
		if(tmp.m_despoit > 1.0 || tmp.m_despoit <= 0)
		{
			m_editPayDiscounted.ShowBalloonTip(_T("��ʾ"), _T("�ۿ۷�Χ����С��10�Ҵ��ڵ���1"), TTI_INFO);
			return false;
		}
	}
	else
		tmp.m_despoit = 1;
	// �����ݿ��ȡ����ԭ��
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
	// ��ȡ�ۿ۽����ּ�
	tmp.m_cstrDiscounted.Format(_T("-%.2lf"), (1.0 - tmp.m_despoit) * tmp.m_prePay);
	tmp.m_cstrPrice.Format(_T("%.2lf"), tmp.m_despoit * tmp.m_prePay);
	// �������Ʒ���
	tmp.m_cstrTotal.Format(_T("%.2lf"), tmp.m_despoit * tmp.m_prePay * tmp.m_stayDay);

	tmp.m_basicInfo.m_nGuestID = 0;
	m_vecRoom.push_back(tmp);
	return true;
}

// �ӱ༭��ӻ�ȡ��Ϣ
bool CHotelSystemCheckinDlg::GetInfoFromEdit(CEdit& __from, std::string & __info, bool __isnessary)
{
	CString tmp;
	__from.GetWindowTextW(tmp);
	if (__isnessary && tmp == _T(""))
	{
		__from.ShowBalloonTip(_T("��ʾ"), _T("����д���ֶ�"), TTI_INFO);
		return false;
	}
	USES_CONVERSION;
	__info = W2A(tmp.GetBuffer());
	tmp.ReleaseBuffer();
	return true;
}

// ��ȡѡ�еı༭�����Ϣ
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

// �����ÿշ�����Ϣ��
void CHotelSystemCheckinDlg::emptyRoomInfo()
{
	m_comboRoomNumber.ResetContent();
	m_comboRoomFloor.ResetContent();
	m_comboRoomType.SetWindowTextW(_T(""));
	m_editStayTime.SetWindowTextW(_T(""));
	m_editPayDiscounted.SetWindowTextW(_T(""));
}

// �����ܼ۰�ť�¼���Ӧ
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

// �Ƴ����䰴ť�¼���Ӧ
void CHotelSystemCheckinDlg::OnBnClickedButtonRemoveroom()
{
	if (m_nCilckListLine == -1)
		return;
	CString cstrDelRoomID = m_listAccount.GetItemText(m_nCilckListLine, 0);
	if (cstrDelRoomID == _T(""))
		return;
	// ����,�Ƚ�������Ч��Ҫ�ȱȽ��ַ�����Ч�ʸ߰�
	int nDelRoomID{ _ttoi(cstrDelRoomID) };
	for (int i = 0; i < m_vecRoom.size(); i++)
		if(m_vecRoom[i].m_basicInfo.m_nRoomID == nDelRoomID)
		{
			m_vecRoom.remove(i);
			break;
		}
	// ɾ����Ϣ
	m_listAccount.DeleteItem(m_nCilckListLine);
	m_nCilckListLine = -1;	// �����֮��һ�����ϸ�ԭ
}

// ����ؼ���ȡ�к�
void CHotelSystemCheckinDlg::OnClickListAccount(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	NM_LISTVIEW * pNMListView = reinterpret_cast<NM_LISTVIEW*>(pNMHDR);
	m_nCilckListLine = pNMListView->iItem;	// ��ȡѡ������Ϣ
	*pResult = 0;
}

// ֧�������水ť�¼���Ӧ
void CHotelSystemCheckinDlg::OnBnClickedButtonPayandsave()
{
	std::string tmp;
	GetInfoFromCombo(m_comboPayMethod, tmp);
	try
	{
		USES_CONVERSION;
		// guest��û�����Լ��,�ȼ�guest��
		CString sql{ "INSERT INTO `guest` VALUES (" };
		CString cstrtmp;
		// ��ȡid
		std::string sqltmp{ "SELECT MAX(id)+1 AS id FROM guest" };
		g_mysql.excuteQuery(sqltmp);
		int nID{};
		if (g_mysql.resultNext())
			nID = g_mysql.getResultSet()->getInt("id");
		cstrtmp.Format(_T("'%d', "), nID);
		sql += cstrtmp;
		// ��ȡname,����
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strName.c_str()));
		sql += cstrtmp;
		// ��ȡ�Ա�,����
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strSex.c_str()));
		sql += cstrtmp;
		// ��ȡ����,ѡ��
		m_sGuest.m_strYearOld == "" ? cstrtmp = _T("NULL, ") : cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strYearOld.c_str()));
		sql += cstrtmp;
		// ��ȡ֤������,����
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strIDCardType.c_str()));
		sql += cstrtmp;
		// ��ȡ֤�����,����
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strIDCardID.c_str()));
		sql += cstrtmp;
		// ��ȡ��ϵ�绰,����
		cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strPhone.c_str()));
		sql += cstrtmp;
		// ��ȡ��ϵ��ַ,ѡ��
		m_sGuest.m_strAddress == "" ? cstrtmp = _T("NULL, ") : cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strAddress.c_str()));
		sql += cstrtmp;
		// ��ȡ��˾����,ѡ��
		m_sGuest.m_strCompName == "" ? cstrtmp = _T("NULL, ") : cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strCompName.c_str()));
		sql += cstrtmp;
		// ��ȡ��˾��ַ,ѡ��
		m_sGuest.m_strCompAddr == "" ? cstrtmp = _T("NULL, ") : cstrtmp.Format(_T("'%s', "), A2W(m_sGuest.m_strCompAddr.c_str()));
		sql += cstrtmp;
		// ��ȡ��סʱ��,����
		CTime tNow = CTime::GetCurrentTime();
		CString cstrTime;
		cstrTime.Format(_T("%d-%d-%d %d:%d:%d"), tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
		CString cstrNowTime(cstrTime);
		cstrtmp.Format(_T("'%s', "), cstrTime.GetBuffer());
		sql += cstrtmp;
		// ��ȡԤ�����ʱ��,����
		// �����￼�ǵ�һ�����˿���Ԥ���������,���Կ��˵�Ԥ�����ʱ��Ϊ�����һ������
		UINT nStayDay{};
		for (int i = 0; i < m_vecRoom.size(); i++)
			if (m_vecRoom[i].m_stayDay > nStayDay)
				nStayDay = m_vecRoom[i].m_stayDay;
		// ���㲢�ۼ�ʱ���
		tNow += CTimeSpan(nStayDay, 0, 0, 0);
		cstrTime.Format(_T("%d-%d-%d %d:%d:%d"), tNow.GetYear(), tNow.GetMonth(), tNow.GetDay(), tNow.GetHour(), tNow.GetMinute(), tNow.GetSecond());
		cstrtmp.Format(_T("'%s', "), cstrTime.GetBuffer());
		sql += cstrtmp;
		// ��ȡѺ����Ŀ,����
		cstrtmp.Format(_T("'%.2lf')"), m_sGuest.m_dDeposit);
		sql += cstrtmp;
		if(g_mysql.excuteUpdate(sql) <= 0)
		{
			MessageBox(_T("���ݿ����ʧ��"), 0, MB_ICONERROR | MB_OK);
			g_log.insertNewError(aduit::e_error, "CHotelSystemCheckinDlg::OnBnClickedButtonPayandsave()���ݿ����ʧ��-1", GetLastError());
			return;
		}

		// ����room��ķ�����Ϣ
		// ����һ���ͻ��ᶩ���������
		for (int i = 0; i < m_vecRoom.size(); i++)
		{
			sql = _T("");
			sql.Format(_T("UPDATE room SET guestid=%d, dirty=0, checkintime='%s', checkouttime='%s', unitprice='%s' WHERE id=%d"), 
				nID, cstrNowTime.GetBuffer(), cstrTime.GetBuffer(), m_vecRoom[i].m_cstrPrice.GetBuffer(), m_vecRoom[i].m_basicInfo.m_nRoomID);
			m_vecRoom[i].m_cstrPrice.ReleaseBuffer();
			if (g_mysql.excuteUpdate(sql) <= 0)
			{
				MessageBox(_T("���ݿ����ʧ��"), 0, MB_ICONERROR | MB_OK);
				g_log.insertNewError(aduit::e_error, "CHotelSystemCheckinDlg::OnBnClickedButtonPayandsave()���ݿ����ʧ��-2", GetLastError());
				return;
			}
		}
		// �ͷ�CString������
		cstrTime.ReleaseBuffer();
		cstrNowTime.ReleaseBuffer();
		// ��ʾ��������һ������
		MessageBox(_T("��Ϣ����ɹ�"), 0, MB_ICONINFORMATION | MB_OK);
		if(MessageBox(_T("�Ƿ�رմ˴���"),0,MB_ICONINFORMATION | MB_YESNO) == IDYES)
		{
			// �رմ���
			this->DestroyWindow();
			this->EndDialog(0);
		}
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}
}
