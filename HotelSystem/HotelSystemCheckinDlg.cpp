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
	// �Ա�������
	m_comboGuestSex.InsertString(0, _T("��"));
	m_comboGuestSex.InsertString(1, _T("Ů"));
	// ֤������
	m_comboGuestIDCardType.InsertString(0, _T("���֤"));
	m_comboGuestIDCardType.InsertString(1, _T("����֤"));
	m_comboGuestIDCardType.InsertString(2, _T("����"));
	m_comboGuestIDCardType.InsertString(3, _T("�۰�ͬ��֤"));
	// ��ȡ��������
	if(g_vecRoomType.size() == 0 && !data::GetRoomType())
	{
		MessageBox(_T("CHotelSystemCheckinDlg::OnInitDialog��ȡ����������Ϣʧ��"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, "CHotelSystemCheckinDlg::OnInitDialog��ȡ����������Ϣʧ��");
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

// �û��л��˷�������ѡ����ѡ��
void CHotelSystemCheckinDlg::OnSelchangeComboRoomtype()
{


	//if (g_vecAllRoom.size() == 0 && !data::GetAllRoom())
	//{
	//	MessageBox(_T("OnSelchangeComboRoomtype��ȡ����¥����Ϣʧ��"), 0, MB_ICONERROR | MB_OK);
	//	g_log.insertNewError(aduit::e_error, "OnSelchangeComboRoomtype��ȡ����¥����Ϣʧ��");
	//	return;
	//}
	//char tmp{};
	//for (int i = 0, j = 0; i< g_vecAllRoom.size(); i++)
	//	if (g_vecAllRoom[i].m_nRoomID / 100 != tmp)
	//	{
	//		tmp = g_vecAllRoom[i].m_nRoomID / 100;
	//		m_comboRoomFloor.InsertString(j++, CString(char(tmp + '0')));
	//	}

}
