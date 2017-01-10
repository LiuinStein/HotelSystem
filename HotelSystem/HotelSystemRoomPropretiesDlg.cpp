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

// �رմ��尴ť��Ӧ
void CHotelSystemRoomPropretiesDlg::OnBnClickedButtonCancel()
{
	if (MessageBox(_T("�Ƿ�رմ˴���"), 0, MB_ICONINFORMATION | MB_YESNO) == IDYES)
	{
		// �رմ���
		this->DestroyWindow();
		this->EndDialog(0);
	}
}


BOOL CHotelSystemRoomPropretiesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��ȡ��������
	if (g_vecRoomType.size() == 0 && !data::GetAllRoomType())
	{
		MessageBox(_T("CHotelSystemRoomPropretiesDlg::OnInitDialog()��ȡ����������Ϣʧ��"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemRoomPropretiesDlg::OnInitDialog()��ȡ����������Ϣʧ��");
		EndDialog(IDCANCEL);
		return FALSE;
	}
	for (int i = 0; i < g_vecRoomType.size(); i++)
		m_comboRoomType.InsertString(i, CString(g_vecRoomType[i].m_nName.c_str()));
	// ������ӷ���Ļ��Ͱ�editĬ�Ͻ��õ�
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

// ȷ���޸İ�ť�¼���Ӧ
void CHotelSystemRoomPropretiesDlg::OnBnClickedButtonSave()
{
	// �������ӷ���Ļ��Ǿͻ�ȡ�����
	if (m_bAddRoom && !GetInfoFromEdit(m_editRoomID, m_nRoomID))
	{
		MessageBox(_T("��ȡ������Ϣʧ��"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemRoomPropretiesDlg::OnBnClickedButtonSave()��ȡeditʧ��", GetLastError());
		return;
	}
	// ��ȡ����������
	std::string strRoomType;
	if(!GetInfoFromCombo(m_comboRoomType,strRoomType))
	{
		MessageBox(_T("��ȡ������Ϣʧ��"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemRoomPropretiesDlg::OnBnClickedButtonSave()��ȡcomboʧ��", GetLastError());
		return;
	}
	// ӳ���typeid
	if (g_mapRoomID.size() == 0)
		data::GetRoomTypeMap();
	int nTypeID = g_mapRoomID[strRoomType];
	// ����ֻ����room��Ϳ�����
	CString sql;
	try
	{
		if (m_bAddRoom)
		{
			// ��ӷ���Ļ�
			sql.Format(_T("INSERT INTO `room` VALUES ('%d', '%d', '0', '0', null, null, '0');"), m_nRoomID, nTypeID);
			if (g_mysql.excuteUpdate(sql) <= 0)
			{
				MessageBox(_T("���ݿ����ʧ��"), 0, MB_ICONERROR | MB_OK);
				g_log.insertNewError(aduit::e_error, sql, GetLastError());
			}
		}
		else
		{
			// ���ķ������͵Ļ�
			sql.Format(_T("UPDATE room SET typeid=%d WHERE id=%d"), nTypeID, m_nRoomID);
			if (g_mysql.excuteUpdate(sql) <= 0)
			{
				MessageBox(_T("���ݿ����ʧ��"), 0, MB_ICONERROR | MB_OK);
				g_log.insertNewError(aduit::e_error, sql, GetLastError());
			}
		}
	}
	catch (const sql::SQLException &e)
	{
		MessageBox(_T("���ݿ����ʧ��"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return;
	}
	MessageBox(_T("��ӻ��޸ĳɹ�"), 0, MB_ICONINFORMATION | MB_OK);
	if (MessageBox(_T("�Ƿ�رմ˴���"), 0, MB_ICONINFORMATION | MB_YESNO) == IDYES)
	{
		// �رմ���
		this->DestroyWindow();
		this->EndDialog(0);
	}
}

// ��ȡѡ�еı༭�����Ϣ
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

// �ӱ༭��ӻ�ȡ��Ϣ
bool CHotelSystemRoomPropretiesDlg::GetInfoFromEdit(CEdit& __from, int & __info, bool __isnessary)
{
	CString tmp;
	__from.GetWindowTextW(tmp);
	if (__isnessary && tmp == _T(""))
	{
		__from.ShowBalloonTip(_T("��ʾ"), _T("����д���ֶ�"), TTI_INFO);
		return false;
	}
	__info = _ttoi(tmp);
	return true;
}
