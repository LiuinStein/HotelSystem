#include "stdafx.h"
#include "DataStruct.h"
#include "mysql_conn.h"
#include "Log.h"

bool data::GetRoomType()
{
	g_vecRoomType.clear();
	try
	{
		db::mysql.excuteQuery("SELECT * FROM room_type");
		while(db::mysql.resultNext())
		{
			SRoomType tmp;
			tmp.m_nID = db::mysql.getResultSet()->getInt("id");
			tmp.m_nName = db::mysql.getResultSet()->getString("name");
			tmp.m_dPrice = db::mysql.getResultSet()->getDouble("price");
			tmp.m_nPeopleNum = db::mysql.getResultSet()->getInt("peoplenum");
			g_vecRoomType.push_back(tmp);
		}
	}
	catch (const sql::SQLException &e)
	{
		aduit::log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	catch (const std::exception &e)
	{
		aduit::log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}
