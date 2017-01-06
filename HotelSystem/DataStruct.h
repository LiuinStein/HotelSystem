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
	
	// 从数据库中获取所有的房间类型
	bool GetRoomType();

	struct SRoom
	{
		int m_nRoomID;	
		int m_nTypeID;
		int m_nGuestID;
		bool m_nIsDirty;
	};
	// 依据条件获取房间,默认输出全部房间
	bool GetRoomByCondition(stl::CVector<SRoom> & __store, const char * __con = "1=1");
	// 获取所有的房间
	bool GetAllRoom();
	// 获取所有可用房间
	bool GetAvailableRoom();

}

#endif // !__DATA_STRUCT_H_
