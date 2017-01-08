#include "stdafx.h"
#include "GlobalVariable.h"

// 房间类型
stl::CVector<data::SRoomType> g_vecRoomType;
// 所有的房间信息
stl::CVector<data::SRoom> g_vecAllRoom;
// 获取所有可用房间
stl::CVector<data::SRoom> g_vecAvailableRoom;
// 所有的物品信息
stl::CVector<data::SItem> g_vecAllItem;

bool data::GetGetRoomTypeByCondition(stl::CVector<SRoomType>& __store, const char* __con)
{
	__store.clear();
	std::string sql("SELECT * FROM room_type WHERE ");
	sql += __con;
	try
	{
		g_mysql.excuteQuery(sql);
		while (g_mysql.resultNext())
		{
			SRoomType tmp;
			tmp.m_nTypeID = g_mysql.getResultSet()->getInt("id");
			tmp.m_nName = g_mysql.getResultSet()->getString("name");
			tmp.m_dPrice = g_mysql.getResultSet()->getDouble("price");
			tmp.m_nPeopleNum = g_mysql.getResultSet()->getInt("peoplenum");
			__store.push_back(tmp);
		}
	}
	catch (const sql::SQLException &e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

bool data::GetAllRoomType()
{
	return GetGetRoomTypeByCondition(g_vecRoomType);
}

bool data::GetRoomByCondition(stl::CVector<SRoom>& __store, const char* __con)
{
	__store.clear();
	std::string sql("SELECT * FROM room WHERE ");
	sql += __con;
	try
	{
		g_mysql.excuteQuery(sql);
		while (g_mysql.resultNext())
		{
			SRoom tmp;
			tmp.m_nRoomID = g_mysql.getResultSet()->getInt("id");
			tmp.m_nTypeID = g_mysql.getResultSet()->getInt("typeid");
			tmp.m_nGuestID = g_mysql.getResultSet()->getInt("guestid");
			tmp.m_nIsDirty = g_mysql.getResultSet()->getBoolean("dirty");
			__store.push_back(tmp);
		}
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

bool data::GetAllRoom()
{
	return GetRoomByCondition(g_vecAllRoom);
}

bool data::GetAvailableRoom()
{
	return GetRoomByCondition(g_vecAvailableRoom, "guestid=0");
}

bool data::GetItemByCondition(stl::CVector<SItem>& __store, const char* __con)
{
	__store.clear();
	std::string sql("SELECT * FROM item WHERE ");
	sql += __con;
	try
	{
		g_mysql.excuteQuery(sql);
		while(g_mysql.resultNext())
		{
			SItem tmp;
			tmp.m_nID = g_mysql.getResultSet()->getInt("id");
			tmp.m_nTypeID = g_mysql.getResultSet()->getInt("typeid");
			tmp.m_strName = g_mysql.getResultSet()->getString("name");
			tmp.m_dPrice = g_mysql.getResultSet()->getDouble("price");
			tmp.m_nResidue = g_mysql.getResultSet()->getInt("residue");
			__store.push_back(tmp);
		}
	}
	catch (const sql::SQLException& e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

bool data::GetAllItem()
{
	return GetItemByCondition(g_vecAllItem);
}
