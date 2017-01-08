// HotelSystemCheckout.cpp : implementation file
//

#include "stdafx.h"
#include "HotelSystem.h"
#include "HotelSystemCheckout.h"
#include "afxdialogex.h"


// CHotelSystemCheckout dialog

IMPLEMENT_DYNAMIC(CHotelSystemCheckout, CDialogEx)

CHotelSystemCheckout::CHotelSystemCheckout(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_CHECKOUT, pParent)
{

}

CHotelSystemCheckout::~CHotelSystemCheckout()
{
}

void CHotelSystemCheckout::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHotelSystemCheckout, CDialogEx)
END_MESSAGE_MAP()


// CHotelSystemCheckout message handlers
