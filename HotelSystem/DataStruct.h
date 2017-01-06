#pragma once
#ifndef __DATA_STRUCT_H_
#define __DATA_STRUCT_H_
#include <string>
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
	// ��������
	static stl::CVector<SRoomType> g_vecRoomType;		
	// �����ݿ��л�ȡ���еķ�������
	bool GetRoomType();

	
	struct SRoom
	{
		int m_nRoomID;	
		int m_nTypeID;
		int m_nGuestID;
		bool m_nIsDirty;
	};
	// ���еķ�����Ϣ
	static stl::CVector<SRoom> g_vecAllRoom;
	// ��ȡ���еķ���
	bool GetAllRoom();
	static stl::CVector<SRoom> g_vecAvailableRoom;
	bool GetAvailableRoom();

}

#endif // !__DATA_STRUCT_H_
