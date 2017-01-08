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
	// 依据所给条件获取房间类型,默认获取所有房间类型
	bool GetGetRoomTypeByCondition(stl::CVector<SRoomType> & __store, const char * __con = "1=1");
	// 从数据库中获取所有的房间类型
	bool GetAllRoomType();

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
	// 依据条件获取Item,默认输出全部Item
	bool GetItemByCondition(stl::CVector<SItem> & __store, const char * __con = "1=1");
	// 获取所有的Item
	bool GetAllItem();

}

#endif // !__DATA_STRUCT_H_
