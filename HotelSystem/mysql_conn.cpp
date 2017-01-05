#include "stdafx.h"
#include "mysql_conn.h"
#include "Log.h"
#include <cctype>

bool db::CMysql::initMysql()
{
	try
	{
		char info[255];
		m_pDriver = sql::mysql::get_driver_instance();
		m_pConn = m_pDriver->connect(c_szHostName, c_szUserName, c_szPassword);
		m_pConn->setSchema(c_szDBName);
		sprintf_s(info, "Use Database %s", c_szDBName);
		aduit::log.insertNewError(aduit::e_info, info);
		m_pStatement = m_pConn->createStatement();
		m_pConn->setAutoCommit(0);
		sprintf_s(info, "Connect to %s use %s", c_szHostName, c_szUserName);
		aduit::log.insertNewError(aduit::e_info, info);
	}
	catch (sql::SQLException &e)
	{
		aduit::log.insertNewError(aduit::e_error, e.what());
		return false;
	}
	catch (std::runtime_error &e)
	{
		aduit::log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return false;
	}
	return true;
}

bool db::CMysql::initIsSuccess() const
{
	return m_bInitSuccess;
}

// ִ��SQL���
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
		aduit::log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return nullptr;
	}
	catch (const std::exception&e)
	{
		aduit::log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return nullptr;
	}
}

bool db::CMysql::resultNext()
{
	return m_pRes->next();
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
