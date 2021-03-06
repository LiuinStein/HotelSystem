#include "stdafx.h"
#include "GlobalVariable.h"
#include <cctype>
#include <atlconv.h>

bool db::CMysql::initMysql()
{
	try
	{
		char info[255];
		m_pDriver = sql::mysql::get_driver_instance();
		m_pConn = m_pDriver->connect(c_szHostName, c_szUserName, c_szPassword);
		sprintf_s(info, "使用用户名 %s 连接到数据库服务器 %s", c_szUserName, c_szHostName);
		g_log.insertNewError(aduit::e_info, info);
		m_pConn->setSchema(c_szDBName);
		sprintf_s(info, "使用数据库%s", c_szDBName);
		g_log.insertNewError(aduit::e_info, info);
		m_pStatement = m_pConn->createStatement();
		m_pStatement->execute("set names 'GBK'");		// 设置数据库编码格式
	}
	catch (sql::SQLException &e)
	{
		g_log.insertNewError(aduit::e_error, e.what());
		return false;
	}
	catch (std::runtime_error &e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

bool db::CMysql::initIsSuccess() const
{
	return m_bInitSuccess;
}

// 执行返回式SQL语句
const sql::ResultSet* db::CMysql::excuteQuery(const char* __sql)
{
	try
	{
		if (!m_bInitSuccess)
			initMysql();
		m_pRes = m_pStatement->executeQuery(__sql);
		return m_pRes;
	}
	catch (const sql::SQLException &e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return nullptr;
	}
}

// 执行返回式SQL语句
const sql::ResultSet* db::CMysql::excuteQuery(CString& __sql)
{
	USES_CONVERSION;
	char * szQuery = W2A(__sql.GetBuffer());
	__sql.ReleaseBuffer();
	return excuteQuery(szQuery);
}

// 执行返回式SQL语句
const sql::ResultSet* db::CMysql::excuteQuery(const std::string& __sql)
{
	return excuteQuery(__sql.c_str());
}

// 执行非返回式SQL语句
int db::CMysql::excuteUpdate(const char* __sql)
{
	try
	{
		if (!m_bInitSuccess)
			initMysql();
		m_pConn->setAutoCommit(false);
		int nRet = m_pStatement->executeUpdate(__sql);
		m_pConn->commit();		// 提交SQL语句
		m_pConn->setAutoCommit(true);
		return nRet;
	}
	catch (const sql::SQLException &e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return -1;
	}
}

// 执行非返回式SQL语句
int db::CMysql::excuteUpdate(CString& __sql)
{
	USES_CONVERSION;
	char * szQuery = W2A(__sql.GetBuffer());
	__sql.ReleaseBuffer();
	return excuteUpdate(szQuery);
}

// 执行非返回式SQL语句
int db::CMysql::excuteUpdate(const std::string& __sql)
{
	return excuteUpdate(__sql.c_str());
}

bool db::CMysql::resultNext()
{
	return m_pRes->next();
}

const sql::ResultSet* db::CMysql::getResultSet() const
{
	return m_pRes;
}

// 将结果集放入CVector
template<typename T>
bool db::CMysql::resultGetColVector(T(*__get)(const sql::SQLString &), const sql::SQLString & __col, stl::CVector<T>& __vec)
{
	if (!__get || !m_pRes)
	{
		g_log.insertNewError(aduit::e_error, "resultGetColVector指针悬垂", GetLastError());
		return false;
	}
	try
	{
		__vec.clear();
		while (m_pRes->previous());	// 游标回退
		while (m_pRes->next())
			__vec.push_back(__get(__col));
	}
	catch (const sql::SQLException &e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	catch (const std::exception &e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

bool db::SQLIsBad(const char* __sql)
{
	size_t len = strlen(__sql) + 2;
	char * tmp = new char[len];
	strcpy_s(tmp, len,__sql);
	for (size_t i = 0; i < len; i++)
		tmp[i] = tolower(tmp[i]);
	char * block[] = { "select","union", "'", "\"" };
	for (int i = 0; i < ARRAYSIZE(block); i++)
		if (strstr(tmp, block[i]))
			return true;
	return false;
}

bool db::SQLIsBad(CString& __sql)
{
	USES_CONVERSION;
	char * szSQL = W2A(__sql.GetBuffer());
	__sql.ReleaseBuffer();
	return SQLIsBad(szSQL);
}

db::CMysql::CMysql()
{
	m_bInitSuccess = true;
	if (!initMysql())
	{
		m_pConn = nullptr;
		m_pDriver = nullptr;
		m_pStatement = nullptr;
		m_bInitSuccess = false;
	}
}

db::CMysql::~CMysql()
{
}

db::CMysql g_mysql;

