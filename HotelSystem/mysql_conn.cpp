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
		sprintf_s(info, "ʹ���û��� %s ���ӵ����ݿ������ %s", c_szUserName, c_szHostName);
		g_log.insertNewError(aduit::e_info, info);
		m_pConn->setSchema(c_szDBName);
		sprintf_s(info, "ʹ�����ݿ�%s", c_szDBName);
		g_log.insertNewError(aduit::e_info, info);
		m_pStatement = m_pConn->createStatement();
		m_pStatement->execute("set names 'GBK'");		// �������ݿ�����ʽ
		m_pConn->setAutoCommit(0);
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
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return nullptr;
	}
	catch (const std::exception&e)
	{
		g_log.insertNewError(aduit::e_error, e.what(), GetLastError());
		return nullptr;
	}
}

// ִ��SQL���
const sql::ResultSet* db::CMysql::excuteQuery(CString& __sql)
{
	USES_CONVERSION;
	char * szQuery = W2A(__sql.GetBuffer());
	__sql.ReleaseBuffer();
	return excuteQuery(szQuery);
}

bool db::CMysql::resultNext()
{
	return m_pRes->next();
}

const sql::ResultSet* db::CMysql::getResultSet() const
{
	return m_pRes;
}

// �����������CVector
template<typename T>
bool db::CMysql::resultGetColVector(T(*__get)(const sql::SQLString &), const sql::SQLString & __col, stl::CVector<T>& __vec)
{
	if (!__get || !m_pRes)
	{
		g_log.insertNewError(aduit::e_error, "resultGetColVectorָ������", GetLastError());
		return false;
	}
	try
	{
		__vec.clear();
		while (m_pRes->previous());	// �α����
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

