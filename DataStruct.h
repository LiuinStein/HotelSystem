#pragma once
#ifndef __DATA_STRUCT_H_
#define __DATA_STRUCT_H_
#include "Vector.h"
#include <string>

namespace data
{
	typedef std::string VARCHAR;
	typedef std::string DATATIME;
	typedef double DECIMAL;
	typedef unsigned char BIT;

	struct SRoomType
	{
		int m_nTypeID;
		VARCHAR m_nName;
		DECIMAL m_dPrice;
		int m_nPeopleNum;
	};
	// ��������������ȡ��������,Ĭ�ϻ�ȡ���з�������
	bool GetGetRoomTypeByCondition(stl::CVector<SRoomType> & __store, const char * __con = "1=1");
	// �����ݿ��л�ȡ���еķ�������
	bool GetAllRoomType();

	struct SRoom
	{
		int m_nRoomID;	
		int m_nTypeID;
		int m_nGuestID;
		bool m_nIsDirty;
	};
	// ����������ȡ����,Ĭ�����ȫ������
	bool GetRoomByCondition(stl::CVector<SRoom> & __store, const char * __con = "1=1");
	// ��ȡ���еķ���
	bool GetAllRoom();
	// ��ȡ���п��÷���
	bool GetAvailableRoom();

	struct SGuest
	{
		int m_nID;
		VARCHAR m_strName;
		VARCHAR m_strSex;
		VARCHAR m_strYearOld;
		VARCHAR m_strIDCardType;
		VARCHAR m_strIDCardID;
		VARCHAR m_strPhone;
		VARCHAR m_strAddress;
		VARCHAR m_strCompName;
		VARCHAR m_strCompAddr;
		DATATIME m_tInTime;
		DATATIME m_tOutTime;
		DECIMAL m_dDeposit;
	};

	struct SItem
	{
		int m_nID;
		int m_nTypeID;
		VARCHAR m_strName;
		DECIMAL m_dPrice;
		int m_nResidue;
	};
	// ����������ȡItem,Ĭ�����ȫ��Item
	bool GetItemByCondition(stl::CVector<SItem> & __store, const char * __con = "1=1");
	// ��ȡ���е�Item
	bool GetAllItem();

}

#endif // !__DATA_STRUCT_H_
