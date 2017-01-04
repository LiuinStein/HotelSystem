#pragma once
#ifndef __MYSQL_CONN_H_
#define __MYSQL_CONN_H_
#include <mysql_driver.h>
#include <cppconn/statement.h>

namespace db
{
	const char * c_szHostName = "tcp://127.0.0.1:3307";
	const char * c_szUserName = "root";
	const char * c_szPassword = "1021";
	const char * c_szDBName = "hotel";

	sql::Driver * g_pDriver{ nullptr };
	sql::Connection * g_pConn{ nullptr };
	sql::Statement * g_pStatement{ nullptr };
	
	// 初始化数据库连接
	bool InitMysql();

	// 输入防注入
	bool SQLIsBad(char * __sql);

}

#endif // !__MYSQL_CONN_H_
