#pragma once
#ifndef __DATA_STRUCT_H_
#define __DATA_STRUCT_H_
#include "Vector.h"

namespace data
{
	typedef std::string VARCHAR;
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

}

#endif // !__DATA_STRUCT_H_
