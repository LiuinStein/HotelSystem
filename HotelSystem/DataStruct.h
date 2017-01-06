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
	// 房间类型
	static stl::CVector<SRoomType> g_vecRoomType;		
	// 从数据库中获取所有的房间类型
	bool GetRoomType();

	
	struct SRoom
	{
		int m_nRoomID;	
		int m_nTypeID;
		int m_nGuestID;
		bool m_nIsDirty;
	};
	// 所有的房间信息
	static stl::CVector<SRoom> g_vecAllRoom;
	// 获取所有的房间
	bool GetAllRoom();
	static stl::CVector<SRoom> g_vecAvailableRoom;
	bool GetAvailableRoom();

}

#endif // !__DATA_STRUCT_H_
