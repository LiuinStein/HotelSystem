#pragma once
#ifndef __MYSQL_CONN_H_
#define __MYSQL_CONN_H_
#include <mysql_driver.h>
#include <cppconn/statement.h>
#include "Vector.h"


namespace db
{
	static const char * c_szHostName = "tcp://127.0.0.1:3307";
	static const char * c_szUserName = "root";
	static const char * c_szPassword = "1021";
	static const char * c_szDBName = "hotel";

	class CMysql
	{
		sql::Driver * m_pDriver;
		sql::Connection * m_pConn;
		sql::Statement * m_pStatement;
		sql::ResultSet * m_pRes;

		bool m_bInitSuccess;
		bool initMysql();
	public:
		bool initIsSuccess()const;
		const sql::ResultSet * excuteQuery(const char * __sql);
		const sql::ResultSet * excuteQuery(CString & __sql);
		bool resultNext();
		const sql::ResultSet * getResultSet()const;
		template<typename T>
		bool resultGetColVector(T(*__get)(const sql::SQLString &), const sql::SQLString & __col, stl::CVector<T> & __vec);

		CMysql();
		~CMysql();
	};


	//  ‰»Î∑¿◊¢»Î
	bool SQLIsBad(const char * __sql);

	bool SQLIsBad(CString & __sql);

}


#endif // !__MYSQL_CONN_H_
