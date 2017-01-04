#include "stdafx.h"
#include "mysql_conn.h"
#include "Log.h"

bool db::InitMysql()
{
	try
	{
		// 初始化驱动
		g_pDriver = sql::mysql::get_driver_instance();
		// 建立连接
		g_pConn = g_pDriver->connect(c_szHostName, c_szUserName, c_szPassword);
		g_pStatement = g_pConn->createStatement();
		char szDBChoose[25]{};
		sprintf_s(szDBChoose, "use %s", c_szDBName);
		g_pStatement->execute(szDBChoose);	// 选取数据库
	}
	catch (sql::SQLException &e)
	{
		aduit::log.insertNewError(aduit::e_error, e.what());
		return false;
	}
	catch (std::runtime_error &e)
	{
		aduit::log.insertNewError(aduit::e_error, e.what());
		return false;
	}
	return true;
}

bool db::SQLIsBad(char* __sql)
{
	char * block[] = { "select","union" };
	for (int i = 0; i < 2; i++)
		if (strstr(__sql, block[i]))
			return false;
	return true;
}
