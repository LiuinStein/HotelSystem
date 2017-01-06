
// HotelSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemDlg.h"
#include "HotelSystemMainDlg.h"
#include "afxdialogex.h"
#include "GlobalVariable.h"
#include <atlconv.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHotelSystemDlg dialog



CHotelSystemDlg::CHotelSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HOTELSYSTEM_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHotelSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_USERNAME, m_editUsername);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_editPassword);
}

BEGIN_MESSAGE_MAP(CHotelSystemDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CHotelSystemDlg::OnBnClickedButtonClear)
	ON_BN_CLICKED(IDC_BUTTON_LOGIN, &CHotelSystemDlg::OnBnClickedButtonLogin)
END_MESSAGE_MAP()


// CHotelSystemDlg message handlers

BOOL CHotelSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// 初始化数据库连接
	if(!g_mysql.initIsSuccess())
	{
		MessageBox(_T("数据库连接失败"), 0, MB_ICONERROR | MB_OK);
		exit(0);
	}
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHotelSystemDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHotelSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHotelSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



BOOL CHotelSystemDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE)
		return TRUE;
	return CDialogEx::PreTranslateMessage(pMsg);
}

// 清空输入按钮
void CHotelSystemDlg::OnBnClickedButtonClear()
{
	m_editUsername.SetWindowTextW(_T(""));
	m_editPassword.SetWindowTextW(_T(""));
}

// 登录按钮
void CHotelSystemDlg::OnBnClickedButtonLogin()
{
	CString inp_username;
	m_editUsername.GetWindowTextW(inp_username);
	CString inp_pwd;
	m_editPassword.GetWindowTextW(inp_pwd);
	if(db::SQLIsBad(inp_username) || db::SQLIsBad(inp_pwd))
	{
		CString info;
		info.Format(_T("SQL Injection Attack use input %s and %s"), inp_username.GetBuffer(), inp_pwd.GetBuffer());
		inp_username.ReleaseBuffer();
		inp_pwd.ReleaseBuffer();
		g_log.insertNewError(aduit::e_warn, info);
		MessageBox(_T("输入了不合法的字符,重新输入"), 0, MB_ICONERROR | MB_OK);
		return;
	}
	try
	{
		CString szSqlquery;
		szSqlquery.Format(_T("SELECT password FROM login WHERE name='%s'"), inp_username.GetBuffer());
		inp_username.ReleaseBuffer();
		const sql::ResultSet * res = g_mysql.excuteQuery(szSqlquery);
		std::string strTruePwd;
		if(g_mysql.resultNext())
			strTruePwd = res->getString(std::string("password"));
		CString pwdCmp(strTruePwd.c_str());
		if(inp_pwd == pwdCmp)
		{
			this->ShowWindow(SW_HIDE);
			CHotelSystemMainDlg mainDlg;
			mainDlg.DoModal();
		}
		else
		{
			MessageBox(_T("登录失败,用户名或密码错误"), 0, MB_ICONERROR | MB_OK);
			m_editPassword.SetWindowTextW(_T(""));
		}
	}
	catch (const sql::SQLException & e)
	{
		MessageBox(_T("登录异常"), 0, MB_ICONERROR | MB_OK);
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
	}

}
